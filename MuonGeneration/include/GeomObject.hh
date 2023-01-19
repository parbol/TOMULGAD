#ifndef GeomObject_h
#define GeomObject_h 1

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ErrorMatrix.hh"



class GeomObject {

public:

    GeomObject(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    G4RotationMatrix *getRot();
    G4ThreeVector     getPos();
    G4ThreeVector     getSizes();
    G4ThreeVector     toGlobal(G4ThreeVector);
    G4ThreeVector     toLocal(G4ThreeVector);
    G4ErrorMatrix     toGlobalStateVector(G4ErrorMatrix);
    G4ErrorMatrix     toGlobalStateCov(G4ErrorMatrix);
    G4VPhysicalVolume *getVolume();
    void              setVolume(G4VPhysicalVolume *);
    void              Print();

    G4RotationMatrix rot, invrot;
    G4ThreeVector pos;
    G4ThreeVector sizes;
    G4ThreeVector rots;
    G4ErrorMatrix *superMatrix;
    G4ErrorMatrix *superVector;
    G4VPhysicalVolume * volume;

private:

};



#endif

