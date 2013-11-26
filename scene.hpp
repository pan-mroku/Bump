#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include <QtCore/QObject>

/** Klasa dla poszczególnych scen.
   @remarks
   Buduje się według zapisów z plików ini. Przechowuje wszystko, co związane ze sceną: obiekty, odległość między obiektami, dla której następuje kolizja.
   @note
   Być może wartoby każdej scenie przypisać też osobny Ogre::SceneManager.
*/
class Scene:public QObject
{

Q_OBJECT

signals:
  /** Sygnał informujacy o zmianie stanu kolizji.
      @remarks
      Informowanie o zmianie stanu na podany w parametrze pozwala zaoszczędzić emitowane sygnały, wysyłajac go tylko, gdy potrzeba. Wymaga to dodatkowego pola (bool isCollisionSwitchStillOn)
      @param SceneCollisionState Podaje slotom nowy stan kolizji zapisanej w scenie
  */
  void ObjectsCollisionInSceneChanged(bool SceneCollisionState);

protected:
  bool isCollisionSwitchStillOn;
  bool isActive;

public:
  /* Ogre::Vector3::squaredDistance
     This method is for efficiency - calculating the actual distance to another vector requires a square root, which is expensive in terms of the operations required. This method returns the square of the distance to another vector, i.e. the same as the distance but before the square root is taken. Use this if you want to find the longest / shortest distance without incurring the square root.
  */
  float DistanceSquared; 
  Object ObjectA, ObjectB;

  /** Konstruktor.
      @param propertyTree boost::property_tree::ini_parser odczytuje z pliku ini drzewa. To jest referencja do poddrzewa właściwości dotyczącego tylko danej sceny.
  */
  Scene(boost::property_tree::basic_ptree<std::string, std::string>& propertyTree);

  /** Sprawdza, czy następuje kolizja według zapisu w scenie.
      @remarks
      Jeśli potrzeba, samo wysyła sygnał Scene::ObjectsCollisionInSceneChanged.
  */
  void CheckSceneCollision();

  void SetInactive();
  void SetActive();

  const bool IsActive() const;
};

#endif
