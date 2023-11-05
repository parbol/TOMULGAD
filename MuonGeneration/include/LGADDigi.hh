#ifndef LGADDigi_h
#define LGADDigi_h 1

#include "LGADSensorHit.hh"
#include "ConfigurationGeometry.hh"

class LGADDigi {

public:

    LGADDigi(LGADSensorHit *, ConfigurationGeometry *);

   
private:
  
    G4int hitID;
    G4float TOA;
    G4float TOT;
    G4float charge;
        
};



#endif

