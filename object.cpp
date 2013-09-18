#include "object.hpp"
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

Object::Object(const std::string& meshFile, const Ogre::Vector3& position)
{
  Ogre::SceneManager* sceneManager=Ogre::Root::getSingleton().getSceneManagerIterator().current()->second;
  Ogre::Entity* entity=sceneManager->createEntity(meshFile);
  Node=sceneManager->getRootSceneNode()->createChildSceneNode();
  Node->setPosition(position);
  Node->attachObject(entity);
  //Node->showBoundingBox(true);
}
