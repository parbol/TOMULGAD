//------------------------------------------------------------//
// |__   __/ __ \|  \/  | |  | | |    / ____|   /\   |  __ \  //
//    | | | |  | | \  / | |  | | |   | |  __   /  \  | |  | | //
//    | | | |  | | |\/| | |  | | |   | | |_ | / /\ \ | |  | | //
//    | | | |__| | |  | | |__| | |___| |__| |/ ____ \| |__| | //
//    |_|  \____/|_|  |_|\____/|______\_____/_/    \_\_____/  //
//------------------------------------------------------------//
// ConfigurationGeometry class:                               //                                                           
//                                                            //
// Parses json files with the configuration of the detectors. //
//                                                            //
//------------------------------------------------------------//

#ifndef ConfigurationGeometry_h
#define ConfigurationGeometry_h 1

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "assert.h"

#include "globals.hh"
#include "Phantom.hh"
#include "Detector.hh"
#include "Layer.hh"
#include "LGAD.hh"


class ConfigurationGeometry {

public:

    ConfigurationGeometry(G4String);
    
    bool isGood();

    // Information about the GEANT4 universe 
    G4double getSizeX();
    G4double getSizeY();
    G4double getSizeZ();
   
    // Detector information
    Detector *getDetector(G4int);
    G4int getNDetectors();
    
    G4double getZCeiling();
    G4double getSkySize();
    G4double getThetaMin();
    G4double getThetaMax();
    G4double getSkyX();
    G4double getSkyY();
    G4double getSkyZ();
    
    // Detector information
    Phantom *getPhantom(G4int);
    G4int getNPhantoms();

    //Creating the geometry
    void createG4objects(G4LogicalVolume *, 
                         std::map<G4String, G4Material*> &,
                         G4SDManager *);
    // Detector collections
    void registerCollection(G4String);
    std::vector <G4String> collections;
    
    void Print();

private:

   

    G4double uniSizeX, uniSizeY, uniSizeZ;
    G4double zCeiling, SkySize, thetaMin, thetaMax, SkyX, SkyY, SkyZ;
    std::vector <Detector *> detectors;
    std::vector <Phantom *> phantoms;
    bool goodGeometry;
    LGADSignalShape *signalShape;
};



#endif

