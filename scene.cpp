#include "scene.hpp"
#include <OgreSceneNode.h>
#include <cstdlib> //sscanf http://stackoverflow.com/questions/14755484/stoi-and-stdto-string-on-mingw-4-7-1

Scene::Scene(boost::property_tree::basic_ptree<std::string, std::string>& propertyTree):isCollisionSwitchStillOn(false), isActive(true)
{
  for(auto property : propertyTree) //std::pair<string,basic_ptree>
    if(property.first=="ObjectA")
      ObjectA=Object(property.second.data(),Ogre::Vector3(-2,0,0));
    else if(property.first=="ObjectB")
      ObjectB=Object(property.second.data(),Ogre::Vector3(2,0,0),false);
    else if(property.first=="DistanceSquared")
      sscanf(property.second.data().c_str(), "%f", &DistanceSquared);
}

void Scene::CheckSceneCollision()
{
  bool collision;
  Ogre::Vector3 positionA=ObjectA.Node->_getDerivedPosition();
  Ogre::Vector3 positionB=ObjectB.Node->_getDerivedPosition();

  if(positionA.squaredDistance(positionB)-DistanceSquared<0.1)
    collision=true;
  else
    collision=false;

  if(collision!=isCollisionSwitchStillOn)
    {
      isCollisionSwitchStillOn=collision;
      emit(ObjectsCollisionInSceneChanged(collision));
    }
    
}

void Scene::SetInactive()
{
  ObjectA.Node->setVisible(false);
  ObjectA.Node->showBoundingBox(false);
  ObjectB.Node->setVisible(false);
  ObjectB.Node->showBoundingBox(false);
  isActive=false;
}

void Scene::SetActive()
{
  ObjectA.Node->setVisible(true);
  ObjectA.Node->showBoundingBox(true);
  ObjectB.Node->setVisible(true);
  ObjectB.Node->showBoundingBox(true);
  isActive=true;
}

const bool Scene::IsActive() const
{
  return isActive;
}

void Scene::Tick(unsigned long delta)
{
  ObjectA.Move(delta);
  ObjectB.Move(delta);

  Ogre::Vector3 positionA=ObjectA.Node->_getDerivedPosition();

  if(positionA.x>0) 
    {
      ObjectA.Node->setPosition(0,0,0);
      ObjectB.Node->setPosition(0,0,0);
      ObjectA.FlipMoveVector();
      ObjectB.FlipMoveVector();
    }
  if(positionA.x<-2)
    {
      ObjectA.Node->setPosition(-2,0,0);
      ObjectB.Node->setPosition(2,0,0);
      ObjectA.FlipMoveVector();
      ObjectB.FlipMoveVector();
    }
}
