#include "object.hpp"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include <OgreEdgeListBuilder.h>
#include <iostream>

Object::Object(const std::string& meshFile, const Ogre::Vector3& position, bool left)
{
  Ogre::SceneManager* sceneManager=Ogre::Root::getSingleton().getSceneManagerIterator().current()->second;
  Entity=sceneManager->createEntity(meshFile);
  Node=sceneManager->getRootSceneNode()->createChildSceneNode();

  Node->setPosition(position);

  if(left)
    moveVector=Ogre::Vector3(0.02,0,0);
  else
    moveVector=Ogre::Vector3(-0.02,0,0);

  Node->attachObject(Entity);

  Node->showBoundingBox(true);

  Mesh=Entity->getMesh().getPointer();
  quat=Ogre::Quaternion(1,0.1,0,0);

  UpdateBuffers();
}

void Object::PrintFaceVertexCoords() const
{
  //http://ogre3d.org/forums/viewtopic.php?f=2&t=65573
  //
  for (auto o : Node->getAttachedObjectIterator())
    {
      Ogre::Entity* e=dynamic_cast<Ogre::Entity*>(o.second);

      if (e==NULL)
        continue;

      Ogre::Mesh* mesh=e->getMesh().getPointer();

      for (Ogre::SubMesh* subMesh : mesh->getSubMeshIterator())
        {
          Ogre::VertexData* vertexData;

          if (subMesh->useSharedVertices)
            vertexData=mesh->sharedVertexData;
          else
            vertexData=subMesh->vertexData;

          Ogre::HardwareVertexBufferSharedPtr vertexBuffer=vertexData->vertexBufferBinding->getBuffer(vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource());
          
          int positionElementOffsetInVertex=vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getOffset();
          int vertexSizeInBuffer=vertexData->vertexDeclaration->getVertexSize(vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource())/sizeof(float);

          Ogre::IndexData* indexData=subMesh->indexData;
          Ogre::HardwareIndexBufferSharedPtr indexBuffer=indexData->indexBuffer;

          float* vertices=(float*)vertexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
          unsigned short* indices=(unsigned short*)indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);


          //Zrzut tabel
          std::cout<<"v: ";
          for(int i=0;i<vertexBuffer->getNumVertices();i++)
            {
              for(int j=0;j<vertexSizeInBuffer;j++)
                std::cout<<vertices[vertexSizeInBuffer*i+j]<<" ";
              std::cout<<std::endl;
            }
          std::cout<<std::endl;
          std::cout<<"i: ";
          for(int i=0;i<indexBuffer->getNumIndexes();i++)
            std::cout<<indices[i]<<" ";
          std::cout<<std::endl;

          //zrzut trójkątów
          for(int faceIndex=0;faceIndex<indexBuffer->getNumIndexes()/3;faceIndex++)
            {
              for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
                {
                  for(int coordIndex=0;coordIndex<3;coordIndex++)
                    {
                      std::cout<<vertices[positionElementOffsetInVertex+vertexSizeInBuffer*indices[3*faceIndex+vertexIndex]+coordIndex]<<" ";
                    }
                  std::cout<<std::endl;
                }
              std::cout<<std::endl;
            }
            


          vertexBuffer->unlock();
          indexBuffer->unlock();
        }
    }
}

void Object::PrintFaceVertexCoordsCivilised() const
{
  Mesh->buildEdgeList();
  
  Ogre::EdgeData* edgeList=Mesh->getEdgeList();
  Ogre::EdgeData::EdgeGroup* edgeGroup=&(edgeList->edgeGroups[0]);
  const Ogre::VertexData* vd=edgeGroup->vertexData;
  
  for(int i=0;i<2;i++)
    {
      Ogre::EdgeData::Triangle* triangle=&(edgeList->triangles[i]);
      std::cout<<triangle->indexSet<<" "<<triangle->sharedVertIndex[0]<<" "<<triangle->sharedVertIndex[1]<<" "<<triangle->sharedVertIndex[2]<<" "<<triangle->vertexSet<<" "<<triangle->vertIndex[0]<<" "<<triangle->vertIndex[1]<<" "<<triangle->vertIndex[2]<<" "<<std::endl;
    }
  //std::cout<<edgeGroup->edges.size()<<std::endl;
}

void Object::Move(unsigned long delta)
{
  Node->translate(moveVector*delta);

  //What shall we do with rotating sailor?
  //Node->rotate(quat*delta);
}

void Object::FlipMoveVector()
{
  moveVector*=-1;
}

const Ogre::AxisAlignedBox& Object::GetBoundingBox() const
{
  return Entity->getWorldBoundingBox();
}

void Object::UpdateBuffers()
{
  //W moich modelach nie ma submeshy, ale niech będzie porządnie.
  for (Ogre::SubMesh* subMesh : Mesh->getSubMeshIterator())
    {
      Ogre::VertexData* vertexData;

      if (subMesh->useSharedVertices)
        vertexData=Mesh->sharedVertexData;
      else
        vertexData=subMesh->vertexData;

      Ogre::HardwareVertexBufferSharedPtr vertexBuffer=vertexData->vertexBufferBinding->getBuffer(vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource());
          
      int positionElementOffsetInVertex=vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getOffset();
      int vertexSizeInBuffer=vertexData->vertexDeclaration->getVertexSize(vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION)->getSource())/sizeof(float);

      Ogre::IndexData* indexData=subMesh->indexData;
      Ogre::HardwareIndexBufferSharedPtr indexBuffer=indexData->indexBuffer;

      float* vertices=(float*)vertexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
      unsigned short* indices=(unsigned short*)indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);


      VerticesBuffer.clear();
      for(int vertIndex=0;vertIndex<vertexBuffer->getNumVertices();vertIndex++)
        {
          VerticesBuffer.push_back(Ogre::Vector3(
                                                 vertices[positionElementOffsetInVertex+vertexSizeInBuffer*vertIndex],
vertices[positionElementOffsetInVertex+vertexSizeInBuffer*vertIndex+1],
vertices[positionElementOffsetInVertex+vertexSizeInBuffer*vertIndex+2]
                                                 ));
        }

      IndicesBuffer.clear();
      for(int indIndex=0;indIndex<indexBuffer->getNumIndexes();indIndex++)
        {
          IndicesBuffer.push_back(indices[indIndex]);
        }

      vertexBuffer->unlock();
      indexBuffer->unlock();
    }

  //DEBUG
  /*
  if(Mesh->getName()=="Cube.mesh")
    {
      for(auto v : VerticesBuffer)
        std::cout<<v<<" ";
      
      std::cout<<std::endl<<std::endl;
      
      for(auto i : IndicesBuffer)
        std::cout<<i<<" ";
      
      std::cout<<std::endl<<std::endl;
      
      for(int index=0;index<IndicesBuffer.size()/3;index++)
        {
          for(int i=0;i<3;i++)
            std::cout<<VerticesBuffer[IndicesBuffer[3*index+i]]<<" ";
          std::cout<<std::endl;
        }
      std::cout<<std::endl<<std::endl<<std::endl;
    }
  */
}

