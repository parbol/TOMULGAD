#include "Detector.hh"




//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
Detector::Detector(G4double xPos, G4double yPos, G4double zPos, G4double xRot, G4double yRot, G4double zRot, G4double xSize, G4double ySize, G4double zSize) : GeomObject(xPos, yPos, zPos, xRot, yRot, zRot, xSize, ySize, zSize) {};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Add a layer to the detector                                          //
//----------------------------------------------------------------------//
void Detector::AddLayer(Layer *l) {
	layers.push_back(l);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return layer                                                         //
//----------------------------------------------------------------------//
Layer * Detector::GetLayer(G4int a) {
	return layers.at(a);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return number of layers                                              //
//----------------------------------------------------------------------//
G4int Detector::GetNLayers() {
	return layers.size();
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// createG4Objects                                              //
//----------------------------------------------------------------------//
void Detector::createG4Objects(G4String name, G4LogicalVolume *mother) {

    G4String detName = G4String("detector_") + name;  
    solidVolume = new G4Box(detName, sizes[0], sizes[1], sizes[2]);
    logicalVolume = new G4LogicalVolume(solidVolume, materials["air"]), detName, 0., 0., 0.);
    for(int i = 0; i < layers.size(); i++) {
        G4String layerName = name + G4String("_") + G4String(to_string(i)); 
        layers[i].createG4Objects(layerName, logicalVolume);
    }     
    G4String detPhysicalName = G4String("detectorPhys_") + name;
    physicalVolume = new G4PVPlacement((pos[0], pos[1], pos[2]),
                                       (dir[0], dir[1], dir[2]),
                                       logicalVolume, detPhysicalName,
                                       mother, false);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void Detector::Print() {

    G4cout << "\033[1;34m" << "Detector" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x() << ", y: " << pos.y() << ", z: " << pos.z() << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[1;34m" << "Sizes x: " << sizes.x() << ", y: " << sizes.y() << ", z: " << sizes.z() << G4endl;
    G4cout << "\033[1;34m" << "Number of layers " << GetNLayers() << G4endl;
    for(G4int i = 0; i < GetNLayers(); i++) {
        layers.at(i)->Print();
    }
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
