#include "DetectorConstruction.hh"
#include "DriftChamberLayer.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Para.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"

#include "G4ios.hh"


#include "G4PVReplica.hh"

#include "G4SubtractionSolid.hh"



//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
DetectorConstruction::DetectorConstruction(ConfigurationGeometry *w) {
    myConf = w;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Destructor                                                           //
//----------------------------------------------------------------------//
DetectorConstruction::~DetectorConstruction() {

    DestroyMaterials();

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Creates all the geometrical structures                               //
//----------------------------------------------------------------------//
G4VPhysicalVolume* DetectorConstruction::Construct() {

    //Building the materials
    ConstructMaterials();

    //Printing the geometry
    myConf->Print();

    //Manager of objects in memory
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    //Creating the world
    G4VSolid* worldSolidPrim = new G4Box("worldBoxPrim", 1.1 * myConf->getSizeX()/2.0 , 1.1 * myConf->getSizeY() / 2.0 , 1.1 * myConf->getSizeZ()/2.0 );
    G4LogicalVolume* worldLogicalPrim = new G4LogicalVolume(worldSolidPrim, materials["air"], "worldLogicalPrim",0,0,0);
    G4VPhysicalVolume* worldPhysicalPrim = new G4PVPlacement(0, G4ThreeVector(), worldLogicalPrim, "worldPhysicalPrim", 0, 0, 0);

    G4VSolid* worldSolid = new G4Box("worldBox", myConf->getSizeX()/2.0 , myConf->getSizeY()/2.0 , myConf->getSizeZ()/2.0 );
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, materials["air"], "worldLogical",0,0,0);
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLogical, "worldPhysical", worldLogicalPrim, false, 0);

    //Creating the layer
    if(myConf->getIsLayer()) {
        G4Box * leadLayer = new G4Box("leadLayer", myConf->getSizeXLayer()/2.0, myConf->getSizeYLayer()/2.0, myConf->getSizeZLayer()/2.0);
        G4LogicalVolume *leadLayerLogical = new G4LogicalVolume(leadLayer, materials[myConf->getOuterMaterialLayer()], "leadLayerLogical", 0, 0, 0);
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), leadLayerLogical, "leadLayerPhysical", worldLogical, false, 0, true);
    }

    //Creating the Pipe
    if(myConf->getIsPipe()) {
        G4Tubs * pipeWall = new G4Tubs("pipeWall", myConf->getInnerRadiusPipe() , myConf->getOuterRadiusPipe() , 0.9 * myConf->getSizeZ()/2.0 , 0 * CLHEP::deg, 360.0 * CLHEP::deg);
        G4Tubs * pipeInterior = new G4Tubs("pipeInterior", 0.0 , myConf->getInnerRadiusPipe() , 0.9 * myConf->getSizeZ()/2.0 , 0 * CLHEP::deg, 360.0 * CLHEP::deg);
        G4LogicalVolume *pipeWallVolume = new G4LogicalVolume(pipeWall, materials[myConf->getOuterMaterialPipe()], "pipeWallLogical");
        G4LogicalVolume *pipeInteriorVolume = new G4LogicalVolume(pipeInterior, materials[myConf->getInnerMaterialPipe()], "pipeInteriorLogical");
        G4RotationMatrix * rot = new G4RotationMatrix();
        rot->rotateY(90.0*CLHEP::deg);
        new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), pipeWallVolume, "pipeWallPhysical", worldLogicalPrim, false, true);
        new G4PVPlacement(rot, G4ThreeVector(0, 0, 0), pipeInteriorVolume, "pipeInteriorPhysical", worldLogicalPrim, false, true);
    }


    //Creating the BlastFurance
    if(myConf->getIsBlastFurnace()) {
        G4Tubs * blastFurnaceWall = new G4Tubs("blastFurnaceWall", myConf->getInnerRadius() * CLHEP::cm, myConf->getOuterRadius() * CLHEP::cm, 0.9 * myConf->getSizeZ()/2.0 * CLHEP::cm, 0 * CLHEP::deg, 360.0 * CLHEP::deg);
        G4Tubs * blastFurnaceInterior = new G4Tubs("blastFurnaceInterior", 0.0 * CLHEP::cm, myConf->getInnerRadius() * CLHEP::cm, 0.9 * myConf->getSizeZ()/2.0 * CLHEP::cm, 0 * CLHEP::deg, 360.0 * CLHEP::deg);
        G4LogicalVolume *blastFurnaceWallVolume = new G4LogicalVolume(blastFurnaceWall, materials[myConf->getOuterMaterial()], "blastFurnaceWallLogical");
        G4LogicalVolume *blastFurnaceInteriorVolume = new G4LogicalVolume(blastFurnaceInterior, materials[myConf->getInnerMaterial()], "blastFurnaceInteriorLogical");
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), blastFurnaceWallVolume, "blastFurnaceWallPhysical", worldLogicalPrim, false, 0);
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), blastFurnaceInteriorVolume, "blastFurnaceInteriorPhysical", worldLogicalPrim, false, 0);
    }


    G4VSolid* layer1 = new G4Box("layer1", myConf->getDetector1()->getSizeLayer(0).x()/2.0, myConf->getDetector1()->getSizeLayer(0).y()/2.0, myConf->getDetector1()->getSizeLayer(0).z()/2.0);
    G4LogicalVolume* layerLogical1 = new G4LogicalVolume(layer1, materials["argon"], "layerLog1", 0, 0, 0);

    G4VSolid* layer2 = new G4Box("layer2", myConf->getDetector2()->getSizeLayer(0).x()/2.0, myConf->getDetector2()->getSizeLayer(0).y()/2.0, myConf->getDetector2()->getSizeLayer(0).z()/2.0);
    G4LogicalVolume* layerLogical2 = new G4LogicalVolume(layer2, materials["argon"], "layerLog2", 0, 0, 0);

    G4VSolid* chamber1 = new G4Box("Chamber1", myConf->getDetector1()->getSizes().x()/2.0, myConf->getDetector1()->getSizes().y()/2.0, myConf->getDetector1()->getSizes().z()/2.0);
    G4LogicalVolume* chamber1Logical = new G4LogicalVolume(chamber1, materials["air"], "Chamber1Log", 0, 0, 0);

    G4VSolid* chamber2 = new G4Box("Chamber2", myConf->getDetector2()->getSizes().x()/2.0, myConf->getDetector2()->getSizes().y()/2.0, myConf->getDetector2()->getSizes().z()/2.0);
    G4LogicalVolume* chamber2Logical = new G4LogicalVolume(chamber2, materials["air"], "Chamber2Log", 0, 0, 0);


    for(G4int c_layer = 0; c_layer < myConf->getDetector1()->getNLayers(); c_layer++) {

        G4VPhysicalVolume *vol = new G4PVPlacement(myConf->getDetector1()->getRotLayer(c_layer), myConf->getDetector1()->getPosLayer(c_layer), layerLogical1, "p_layer1", chamber1Logical, false, c_layer, true);
        myConf->getDetector1()->setVolume(vol);

    }

    for(G4int c_layer = 0; c_layer < myConf->getDetector2()->getNLayers(); c_layer++) {

        G4VPhysicalVolume *vol = new G4PVPlacement(myConf->getDetector2()->getRotLayer(c_layer), myConf->getDetector2()->getPosLayer(c_layer), layerLogical2, "p_layer2", chamber2Logical, false, c_layer, true);
        myConf->getDetector2()->setVolume(vol);

    }


    //Careful to the units
    new G4PVPlacement(myConf->getDetector1()->getRot(), myConf->getDetector1()->getPos(), chamber1Logical, "chamber1_phys", worldLogical, false, 0, true);
    new G4PVPlacement(myConf->getDetector2()->getRot(), myConf->getDetector2()->getPos(), chamber2Logical, "chamber2_phys", worldLogical, false, 0, true);

    //G4VSolid* activeLayer1 = new G4Box("activeLayer1", myConf->getDetector1()->getSizes().x()/2.0, myConf->getDetector1()->getSizes().y()/2.0, myConf->getDetector1()->getSizes().z()/2.0);
    //G4LogicalVolume* activeLayer1Logical = new G4LogicalVolume(activeLayer1, materials["argon"], "activeLayer1Logical", 0, 0, 0);
    //new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), activeLayer1Logical, "activeLayer1Physical", chamber1Logical, false, 0, true);

    //G4VSolid* activeLayer2 = new G4Box("activeLayer2", myConf->getDetector2()->getSizes().x()/2.0, myConf->getDetector2()->getSizes().y()/2.0, myConf->getDetector2()->getSizes().z()/2.0);
    //G4LogicalVolume* activeLayer2Logical = new G4LogicalVolume(activeLayer2, materials["argon"], "activeLayer2Logical", 0, 0, 0);
    //new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), activeLayer2Logical, "activeLayer2Physical", chamber2Logical, false, 0, true);

    DriftChamberLayer *theChamber1 = new DriftChamberLayer(SDname = "Chamber1", "HitsCollection");
    theChamber1->SetStructure(myConf->getDetector1());
    SDman->AddNewDetector(theChamber1);
    layerLogical1->SetSensitiveDetector(theChamber1);
    //activeLayer1Logical->SetSensitiveDetector(theChamber1);
    DriftChamberLayer *theChamber2 = new DriftChamberLayer(SDname = "Chamber2", "HitsCollection");
    theChamber2->SetStructure(myConf->getDetector2());
    SDman->AddNewDetector(theChamber2);
    layerLogical2->SetSensitiveDetector(theChamber2);
    //activeLayer2Logical->SetSensitiveDetector(theChamber2);

    DumpGeometricalTree(worldPhysicalPrim, 3);


    return worldPhysicalPrim;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Construct all the materials                                          //
