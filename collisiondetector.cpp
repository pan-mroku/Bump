#include "collisiondetector.hpp"
#include <OgreAxisAlignedBox.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreSceneNode.h>

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

  //TEST
  /*  int faceIndex=0;
      for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
      {
      int vertexStart=positionElementOffsetInVertexA+vertexSizeInBufferA*indicesA[3*faceIndex+vertexIndex];
      Ogre::Vector3 vertex=Ogre::Vector3(
      verticesA[vertexStart],
      verticesA[vertexStart+1],
      verticesA[vertexStart+2]
      );
                                         
      std::cout<<orientationA*(vertex+positionA)<<std::endl;
      }
      std::cout<<std::endl;*/

  //Dynamiczna alokacja i wektor spowalniały wszystko. Trzeba niestety "na siłę".
  Ogre::Vector3 a0,a1,a2, b0,b1,b2;

  //dla każdego trójkąta A
  for(int faceIndexA=0;faceIndexA<objectA.IndicesBuffer.size()/3;faceIndexA++)
    {
      //trójkąt A
      int vertexIndex=0;
      for(auto a : {&a0, &a1, &a2})
        {
          *a=transformA*objectA.VerticesBuffer[objectA.IndicesBuffer[3*faceIndexA+vertexIndex]];
          /*orientationA*
            (
             Ogre::Vector3(
                           verticesA[vertexAStart],
                           verticesA[vertexAStart+1],
                           verticesA[vertexAStart+2]
                           )
                           +positionA*/

          vertexIndex++;
        }

      //sprawdzamy każdy trójkąt B
      for(int faceIndexB=0;faceIndexB<objectB.IndicesBuffer.size()/3;faceIndexB++)
        {
          //trójkąt B
          vertexIndex=0;
          for(auto b:{&b0, &b1, &b2})
            {
              *b=transformB*objectB.VerticesBuffer[objectB.IndicesBuffer[3*faceIndexB+vertexIndex]];
              vertexIndex++;
            }

          if(Moller(a0, a1, a2, b0, b1, b2))
            return true;
        }
    }

  return false;
}

bool CollisionDetector::Moller(const Ogre::Vector3& a0, const Ogre::Vector3& a1, const Ogre::Vector3& a2, const Ogre::Vector3& b0, const Ogre::Vector3& b1, const Ogre::Vector3& b2)
{
  //std::cout<<a0<<std::endl<<a1<<std::endl<<a2<<std::endl<<std::endl;
  return false;
}
