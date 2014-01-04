#include "collisiondetector.hpp"
#include <OgreAxisAlignedBox.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreSceneNode.h>
#include <cmath> //fabs, sign

CollisionDetector::CollisionDetector():
  isCollisionSwitchStillOn(false),
  ActiveAlgorithm(CollisionAlgorithmEnum::None)
{
}

void CollisionDetector::CheckCollision(const Object& objectA, const Object& objectB)
{
  bool collision;
  switch(ActiveAlgorithm)
    {
    case None:
      collision=NoneCollisionAlgorithm(objectA, objectB);
      break;
    case BoundingBox:
      collision=BoundingBoxCollisionAlgorithm(objectA, objectB);
      break;
    case Triangle:
      collision=TriangleCollisionAlgorithm(objectA, objectB);
      break;
    case Complex:
      collision=ComplexCollisionAlgorithm(objectA, objectB);
      break;
    }

  if( collision != isCollisionSwitchStillOn )
    {
      isCollisionSwitchStillOn=collision;
      emit(ObjectsCollisionInAlgorithmChanged(collision));
    }
}

bool CollisionDetector::NoneCollisionAlgorithm(const Object& objectA, const Object& objectB)
{
  return false;
}

bool CollisionDetector::BoundingBoxCollisionAlgorithm(const Object& objectA, const Object& objectB)
{
  Ogre::Vector3 minA=objectA.GetBoundingBox().getMinimum();
  Ogre::Vector3 maxA=objectA.GetBoundingBox().getMaximum();
  Ogre::Vector3 minB=objectB.GetBoundingBox().getMinimum();
  Ogre::Vector3 maxB=objectB.GetBoundingBox().getMaximum();

  //x
  if(maxA.x<minB.x || maxB.x<minA.x)
    return false;
  //y
  if(maxA.y<minB.y || maxB.y<minA.y)
    return false;
  //z
  if(maxA.z<minB.z || maxB.z<minA.z)
    return false;

  return true;
}

bool CollisionDetector::TriangleCollisionAlgorithm(const Object& objectA, const Object& objectB)
{
  //Position
  Ogre::Vector3 positionA=objectA.Node->_getDerivedPosition();
  Ogre::Vector3 positionB=objectB.Node->_getDerivedPosition();

  //Orientation
  Ogre::Quaternion orientationA=objectA.Node->_getDerivedOrientation();
  Ogre::Quaternion orientationB=objectB.Node->_getDerivedOrientation();

  //FullTransform
  Ogre::Matrix4 transformA=objectA.Node->_getFullTransform();
  Ogre::Matrix4 transformB=objectB.Node->_getFullTransform();

  //trzyprzejściowe pętle bardzo wszystko spowalniały, więc trzeba sobie radzić inaczej
  Ogre::Vector3 a0,a1,a2, b0,b1,b2;

  //dla każdego trójkąta A
  for(int faceIndexA=0;faceIndexA<objectA.IndicesBuffer.size()/3;faceIndexA++)
    {
      //trójkąt A
      a0=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA]];
      //std::cout<<a0<<" "<<objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA]]<<" "<<positionA<<std::endl;
      a1=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA+1]];
      a2=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA+2]];

      //sprawdzamy każdy trójkąt B
      for(int faceIndexB=0;faceIndexB<objectB.IndicesBuffer.size()/3;faceIndexB++)
        {
          //trójkąt B
          b0=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB]];
          b1=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB+1]];
          b2=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB+2]];

          if(Moller(a0, a1, a2, b0, b1, b2))
            return true;
        }
    }

  return false;
}

bool CollisionDetector::ComplexCollisionAlgorithm(const Object& objectA, const Object& objectB)
{
  if(BoundingBoxCollisionAlgorithm(objectA, objectB))
    if(TriangleCollisionAlgorithm(objectA, objectB))
      return true;
  return false;
}

//http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/opttritri.txt
#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
  {                                                                     \
    if(D0D1>0.0f)                                                       \
      {                                                                 \
        /* here we know that D0D2<=0.0 */                               \
        /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
        A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;      \
      }                                                                 \
    else if(D0D2>0.0f)                                                  \
      {                                                                 \
        /* here we know that d0d1<=0.0 */                               \
        A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;      \
      }                                                                 \
    else if(D1*D2>0.0f || D0!=0.0f)                                     \
      {                                                                 \
        /* here we know that d0d1<=0.0 or that D0!=0.0 */               \
        A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2;      \
      }                                                                 \
    else if(D1!=0.0f)                                                   \
      {                                                                 \
        A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;      \
      }                                                                 \
    else if(D2!=0.0f)                                                   \ 
      {                                                                 \
        A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;      \
      }                                                                 \
      else\
        {\
          return false;\
        }\
  }

