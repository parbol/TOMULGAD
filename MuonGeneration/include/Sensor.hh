#ifndef Sensor_h
#define Sensor_h 1

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ErrorMatrix.hh"

#include "GeomObject.hh"


class Sensor : GeomObject {

public:

    Sensor(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double,
           G4int, G4int, G4double, G4double, G4double, G4double, G4double, G4double, G4double);

    void Print();

private:

    G4double chargeThreshold;
    G4double noise;
    G4double tdcSigma;
    G4int nPadx;
    G4int nPady;
    G4double interPadx;
    G4double interPady;
    G4double xborder;
    G4double yborder;

    std::vector<Sensor *> sensors;

};

#endif

