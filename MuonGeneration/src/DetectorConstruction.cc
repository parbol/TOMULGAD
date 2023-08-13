#include "DetectorConstruction.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4Material.hh"
#i1nclude "G4Element.hh"
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
    
    //we start by creating solids for the different components
    G4VSolid *detector = new G4Box("detector", geom->getDetector(0)->getSizes()[0]*CLHEP::cm, 
                                               geom->getDetector(0)->getSizes()[1]*CLHEP::cm,
                                               geom->getDetector(0)->getSizes()[2]*CLHEP::cm);
    G4LogicalVolume *detectorLogical = new G4LogicalVolume(detector, materials["air"], "detector", 0, 0, 0);
    
    G4VSolid *layer = new G4Box("layer", geom->getDetector(0)->getLayer(0)->getSizes()[0]*CLHEP::cm, 
                                         geom->getDetector(0)->getLayer(0)->getSizes()[1]*CLHEP::cm,
                                         geom->getDetector(0)->getLayer(0)->getSizes()[2]*CLHEP::cm);
    G4LogicalVolume *layerLogical = new G4LogicalVolume(detector, materials["air"], "layer", 0, 0, 0);


    G4VSolid *lgadSensor = new G4Box("lgadSensor", geom->getDetector(0)->getLayer(0)->getSensor(0)->getSizes()[0]*CLHEP::cm, 
                                                   geom->getDetector(0)->getLayer(0)->getSensor(0)->getSizes()[1]*CLHEP::cm, 
                                                   geom->getDetector(0)->getLayer(0)->getSensor(0)->getSizes()[2]*CLHEP::cm);
    G4LogicalVolume *lgadSensorLogical = new G4LogicalVolume(lgadSensor, materials["silicon"], "lgadSensor", 0, 0, 0);

    G4VSolid *lgadSupportingPlate = new G4Box("lgadSupportingPlate", geom->getDetector(0)->getSizes()[0]*CLHEP::cm, 
                                                   geom->getDetector(0)->getSizes()[1]*CLHEP::cm, 
                                                   geom->getDetector(0)->getSizes()[2]*CLHEP::cm);

    G4LogicalVolume *lgadSensorLogical = new G4LogicalVolume(lgadSensor, materials["silicon"], "lgadSensor", 0, 0, 0);

    G4VSolid *lgadUnit = new G4Box("lgadUnit", 30.0*CLHEP::cm, 30.0*CLHEP::cm, 1.0*CLHEP::cm);
    G4LogicalVolume *lgadUnitLogical = new G4LogicalVolume(lgadUnit, materials["air"], "lgadUnit", 0, 0, 0);

    //We put the layer inside the chamber    
    G4VPhysicalVolume *vol = new G4PVPlacement((0,0,0), (0,0,0), lgadSensorLogical, "lgadSensor_1", lgadUnitLogical, false, 0, true);
    myConf->getDetector1()->setVolume(vol);

    //Then we put the chamber inside the world
    G4VPhysicalVolume *unit = G4PVPlacement((0,0,0), (0,0,0), lgadUnitLogical, "chamber1_phys", worldLogical, false, 0, true);

    
    LGADSensor *lgad = new LGADSensor(SDname = "LGADSensor", "HitsCollection");
    lgad->SetStructure(myConf->getDetector1());
    SDman->AddNewDetector(lgad);
    lgadSensorLogical->SetSensitiveDetector(lgad);
 
    DumpGeometricalTree(worldPhysicalPrim, 3);
    */
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
