#ifndef LGAD_h
#define LGAD_h 1

#include "GeomObject.hh"

class LGAD : GeomObject {

public:

    LGAD(G4double, G4double, G4double, 
         G4double, G4double, G4double, 
         G4double, G4double, G4double,
         G4int, G4int, G4double, G4double,
         G4double, G4double, G4double,
         G4double, G4double, G4int, G4int, G4int);

    G4int detId();

    G4int layerId();

    G4int sensorId();

    void createG4Objects(G4String, G4LogicalVolume *, 
                         std::map<G4String, G4Material*> &,
                         G4SDManager *);
    
    void Print();

private:
    G4int ndetId, nlayerId, nsensorId;
    G4int nPadx, nPady;
    G4double interPadx, interPady, borderPadx, borderPady;
    G4double chargeThreshold, noiseLevel, tdcSigma;
    

};



#endif

