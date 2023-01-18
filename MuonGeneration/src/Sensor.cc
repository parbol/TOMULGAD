#include "Sensor.hh"


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
Sensor::Sensor(G4double xPos, G4double yPos, G4double zPos, G4double xRot, G4double yRot, 
	       G4double zRot, G4double xSize, G4double ySize, G4double zSize,
	       G4int nPadx_, G4int nPady_, G4double interPadx_, G4double interPady_,
	       G4double xborder_, G4double yborder_, G4double chargeThreshold_,
	       G4double noise_, G4double tdcsigma_) : GeomDet(xPos, yPos, zPos, xRot, yRot, zRot, xSize, ySize, zSize) {


    nPadx = nPadx;
    nPady = nPady;
    interPadx = interPadx_;
    interPady = interPady_;
    xborder = xborder_;
    yborder = yborder_;
    chargeThreshold = chargeThreshold_;
    noise = noise_;
    tdcSigma = tdcSigma_;

};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void Sensor::Print() {

    G4cout << "\033[1;34m" << "Sensor" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x() << ", y: " << pos.y() << ", z: " << pos.z() << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[1;34m" << "Sizes x: " << sizes.x() << ", y: " << sizes.y() << ", z: " << sizes.z() << G4endl;
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
