#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "ConfigurationGeometry.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"

#include <b2g4.h>


class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class DetectorConstMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(ConfigurationGeometry *);
    virtual ~DetectorConstruction();

public:
    virtual G4VPhysicalVolume* Construct();

private:
    void ConstructMaterials();
    void DestroyMaterials();
    void DumpGeometricalTree(G4VPhysicalVolume*, G4int);

private:
    DetectorConstMessenger* messenger;

    ConfigurationGeometry *myConf;

    std::map<G4String, G4Material*> materials;
  
    // The B2G4 scene object
    b2g4::Scene scene;
    
    // The messenger object that stores the custom B2G4 macro command
    std::unique_ptr<G4GenericMessenger> fMessenger;  
    
    // Variable to store the path to the B2G4 scene JSON-file
    G4String b2g4ScenePath;  


};

#endif

