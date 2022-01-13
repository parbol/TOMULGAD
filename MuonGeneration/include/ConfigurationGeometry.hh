//--------------------------------------------------------------------------------------//
// _________   ___   ____    ____ _____  _____ _____      ______       _      ______    //
//|  _   _  |.'   `.|_   \  /   _|_   _||_   _|_   _|   .' ___  |     / \    |_   _ `.  //
//|_/ | | \_/  .-.  \ |   \/   |   | |    | |   | |    / .'   \_|    / _ \     | | `. \ //
//    | |   | |   | | | |\  /| |   | '    ' |   | |   _| |   ____   / ___ \    | |  | | //
//   _| |_  \  `-'  /_| |_\/_| |_   \ \__/ /   _| |__/ \ `.___]  |_/ /   \ \_ _| |_.' / //
//  |_____|  `.___.'|_____||_____|   `.__.'   |________|`._____.'|____| |____|______.'  //
//                                                                                      //
//--------------------------------------------------------------------------------------//
// A software project by: Pablo Martinez Ruiz del Árbol                                 //
//--------------------------------------------------------------------------------------//
#ifndef ConfigurationGeometry_h
#define ConfigurationGeometry_h 1


#include <vector>
#include <iostream>
#include <fstream>
#include "globals.hh"

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "assert.h"

#include "Detector.hh"


class ConfigurationGeometry {

public:

    ConfigurationGeometry(G4String);
    bool isGood();
    G4double getSizeX();
    G4double getSizeY();
    G4double getSizeZ();
    G4double getZOffsetCRY();
    G4double getSizeBoxCRY();

    G4double getSizeXLayer();
    G4double getSizeYLayer();
    G4double getSizeZLayer();
    G4double getXOffsetLayer();
    G4double getYOffsetLayer();
    G4double getZOffsetLayer();

    Detector *getDetector1();
    Detector *getDetector2();

    void Print();

private:

    G4double uniSizeX, uniSizeY, uniSizeZ;
    G4double zOffsetCRY, sizeBoxCRY;
    G4double xSizeLayer, ySizeLayer, zSizeLayer, xOffsetLayer, yOffsetLayer, zOffsetLayer;

    Detector *detector1, *detector2;
    bool goodGeometry;

};



#endif

