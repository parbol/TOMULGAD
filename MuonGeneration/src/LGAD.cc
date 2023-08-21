#include "LGAD.hh"


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
LGAD::LGAD(G4double xPos, G4double yPos, G4double zPos,
           G4double xRot, G4double yRot, G4double zRot,
           G4double xSize, G4double ySize, G4double zSize) :
           GeomObject(xPos, yPos, zPos, xRot, yRot, zRot, xSize, ySize, zSize) {};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return LGAD                                                         //
//----------------------------------------------------------------------//
LGADSensor * LGAD::GetSensor() {
	return sensor;
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
    LGADSensor *lgadSensor = new LGADSensor(SDname = "LGADSensor", "HitsCollection");
    lgadSensor->SetStructure(myConf->getDetector1());
    SDman->AddNewDetector(lgad);
    lgadSensorLogical->SetSensitiveDetector(lgad);

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void LGAD::Print() {

    G4cout << "\033[1;34m" << "LGAD" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x() << ", y: " << pos.y() << ", z: " << pos.z() << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[1;34m" << "Sizes x: " << sizes.x() << ", y: " << sizes.y() << ", z: " << sizes.z() << G4endl;
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
