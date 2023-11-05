#ifndef LGADDigi_h
#define LGADDigi_h 1

#include "LGADSensorHit.hh"

class LGADDigi {

public:

    LGADDigi(LGADSensorHit *);

    G4int GetDet();

    G4int GetLayer();

    G4int GetLGAD();

    G4int GetPadx();

    G4int GetPady();

    G4bool Digitize();

    G4int eventNumber;
    G4int hitID;
    G4double TOA;
    G4double TOT;
    G4double charge;
        
};



#endif

