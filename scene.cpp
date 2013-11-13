#include "scene.hpp"
#include <OgreSceneNode.h>

Scene::Scene(boost::property_tree::basic_ptree<std::string, std::string>& propertyTree):isCollisionSwitchStillOn(false), isActive(true)
{
  for(auto property : propertyTree) //std::pair<string,basic_ptree>
    if(property.first=="ObjectA")
      ObjectA=Object(property.second.data(),Ogre::Vector3(-1,0,0));
    else if(property.first=="ObjectB")
      ObjectB=Object(property.second.data(),Ogre::Vector3(1,0,0));
    else if(property.first=="DistanceSquared")
      {
        float distance=std::stof(property.second.data());
        DistanceSquared=distance;
      }
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
