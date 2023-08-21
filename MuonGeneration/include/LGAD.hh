#ifndef LGAD_h
#define LGAD_h 1

#include "GeomObject.hh"
#include "LGADSensor.hh"


class LGAD : GeomObject {

public:

    LGAD(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    
    void AddSensor(LGAD *);
    
    //LGADSensor *GetSensor();

    void createG4Objects(G4String, G4LogicalVolume *, 
                         std::map<G4String, G4Material*> &,
                         G4SDManager *);
    
    void Print();

private:
    LGADSensor * sensor;

};



#endif

