#include "collisiondetector.hpp"
#include <OgreAxisAlignedBox.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

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
  //Dobieram się do trójkątow obu obiektów.

  //VertexData
  Ogre::VertexData* vertexDataA;
  
  if(objectA.Mesh->getSubMesh(0)->useSharedVertices)
    vertexDataA=objectA.Mesh->sharedVertexData;
  else
    vertexDataA=objectA.Mesh->getSubMesh(0)->vertexData;

  Ogre::VertexData* vertexDataB;
  if(objectB.Mesh->getSubMesh(0)->useSharedVertices)
    vertexDataB=objectB.Mesh->sharedVertexData;
  else
    vertexDataB=objectB.Mesh->getSubMesh(0)->vertexData;

  //IndexData
  Ogre::IndexData* indexDataA=objectA.Mesh->getSubMesh(0)->indexData;
  Ogre::IndexData* indexDataB=objectB.Mesh->getSubMesh(0)->indexData;

  //VertexBuffer
  Ogre::HardwareVertexBufferSharedPtr vertexBufferA = vertexDataA->vertexBufferBinding->getBuffer(vertexDataA->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource());
  Ogre::HardwareVertexBufferSharedPtr vertexBufferB = vertexDataB->vertexBufferBinding->getBuffer(vertexDataB->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource());

  //Offset
  int positionElementOffsetInVertexA=vertexDataA->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getOffset();
  int positionElementOffsetInVertexB=vertexDataB->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getOffset();

  //VertexSize
  int vertexSizeInBufferA=vertexDataA->vertexDeclaration->getVertexSize(vertexDataA->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource())/sizeof(float);
  int vertexSizeInBufferB=vertexDataB->vertexDeclaration->getVertexSize(vertexDataB->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource())/sizeof(float);
  
  //IndexBuffer
  Ogre::HardwareIndexBufferSharedPtr indexBufferA=indexDataA->indexBuffer;
  Ogre::HardwareIndexBufferSharedPtr indexBufferB=indexDataB->indexBuffer;

  //Lock buforów
  float* verticesA=(float*)vertexBufferA->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
  unsigned short* indicesA=(unsigned short*)indexBufferA->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);

  float* verticesB=(float*)vertexBufferB->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
  unsigned short* indicesB=(unsigned short*)indexBufferB->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);


  //TEST
  int faceIndex=0;
  for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
    {
      for(int coordIndex=0;coordIndex<3;coordIndex++)
        {
          std::cout<<verticesA[positionElementOffsetInVertexA+vertexSizeInBufferA*indicesA[3*faceIndex+vertexIndex]+coordIndex]<<" ";
        }
      std::cout<<std::endl;
    }
  std::cout<<std::endl;
  
  //TroppTalShimshoni
  //Stage1
  //Stage2
  //Stage3

  //Unlock buforów
  vertexBufferA->unlock();
  indexBufferA->unlock();
  //Podwójny unlock powoduje błędy
  if(vertexBufferA!=vertexBufferB)
    vertexBufferB->unlock();
  if(indexBufferA!=indexBufferB)
    indexBufferB->unlock();

  return false;  
}
