#include "LGAD.hh"
#include "LGADSensor.hh"

//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
LGAD::LGAD(G4double xPos, G4double yPos, G4double zPos,
           G4double xRot, G4double yRot, G4double zRot,
           G4double xSize, G4double ySize, G4double zSize,
           G4int nxpad, G4int nypad, G4double interpadx, G4double interpady,
           G4double borderpadx, G4double borderpady, G4double chargeThres,
           G4double noise, G4double tdcSigma_,
           G4int ndet, G4int nlayer, G4int nsensor) :
           GeomObject(xPos, yPos, zPos, xRot, yRot, zRot, xSize, ySize, zSize) {
            ndetId = ndet;
            nlayerId = nlayer;
            nsensorId = nsensor;
            nPadx = nxpad;
            nPady = nypad;
            interPadx = interpadx;
            interPady = interpady;
            borderPadx = borderpadx;
            borderPady = borderpady;
            chargeThreshold = chargeThres;
            noiseLevel = noise;
            tdcSigma = tdcSigma_;
           };
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return detId                                                         //
//----------------------------------------------------------------------//
G4int LGAD::detId() {
	return ndetId;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Return layerId                                                       //
//----------------------------------------------------------------------//
G4int LGAD::layerId() {
	return nlayerId;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Return sensorId                                                         //
//----------------------------------------------------------------------//
G4int LGAD::sensorId() {
	return nsensorId;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//



//----------------------------------------------------------------------//
// createG4Objects                                              //
//----------------------------------------------------------------------//
void LGAD::createG4Objects(G4String name, G4LogicalVolume *mother,
                               std::map<G4String, G4Material*> &materials,
                               G4SDManager *SDman) {

    G4String LGADName = G4String("LGAD_") + name;  
    solidVolume = new G4Box(LGADName, sizes[0], sizes[1], sizes[2]);
    logicalVolume = new G4LogicalVolume(solidVolume, materials["air"], LGADName);
    G4String LGADPhysicalName = G4String("LGADPhys_") + name;
    physicalVolume = new G4PVPlacement(getRot(), getPos(), 
                                       logicalVolume, LGADPhysicalName,
                                       mother, false, 0, true);

    //We need to make this object sensitive
    G4String SDname = LGADName;
    G4String Collection = G4String("HitsCollection") + G4String(std::to_string(detId()));
    LGADSensor *lgadSensor = new LGADSensor(SDname = SDname, Collection);
    lgadSensor->setLGAD(this);
    SDman->AddNewDetector(lgadSensor);
    logicalVolume->SetSensitiveDetector(lgadSensor);

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void LGAD::Print() {

    G4cout << "\033[1;34m" << "LGAD" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x()/CLHEP::cm << ", y: " << pos.y()/CLHEP::cm << ", z: " << pos.z()/CLHEP::cm << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[1;34m" << "Sizes x: " << sizes.x()/CLHEP::cm << ", y: " << sizes.y()/CLHEP::cm << ", z: " << sizes.z()/CLHEP::cm << G4endl;
    G4cout << "\033[1;34m" << "N pads x: " << nPadx << " N pads y: " << nPady << G4endl; 
    G4cout << "\033[1;34m" << "Interpad x: " << interPadx << " Interpad y: " << interPady << G4endl; 
    G4cout << "\033[1;34m" << "Border x: " << borderPadx << " Border y: " << borderPady << G4endl; 
    G4cout << "\033[1;34m" << "chargeThreshold: " << chargeThreshold << " noise: " << noiseLevel << " tdcSigma: " << tdcSigma << G4endl; 
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
