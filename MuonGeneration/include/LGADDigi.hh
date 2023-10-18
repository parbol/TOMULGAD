#ifndef LGADDigi_h
#define LGADDigi_h 1

#include "LGADSensorHit.hh"
#include "ConfigurationGeometry.hh"

class LGADDigi {

public:

    LGADDigi(LGADSensorHit *, ConfigurationGeometry *);

   
private:
    LGADSensorHit *hit;
    ConfigurationGeometry *geom;
    G4String name;

};



#endif

