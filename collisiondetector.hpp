#ifndef COLLISIONDETECTOR_HPP
#define COLLISIONDETECTOR_HPP

#include "object.hpp"
#include <QtCore/QObject>

class CollisionDetector:public QObject
{

  typedef enum
    {
      None,
      BoundingBox,
      Triangle,
      Complex
    } CollisionAlgorithmEnum;

Q_OBJECT

signals:
  /** Sygnał informujacy o zmianie stanu kolizji.
      @remarks
      Informowanie o zmianie stanu na podany w parametrze pozwala zaoszczędzić emitowane sygnały, wysyłajac go tylko, gdy potrzeba. Wymaga to dodatkowego pola (bool isCollisionSwitchStillOn)
      @param AlgorithmCollisionState Podaje slotom nowy stan kolizji zapisanej w scenie
  */
  void ObjectsCollisionInAlgorithmChanged(bool AlgorithmCollisionState);

protected:
  bool isCollisionSwitchStillOn;


public:
  CollisionAlgorithmEnum ActiveAlgorithm;

  CollisionDetector();

  /** Sprawdza, czy według aktywnego algorytmu następuje kolizja miedzy obiektami.
      @remarks
      Jeśli potrzeba, samo wysyła sygnał Scene::ObjectsCollisionInSceneChanged.
      @param Referencje do obiektów, miedzy którymi bada kolizję.
  */
  void CheckCollision(const Object& objectA, const Object& objectB);

  /** Algorytm kolizji, który kompletnie nic nie robi. */
  bool NoneCollisionAlgorithm(const Object& objectA, const Object& objectB);
};

#endif
