#include "collisiondetector.hpp"
#include "opttritri.hpp"
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

          //if(Moller(a0, a1, a2, b0, b1, b2))
          if(NoDivTriTriIsect(&a0.x,&a1.x,&a2.x,&b0.x,&b1.x,&b2.x))
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