bool CollisionDetector::Moller(const Ogre::Vector3& a0, const Ogre::Vector3& a1, const Ogre::Vector3& a2, const Ogre::Vector3& b0, const Ogre::Vector3& b1, const Ogre::Vector3& b2)
{
  //T1 pi2
  Ogre::Vector3 N1=(a1-a0).crossProduct(a2-a0);
  Ogre::Real d1=-(N1.dotProduct(a0));
  
  Ogre::Real db0,db1,db2;
  
  db0=N1.dotProduct(b0)+d1;
  db1=N1.dotProduct(b1)+d1;
  db2=N1.dotProduct(b2)+d1;

  if(fabs(db0)<0.0001)db0=0;
  if(fabs(db1)<0.0001)db1=0;
  if(fabs(db2)<0.0001)db2=0;

  //T2 pi1
  Ogre::Vector3 N2=(b1-b0).crossProduct(b2-b0);
  Ogre::Real d2=-(N2.dotProduct(b0));
  
  Ogre::Real da0,da1,da2;
  
  da0=N2.dotProduct(a0)+d2;
  da1=N2.dotProduct(a1)+d2;
  da2=N2.dotProduct(a2)+d2;

  if(fabs(da0)<0.0001)da0=0;
  if(fabs(da1)<0.0001)da1=0;
  if(fabs(da2)<0.0001)da2=0;

  Ogre::Real da0da1, da0da2,  db0db1, db0db2;
  
  da0da1=da0*da1;
  da0da2=da0*da2;
  db0db1=db0*db1;
  db0db2=db0*db2;
  
  //These two early rejection tests avoid a lot of computations for some triangle pairs.
  if(da0da1>0 && da0da2>0)
    return false;

  if(db0db1>0 && db0db2>0)
    return false;
  
  //@TODO:triangles are co-planar, and this case is handled separately and discussed later
  //  if(da0==da1==da2==0)
  // return false;

  Ogre::Real pa0, pa1, pa2, pb0, pb1, pb2;

  //2.1 Optimizations -> potrzebuję tylko moduły wektora D
  Ogre::Vector3 fD=N1.crossProduct(N2);
  fD.x=fabs(fD.x);
  fD.y=fabs(fD.y);
  fD.z=fabs(fD.z);

  //znów unikam pętli.
  if(fD.x==fmax(fD.x, fmax(fD.y, fD.z)))
    {
      pa0=a0.x;
      pa1=a1.x;
      pa2=a2.x;
      
      pb0=b0.x;
      pb1=b1.x;
      pb2=b2.x;
    }
  else if(fD.y==fmax(fD.y, fD.z))
    {
      pa0=a0.y;
      pa1=a1.y;
      pa2=a2.y;
      
      pb0=b0.y;
      pb1=b1.y;
      pb2=b2.y;
    }
  else
    {
      pa0=a0.z;
      pa1=a1.z;
      pa2=a2.z;
      
      pb0=b0.z;
      pb1=b1.z;
      pb2=b2.z;
    }
  
  //szukamy odcinków. 
  Ogre::Real ta1,ta2, tb1,tb2, a,b,c,d,e,f, x0,x1,y0,y1;

  NEWCOMPUTE_INTERVALS(pa0,pa1,pa2,da0,da1,da2,da0da1,da0da2,a,b,c,x0,x1);

  NEWCOMPUTE_INTERVALS(pb0,pb1,pb2,db0,db1,db2,db0db1,db0db2,d,e,f,y0,y1);
      
  Ogre::Real xx,yy,xxyy,tmp;
  xx=x0*x1;
  yy=y0*y1;
  xxyy=xx*yy;

  tmp=a*xxyy;
  ta1=tmp+b*x1*yy;
  ta2=tmp+c*x0*yy;
  
  tmp=d*xxyy;
  tb1=tmp+e*xx*y1;
  tb2=tmp+f*xx*y0;
  
  if(fmax(tb1,tb2)<fmin(ta1,ta2) || fmax(ta1,ta2)<fmin(tb1,tb2))
    return false;
  return true;
}
