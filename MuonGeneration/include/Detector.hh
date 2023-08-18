#ifndef Detector_h
#define Detector_h 1

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ErrorMatrix.hh"

#include "GeomObject.hh"
#include "Layer.hh"


class Detector : GeomObject {

public:

    Detector(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    
    void AddLayer(Layer *);
    
    Layer *GetLayer(G4int);
    
    G4int GetNLayers();
    
    void createG4Objects(G4String, G4LogicalVolume *);

    void Print();

private:
    std::vector<Layer *> layers;

};



#endif

