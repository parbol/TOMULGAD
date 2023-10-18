#include "LGADDigi.hh"

//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
LGADDigi::LGADDigi(LGADSensorHit *h, ConfigurationGeometry *g) {
    hit = h;
    geom = g;
    
    G4int det = hit->GetDetectorID();
    G4int layer = hit->GetLayerID();
    G4int lgad = hit->GetLGADId();
    G4int padx = hit->GetPadx();
    G4int pady = hit->GetPady();
};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