//----------------------------------------------------------------------//
void DetectorConstruction::ConstructMaterials() {

    G4NistManager* man = G4NistManager::Instance();

    materials.insert(std::pair<G4String, G4Material *>("air", man->FindOrBuildMaterial("G4_AIR")));
    materials.insert(std::pair<G4String, G4Material *>("iron", man->FindOrBuildMaterial("G4_Fe")));
    materials.insert(std::pair<G4String, G4Material *>("uranium", man->FindOrBuildMaterial("G4_U")));
    materials.insert(std::pair<G4String, G4Material *>("aluminium", man->FindOrBuildMaterial("G4_Al")));
    materials.insert(std::pair<G4String, G4Material *>("argon", man->FindOrBuildMaterial("G4_Ar")));
    materials.insert(std::pair<G4String, G4Material *>("lead", man->FindOrBuildMaterial("G4_Pb")));
    materials.insert(std::pair<G4String, G4Material *>("silicon", man->FindOrBuildMaterial("G4_Si")));
    materials.insert(std::pair<G4String, G4Material *>("steel", man->FindOrBuildMaterial("G4_STAINLESS-STEEL")));


}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Destroy all the materials                                            //
//----------------------------------------------------------------------//
void DetectorConstruction::DestroyMaterials() {
    // Destroy all allocated elements and materials
    size_t i;
    G4MaterialTable* matTable = (G4MaterialTable*)G4Material::GetMaterialTable();
    for(i=0; i<matTable->size(); i++) delete (*(matTable))[i];
    matTable->clear();
    G4ElementTable* elemTable = (G4ElementTable*)G4Element::GetElementTable();
    for(i=0; i<elemTable->size(); i++) delete (*(elemTable))[i];
    elemTable->clear();

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


void DetectorConstruction::DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth)
{

    for(int isp=0; isp<depth; isp++)
    {
        G4cout << "  ";
    }
    G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
           << aVolume->GetLogicalVolume()->GetName() << " "
           << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
           << aVolume->GetLogicalVolume()->GetMaterial()->GetName();
    if(aVolume->GetLogicalVolume()->GetSensitiveDetector())
    {
        G4cout << " " << aVolume->GetLogicalVolume()->GetSensitiveDetector()->GetFullPathName();
    }
    G4cout << G4endl;
    for(int i=0; i<aVolume->GetLogicalVolume()->GetNoDaughters(); i++)
    {
        DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1);
    }

}
