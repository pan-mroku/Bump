#include "object.hpp"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include <OgreEdgeListBuilder.h>
#include <iostream>

Object::Object(const std::string& meshFile, const Ogre::Vector3& position)
{
  Ogre::SceneManager* sceneManager=Ogre::Root::getSingleton().getSceneManagerIterator().current()->second;
  Ogre::Entity* entity=sceneManager->createEntity(meshFile);
  Node=sceneManager->getRootSceneNode()->createChildSceneNode();
  Node->setPosition(position);
  Node->attachObject(entity);

  Node->showBoundingBox(true);

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


          //zrzut trójkątów
          for(int faceIndex=0;faceIndex<indexBuffer->getNumIndexes()/3;faceIndex++)
            {
              for(int vertexIndex=0;vertexIndex<3;vertexIndex++)
                {
                  for(int coordIndex=0;coordIndex<3;coordIndex++)
                    std::cout<<vertices[positionElementOffsetInVertex+vertexSizeInBuffer*indices[3*faceIndex+vertexIndex]+coordIndex]<<" ";
                  std::cout<<std::endl;
                }
              std::cout<<std::endl;
            }
            
          //Zrzut tabel
          /*          std::cout<<"v: ";
          for(int i=0;i<vertexBuffer->getNumVertices()*vertexSizeInBuffer;i++)
            std::cout<<vertices[i]<<" ";
          std::cout<<std::endl;
          std::cout<<"i: ";
          for(int i=0;i<indexBuffer->getNumIndexes();i++)
            std::cout<<indices[i]<<" ";
            std::cout<<std::endl;*/

          vertexBuffer->unlock();
          indexBuffer->unlock();
        }
    }
}
