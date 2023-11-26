#ifndef LGADSignalShape_h
#define LGADSignalShape_h 1

#include "globals.hh"

class LGADSignalShape {

public:

    LGADSignalShape();
    std::pair<double, double> getTimes(double, double);

private:

    const static double signalShape[];  
    G4int maxN;
};

#endif