#include "Layer.hh"


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
Layer::Layer(G4double xPos, G4double yPos, G4double zPos, 
             G4double xRot, G4double yRot, G4double zRot,
             G4double xSize, G4double ySize, G4double zSize) :
             GeomObject(xPos, yPos, zPos, xRot, yRot, zRot, xSize, ySize, zSize) {};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Add a layer to the detector                                          //
//----------------------------------------------------------------------//
void Layer::AddSensor(LGAD *l) {
	sensors.push_back(l);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return layer                                                         //
//----------------------------------------------------------------------//
LGAD * Layer::GetSensor(G4int a) {
	return sensors.at(a);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return number of layers                                              //
//----------------------------------------------------------------------//
G4int Layer::GetNSensors() {
	return sensors.size();
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// createG4Objects                                              //
//----------------------------------------------------------------------//
void Layer::createG4Objects(G4String name, G4LogicalVolume *mother,
                            std::map<G4String, G4Material*> & materials,
                            G4SDManager *SDman) {

    G4String layerName = G4String("layer_") + name;  
    solidVolume = new G4Box(layerName, sizes[0], sizes[1], sizes[2]);
    logicalVolume = new G4LogicalVolume(solidVolume, materials["air"], layerName);
    for(int i = 0; i < sensors.size(); i++) {
        G4String sensorName = name + G4String("_") + G4String(std::to_string(i));  
        sensors[i]->createG4Objects(sensorName, logicalVolume, materials, SDman);
    }     
    G4String layerPhysicalName = G4String("layerPhys_") + name;
    physicalVolume = new G4PVPlacement(getRot(), getPos(), 
                                       logicalVolume, layerPhysicalName,
                                       mother, false, 0, true);
   
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void Layer::Print() {

    G4cout << "\033[1;34m" << "Layer" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x() << ", y: " << pos.y() << ", z: " << pos.z() << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[1;34m" << "Sizes x: " << sizes.x() << ", y: " << sizes.y() << ", z: " << sizes.z() << G4endl;
    G4cout << "\033[1;34m" << "Number of sensors " << GetNSensors() << G4endl;
    for(G4int i = 0; i < GetNSensors(); i++) {
        sensors.at(i)->Print();
    }
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
