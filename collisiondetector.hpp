#ifndef COLLISIONDETECTOR_HPP
#define COLLISIONDETECTOR_HPP

#include "object.hpp"
#include <QtCore/QObject>

/** Klasa zawierająca algorytmy detekcji kolizji.
    @remarks
    Rozwiązanie mało profesjonalne, ale ostatecznie chodzi głównie o porównywanie między sobą różnych algorytmów. Gdyby się uprzeć możnaby je nawet jako statyczne funkcje zdefiniować, więc nie ma co niewiadomo czego wymyślać.
*/
class CollisionDetector:public QObject
{

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

  /** Enumerator nazywający algorytmy, żeby można było w miarę łąto z zewnątrz je przełączać, przy jednoczesnym zachowaniu pewności co jest wywoływane.*/
  typedef enum
    {
      None,
      BoundingBox,
      Triangle,
      Complex
    } CollisionAlgorithmEnum;
  
  CollisionAlgorithmEnum ActiveAlgorithm;

  CollisionDetector();

  /** Sprawdza, czy według aktywnego algorytmu następuje kolizja miedzy obiektami.
      @remarks
      Jeśli potrzeba, samo wysyła sygnał Scene::ObjectsCollisionInSceneChanged.
      @param objectA, objectB Referencje do obiektów, miedzy którymi bada kolizję.
  */
  void CheckCollision(const Object& objectA, const Object& objectB);

  /** Algorytm kolizji, który kompletnie nic nie robi. */
  bool NoneCollisionAlgorithm(const Object& objectA, const Object& objectB);

  /** Algorytm kolizji oparty na BoundingBox. */
  bool BoundingBoxCollisionAlgorithm(const Object& objectA, const Object& objectB);

  /** Algorytm kolizji oparty na trójkątach. */
  bool TriangleCollisionAlgorithm(const Object& objectA, const Object& objectB);

  /** Algorytm kolizji łączący BB i trójkąty. */
  bool ComplexCollisionAlgorithm(const Object& objectA, const Object& objectB);
};

#endif
