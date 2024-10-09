#include "Phantom.hh"
#define DISK 0
#define PRISM 1




//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
Phantom::Phantom(G4double xPos, G4double yPos, G4double zPos, G4double xRot, 
                   G4double yRot, G4double zRot, G4double radius_, G4double zsize_, G4String name_, G4String material_) : GeomObject(xPos, yPos, zPos, xRot, yRot,
                   zRot, 0.0, 0.0, 0.0) {
                    radius = radius_;
		    zsize = zsize_;
	 	    name = name_;
		    material = material_;
		    type = DISK; //
                   };
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
Phantom::Phantom(G4double xPos, G4double yPos, G4double zPos, G4double xRot,
                   G4double yRot, G4double zRot, G4double sideX_, G4double sideY_, G4double sideZ_, G4String name_, G4String material_) : GeomObject(xPos, yPos, zPos, xRot, yRot,
                   zRot, 0.0, 0.0, 0.0) {
                    sideX = sideX_;
                    sideY = sideY_;
		    sideZ = sideZ_;
                    name = name_;
                    material = material_;
                    type = PRISM; //
                   };
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return detId                                                         //
//----------------------------------------------------------------------//
G4String Phantom::getName() {
	return name;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Return detId                                                         //
//----------------------------------------------------------------------//
G4String Phantom::getMaterial() {
	return material;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// createG4Objects                                              //
//----------------------------------------------------------------------//
void Phantom::createG4Objects(G4LogicalVolume *mother,
                               std::map<G4String, G4Material*> & materials,
                               G4SDManager *SDman) {
    G4String phantomName = G4String("phantom_") + name;  
    if(type == DISK) {
        solidVolume = new G4Tubs(phantomName, 0, radius, zsize/2.0, 0.0, CLHEP::twopi);
    } else if(type == PRISM) {
        solidVolume = new G4Box(phantomName, sideX/2.0, sideY/2.0, sideZ/2.0);
    }    
    logicalVolume = new G4LogicalVolume(solidVolume, materials[material], phantomName);
    G4String phantomPhysicalName = G4String("phantomPhys_") + name;
    physicalVolume = new G4PVPlacement(getRot(), getPos(), logicalVolume, phantomPhysicalName,
                                       mother, false, 0, true);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Print                                                                //
//----------------------------------------------------------------------//
void Phantom::Print() {

    G4cout << "\033[1;34m" << "Phantom" << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Name: " << name << " Material: " << material << "\033[0m" << G4endl;
    G4cout << "\033[1;34m" << "Location x: " << pos.x()/CLHEP::cm << ", y: " << pos.y()/CLHEP::cm << ", z: " << pos.z()/CLHEP::cm << G4endl;
    G4cout << "\033[1;34m" << "Rotation x: " << rots.x() << ", y: " << rots.y() << ", z: " << rots.z() << G4endl;
    G4cout << "\033[0m" << G4endl;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
