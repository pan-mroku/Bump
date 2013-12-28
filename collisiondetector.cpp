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

  //Position
  Ogre::Vector3 positionA=objectA.Node->_getDerivedPosition();
  Ogre::Vector3 positionB=objectB.Node->_getDerivedPosition();

  //Orientation
  Ogre::Quaternion orientationA=objectA.Node->_getDerivedOrientation();
  Ogre::Quaternion orientationB=objectB.Node->_getDerivedOrientation();

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

  //Zamiast pisania klasy Triangle
  Ogre::Vector3* triangleA=new Ogre::Vector3[3];
  Ogre::Vector3* triangleB=new Ogre::Vector3[3];
  //Będziemy przechowywać wynik kolizji, żeby nie kombinować z porządkowaniem pamięci
  bool collision=false;
  
  //dla każdego trójkąta A
  for(int faceIndexA=0;faceIndexA<indexBufferA->getNumIndexes()/3;faceIndexA++)
    {
      //Pętle są dwie, więc możliwe, że już nastąpiła kolizja.
      if(collision)
        break;

      //trójkąt A
      /*for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
        {
          int vertexAStart = positionElementOffsetInVertexA+vertexSizeInBufferA*indicesA[3*faceIndexA+vertexIndex];
          
          triangleA[vertexIndex]=orientationA*
            (
             Ogre::Vector3(
                           verticesA[vertexAStart],
                           verticesA[vertexAStart+1],
                           verticesA[vertexAStart+2]
                           )
             +positionA
             );
             }*/

      //sprawdzamy każdy trójkąt B
      for(int faceIndexB=0;faceIndexB<indexBufferB->getNumIndexes()/3;faceIndexB++)
        {
          //trójkąt B
          for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
            {
              int vertexBStart = positionElementOffsetInVertexB+vertexSizeInBufferB*indicesB[3*faceIndexB+vertexIndex];
              
              Ogre::Vector3 tmpVertex=Ogre::Vector3(
                                                    verticesB[vertexBStart],
                                                    verticesB[vertexBStart+1],
                                                    verticesB[vertexBStart+2]
                                                    );
              //triangleB[vertexIndex]=tmpVertex; //@FIX źle. zapis do pamięci strasznie muli. Niech TroppTalShimshoni przyjmuje inne parametry
            }

          if(TroppTalShimshoni(triangleA, triangleB))
            {
              collision=true;
              break;
            }
          if(TroppTalShimshoni(triangleB, triangleA))
            {
              collision=true;
              break;
            }
        }
    }

  //Porządki
  delete[] triangleA;  
  delete[] triangleB;
  //Unlock buforów
  vertexBufferA->unlock();
  indexBufferA->unlock();
  //Podwójny unlock powoduje błędy
  if(vertexBufferA!=vertexBufferB)
    vertexBufferB->unlock();
  if(indexBufferA!=indexBufferB)
    indexBufferB->unlock();

  return collision;  
}

bool CollisionDetector::TroppTalShimshoni(const Ogre::Vector3* triangleP, const Ogre::Vector3* triangleQ)
{
  return false;
}
