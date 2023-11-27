#ifndef LGADSignalShape_h
#define LGADSignalShape_h 1

#include "globals.hh"

class LGADSignalShape {

public:

    LGADSignalShape(G4double);
    std::pair<G4double, G4double> getTimes(G4double);

private:

    const static G4double signalShape[];  
    G4int maxN;
    G4double threshold;

};

#endif