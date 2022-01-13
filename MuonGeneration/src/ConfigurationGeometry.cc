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

#include "ConfigurationGeometry.hh"
#include <json/json.h>
#include <json/value.h>
//#include "jsoncpp.cpp"



//--------------------------------------------------------------------------------------//
// Constructor                                                                          //
//--------------------------------------------------------------------------------------//
ConfigurationGeometry::ConfigurationGeometry(G4String file) {

    xSizeLayer = 0;
    ySizeLayer = 0;
    zSizeLayer = 0;
    xOffsetLayer = 0;
    yOffsetLayer = 0;
    zOffsetLayer = 0;

    Json::Value root;
    Json::Reader reader;

    //We open the JSON file, read it and put it on a string
    std::stringstream  filecontent;
    std::ifstream infile(file.c_str());

    if(!(infile.good())) {
        G4cerr << "\033[1;31m" << "Error opening geometry file: " + file << "\033[0m" << G4endl;
        goodGeometry = false;
        return;
    }

    std::string currline;
    while(getline(infile,currline)) {
        filecontent<<currline;
    }

    infile.close();

    std::string FileContent=filecontent.str();

    //Parsing of the JSON file
    bool parsingSuccessful = reader.parse( FileContent, root );
    if ( !parsingSuccessful ) {
        G4cerr << "\033[1;31m" << "Error parsing file: " + file << "\033[0m" << G4endl;
        goodGeometry = false;
        return;
    }

    if( root.size() > 0 ) {

        G4double xSize = atof(root["TheWorld"]["xSizeWorld"].asString().c_str());
        G4double ySize = atof(root["TheWorld"]["ySizeWorld"].asString().c_str());
        G4double zSize = atof(root["TheWorld"]["zSizeWorld"].asString().c_str());
        G4double zOffsetCRY_ = atof(root["TheWorld"]["zOffsetCRY"].asString().c_str());
        G4double sizeBoxCRY_ = atof(root["TheWorld"]["sizeBoxCRY"].asString().c_str());

        if(xSize <= 0 || ySize <= 0|| zSize <= 0) {
            G4cerr << "\033[1;31m" << "The size of the Universe has been greater than 0" << "\033[0m" << G4endl;
            goodGeometry = false;
            return;
        }

        if(zOffsetCRY_ < 0) {
            G4cerr << "\033[1;31m" << "Cry should be producing muons above the surface" << "\033[0m" << G4endl;
            goodGeometry = false;
            return;
        }

        if(sizeBoxCRY_ <= 0) {
            G4cerr << "\033[1;31m" << "Cry should have a positive size of production" << "\033[0m" << G4endl;
            goodGeometry = false;
            return;
        }

        uniSizeX = xSize * CLHEP::cm;
        uniSizeY = ySize * CLHEP::cm;
        uniSizeZ = zSize * CLHEP::cm;
        zOffsetCRY = zOffsetCRY_ * CLHEP::cm;
        sizeBoxCRY = sizeBoxCRY_ * CLHEP::cm;

        G4double xPos1 = atof(root["Detector1"]["xPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double yPos1 = atof(root["Detector1"]["yPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double zPos1 = atof(root["Detector1"]["zPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double xDir1 = atof(root["Detector1"]["xDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double yDir1 = atof(root["Detector1"]["yDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double zDir1 = atof(root["Detector1"]["zDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double xSize1 = atof(root["Detector1"]["xSizeDetector"].asString().c_str()) * CLHEP::cm;
        G4double ySize1 = atof(root["Detector1"]["ySizeDetector"].asString().c_str()) * CLHEP::cm;
        G4double zSize1 = atof(root["Detector1"]["zSizeDetector"].asString().c_str()) * CLHEP::cm;

        detector1 = new Detector(xPos1, yPos1, zPos1, xDir1, yDir1, zDir1, xSize1, ySize1, zSize1);

        const Json::Value Layer1 = root["Detector1"]["layers"];
        for(G4int icoll = 0; icoll < Layer1.size(); ++icoll) {
            G4double xPosLayer = atof(root["Detector1"]["layers"][icoll]["xPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double yPosLayer = atof(root["Detector1"]["layers"][icoll]["yPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double zPosLayer = atof(root["Detector1"]["layers"][icoll]["zPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double xDirLayer = atof(root["Detector1"]["layers"][icoll]["xDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double yDirLayer = atof(root["Detector1"]["layers"][icoll]["yDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double zDirLayer = atof(root["Detector1"]["layers"][icoll]["zDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double xSizeLayer_ = atof(root["Detector1"]["layers"][icoll]["xSizeLayer"].asString().c_str()) * CLHEP::cm;
            G4double ySizeLayer_ = atof(root["Detector1"]["layers"][icoll]["ySizeLayer"].asString().c_str()) * CLHEP::cm;
            G4double zSizeLayer_ = atof(root["Detector1"]["layers"][icoll]["zSizeLayer"].asString().c_str()) * CLHEP::cm;
            G4int npixelX = atoi(root["Detector1"]["layers"][icoll]["npixelX"].asString().c_str())
            G4int npixelY = atoi(root["Detector1"]["layers"][icoll]["npixelY"].asString().c_str())
            G4double borderSizeX = atof(root["Detector1"]["layers"][icoll]["borderSizeX"].asString().c_str()) * CLHEP::cm;
            G4double borderSizeY = atof(root["Detector1"]["layers"][icoll]["borderSizeY"].asString().c_str()) * CLHEP::cm;
            G4double pixelSizeX = atof(root["Detector1"]["layers"][icoll]["pixelSizeX"].asString().c_str()) * CLHEP::cm;
            G4double pixelSizeY = atof(root["Detector1"]["layers"][icoll]["pixelSizeY"].asString().c_str()) * CLHEP::cm;
            G4double efficiency = atof(root["Detector1"]["layers"][icoll]["efficiency"].asString().c_str()) * CLHEP::cm;
            G4double tResolution = atof(root["Detector1"]["layers"][icoll]["tResolution"].asString().c_str()) * CLHEP::ps;
            if(efficiency < 0 || efficiency > 1) {
                G4cerr << "\033[1;31m" << "Chamber 1 efficiency is not in the range [0, 1]" << "\033[0m" << G4endl;
                goodGeometry = false;
                return;
            }
            detector1->AddLayer(xPosLayer, yPosLayer, zPosLayer, xDirLayer, yDirLayer, zDirLayer, xSizeLayer_, ySizeLayer_, zSizeLayer_,
                                npixelX, npixelY, borderSizeX, borderSizeY, pixelSizeX, pixelSizeY, efficiency, tResolution);
        }


        G4double xPos2 = atof(root["Detector2"]["xPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double yPos2 = atof(root["Detector2"]["yPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double zPos2 = atof(root["Detector2"]["zPosDetector"].asString().c_str()) * CLHEP::cm;
        G4double xDir2 = atof(root["Detector2"]["xDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double yDir2 = atof(root["Detector2"]["yDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double zDir2 = atof(root["Detector2"]["zDirDetector"].asString().c_str()) * CLHEP::degree;
        G4double xSize2 = atof(root["Detector2"]["xSizeDetector"].asString().c_str()) * CLHEP::cm;
        G4double ySize2 = atof(root["Detector2"]["ySizeDetector"].asString().c_str()) * CLHEP::cm;
        G4double zSize2 = atof(root["Detector2"]["zSizeDetector"].asString().c_str()) * CLHEP::cm;
        
        detector2 = new Detector(xPos2, yPos2, zPos2, xDir2, yDir2, zDir2, xSize2, ySize2, zSize2);
        
        const Json::Value Layer2 = root["Detector2"]["layers"];
        for(G4int icoll = 0; icoll < Layer2.size(); ++icoll) {
            G4double xPosLayer = atof(root["Detector2"]["layers"][icoll]["xPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double yPosLayer = atof(root["Detector2"]["layers"][icoll]["yPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double zPosLayer = atof(root["Detector2"]["layers"][icoll]["zPosLayer"].asString().c_str()) * CLHEP::cm;
            G4double xDirLayer = atof(root["Detector2"]["layers"][icoll]["xDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double yDirLayer = atof(root["Detector2"]["layers"][icoll]["yDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double zDirLayer = atof(root["Detector2"]["layers"][icoll]["zDirLayer"].asString().c_str()) * CLHEP::degree;
            G4double xSizeLayer_ = atof(root["Detector2"]["layers"][icoll]["xSizeLayer"].asString().c_str()) * CLHEP::cm;
            G4double ySizeLayer_ = atof(root["Detector2"]["layers"][icoll]["ySizeLayer"].asString().c_str()) * CLHEP::cm;
            G4double zSizeLayer_ = atof(root["Detector2"]["layers"][icoll]["zSizeLayer"].asString().c_str()) * CLHEP::cm;
            G4int npixelX = atoi(root["Detector2"]["layers"][icoll]["npixelX"].asString().c_str())
            G4int npixelY = atoi(root["Detector2"]["layers"][icoll]["npixelY"].asString().c_str())
            G4double borderSizeX = atof(root["Detector2"]["layers"][icoll]["borderSizeX"].asString().c_str()) * CLHEP::cm;
            G4double borderSizeY = atof(root["Detector2"]["layers"][icoll]["borderSizeY"].asString().c_str()) * CLHEP::cm;
            G4double pixelSizeX = atof(root["Detector2"]["layers"][icoll]["pixelSizeX"].asString().c_str()) * CLHEP::cm;
            G4double pixelSizeY = atof(root["Detector2"]["layers"][icoll]["pixelSizeY"].asString().c_str()) * CLHEP::cm;
            G4double efficiency = atof(root["Detector2"]["layers"][icoll]["efficiency"].asString().c_str()) * CLHEP::cm;
            G4double tResolution = atof(root["Detector2"]["layers"][icoll]["tResolution"].asString().c_str()) * CLHEP::ps;
            if(efficiency < 0 || efficiency > 1) {
                G4cerr << "\033[1;31m" << "Chamber 2 efficiency is not in the range [0, 1]" << "\033[0m" << G4endl;
                goodGeometry = false;
                return;
            }
            detector2->AddLayer(xPosLayer, yPosLayer, zPosLayer, xDirLayer, yDirLayer, zDirLayer, xSizeLayer_, ySizeLayer_, zSizeLayer_, 
                                npixelX, npixelY, borderSizeX, borderSizeY, pixelSizeX, pixelSizeY, efficiency, tResolution);
        }

    }
    goodGeometry = true;
    return;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
bool ConfigurationGeometry::isGood() {
    return goodGeometry;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
G4double ConfigurationGeometry::getSizeX() {
    return uniSizeX;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
G4double ConfigurationGeometry::getSizeY() {
    return uniSizeY;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
G4double ConfigurationGeometry::getSizeZ() {
    return uniSizeZ;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
G4double ConfigurationGeometry::getZOffsetCRY() {
    return zOffsetCRY;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
G4double ConfigurationGeometry::getSizeBoxCRY() {
    return sizeBoxCRY;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
Detector * ConfigurationGeometry::getDetector1() {
    return detector1;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Accesor to class information                                         //
//----------------------------------------------------------------------//
Detector * ConfigurationGeometry::getDetector2() {
    return detector2;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print the class information                                          //
//----------------------------------------------------------------------//
void ConfigurationGeometry::Print() {

    G4cout << "\033[1;34m" << "---------------------------------------Geometry information-----------------------------------" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "The loaded geometry is as follows: " << G4endl;
    G4cout << "\033[1;34m" << "The world is contained in [" << -uniSizeX/2.0 << ", " << uniSizeX/2.0 << "]x[" << -uniSizeY/2.0 << ", " << uniSizeY/2.0 << "]x[" << -uniSizeZ/2.0 << ", " << uniSizeZ/2.0 << "]" << G4endl;
    G4cout << "\033[1;34m" << "Detector 1 located" << G4endl;
    detector1->Print();
    G4cout << "\033[1;34m" << "Detector 2 located" << G4endl;
    detector2->Print();
    G4cout << "\033[0m" << G4endl;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//




