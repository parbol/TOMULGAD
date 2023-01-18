#ifndef Layer_h
#define Layer_h 1

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ErrorMatrix.hh"

#include "GeomObject.hh"
#include "Sensor.hh"


class Layer : GeomObject {

public:

    Layer(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    
    void AddSensor(Sensor *);
    
    Layer *GetSensor(G4int);
    
    G4int GetNSensors();
    
    void Print();

private:
    std::vector<Sensor *> sensors;
};



#endif

