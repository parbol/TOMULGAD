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
#include "G4Cons.hh"

#include "G4Para.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"

#include "G4ios.hh"


#include "G4PVReplica.hh"

#include "G4SubtractionSolid.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <algorithm>
#include <math.h>
#include <sys/time.h>


//Check volumes
G4bool checkSurface = false;


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

    if(!checkSurface) G4cout << " **** WARNING : Deactivate checking overlaps for volumes. Activate at DetectorConstruction.cc setting checkSurface to true, for a better solid location control." << G4endl;

    //Building the materials
    ConstructMaterials();

    //Printing the geometry
    myConf->Print();

    //Manager of objects in memory
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
1
    //Creating the world
    G4VSolid* worldSolid = new G4Box("worldBox", myConf->getSizeX()/2.0, myConf->getSizeY()/2.0, myConf->getSizeZ()/2.0 );
    G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, materials["air"], "worldLogical",0,0,0);
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLogical, "worldPhysical", 0, false, 0, checkSurface);
    G4VisAttributes *worldVisAtt = new G4VisAttributes(G4Colour(255.0,6.0,0.0));
    worldVisAtt->SetVisibility(false);
    worldLogical->SetVisAttributes(worldVisAtt);

    //Chamber attr
    G4VisAttributes *attr_chamber = new G4VisAttributes(G4Colour::Magenta());
    G4Colour* chamber_color = new G4Colour(0, 0, 1, 0.5);
    attr_chamber->SetVisibility(false);
    attr_chamber->SetColor(*chamber_color);
    
    //Chambers and layers definition    
    G4LogicalVolume* layers_det1 [myConf->getDetector1()->getNLayers()];
    for(G4int c_layer = 0; c_layer < myConf->getDetector1()->getNLayers(); c_layer++) {
        layers_det1[c_layer] = getPhysicalLayer("Detector1", myConf->getDetector1(), c_layer, SDman);
    }
	
    G4LogicalVolume* layers_det2 [myConf->getDetector2()->getNLayers()];
    for(G4int c_layer = 0; c_layer < myConf->getDetector2()->getNLayers(); c_layer++) {    
            layers_det2[c_layer] = getPhysicalLayer("Detector2", myConf->getDetector2(), c_layer, SDman);
    }

    G4VSolid* chamber1 = new G4Box("Chamber1", myConf->getDetector1()->getSizes().x()/2.0, myConf->getDetector1()->getSizes().y()/2.0, myConf->getDetector1()->getSizes().z()/2.0);
    G4LogicalVolume* chamber1Logical = new G4LogicalVolume(chamber1, materials["air"], "Chamber1Log", 0, 0, 0);
    chamber1Logical->SetVisAttributes(attr_chamber);
    
    G4VSolid* chamber2 = new G4Box("Chamber2", myConf->getDetector2()->getSizes().x()/2.0, myConf->getDetector2()->getSizes().y()/2.0, myConf->getDetector2()->getSizes().z()/2.0);
    G4LogicalVolume* chamber2Logical = new G4LogicalVolume(chamber2, materials["air"], "Chamber2Log", 0, 0, 0);
    chamber2Logical->SetVisAttributes(attr_chamber);

    for(G4int c_layer = 0; c_layer < myConf->getDetector1()->getNLayers(); c_layer++) {

        G4VPhysicalVolume *vol = new G4PVPlacement(myConf->getDetector1()->getRotLayer(c_layer), myConf->getDetector1()->getPosLayer(c_layer), 
                                                   layers_det1[c_layer], "p_layer1"+ std::to_string(c_layer), chamber1Logical, false, c_layer, checkSurface);
        myConf->getDetector1()->setVolume(vol);
    }

    for(G4int c_layer = 0; c_layer < myConf->getDetector2()->getNLayers(); c_layer++) {

        G4VPhysicalVolume *vol = new G4PVPlacement(myConf->getDetector2()->getRotLayer(c_layer), myConf->getDetector2()->getPosLayer(c_layer),
                                                   layers_det2[c_layer], "p_layer2", chamber2Logical, false, c_layer, checkSurface);
        myConf->getDetector2()->setVolume(vol);

    }

    new G4PVPlacement(myConf->getDetector1()->getRot(), myConf->getDetector1()->getPos(), chamber1Logical, "chamber1_phys", worldLogical, false, 0, checkSurface);
    new G4PVPlacement(myConf->getDetector2()->getRot(), myConf->getDetector2()->getPos(), chamber2Logical, "chamber2_phys", worldLogical, false, 0, checkSurface);

    DumpGeometricalTree(worldPhysical, 3);


    return worldPhysical;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//





//----------------------------------------------------------------------//
// Creates a chamber with a sensitive detection volumen
//----------------------------------------------------------------------//
G4LogicalVolume*  DetectorConstruction::getPhysicalLayer(G4String name, Detector *myDetector, G4int c_layer, G4SDManager* SDman) {

    
    //Variables de caracterización de los objetos que forman el layer    
    G4ThreeVector layer_pos = myDetector->getPosLayer(c_layer);
    G4double ROC_thickness = 1 * CLHEP::mm;
    G4double SENSOR_thickness = 0.15 * CLHEP::mm;
    G4double SUBSTRATE_thickness = 1 * CLHEP::mm;
    
    G4double total_height = ROC_thickness + SENSOR_thickness + SUBSTRATE_thickness;
        
    //G4double delta_z_copper =  cathod_gap/2. + copperLayer_thickness/2.0;
    //G4double delta_z_glassfiber = cathod_gap/2. + copperLayer_thickness + glassFiber_thickness/2.0;
    //G4double delta_z_profile =  cathod_gap/2. + copperLayer_thickness + glassFiber_thickness + profile_height/2.0;
    // altura del volumen formado por la fibra de PC, el cobre y la distancia entre placas
    //G4double sandwich_height = cathod_gap + 2*copperLayer_thickness + 2*glassFiber_thickness;
    
    // DEFINICION DE ATRIBUTOS
    // Atributos contenedor layer
    G4VisAttributes *attr_layer = new G4VisAttributes(G4Colour::Magenta());
    G4Colour* layer_color = new G4Colour(53./255., 81./255., 92./255., 0.25);
    attr_layer->SetVisibility(false);
    attr_layer->SetColor(*layer_color);
            
    // Atributos cobre
    G4VisAttributes *attr_copper = new G4VisAttributes(G4Colour::Magenta());
    G4Colour* copper_color = new G4Colour(203./255., 109./255., 81./255., 1);
    attr_copper->SetVisibility(true);
    attr_copper->SetColor(*copper_color);
    
    // Atributos argon
    G4VisAttributes *attr_silicon = new G4VisAttributes(G4Colour::Magenta());
    G4Colour* silicon_color = new G4Colour(255./255., 34./255., 0./255., .5);
    attr_argon->SetVisibility(true);
    attr_argon->SetColor(*silicon_silicon);
    
    // Atributos perfil substrato
    G4VisAttributes *attr_substrate = new G4VisAttributes(G4Colour::Magenta());
    G4Colour* substrate_color = new G4Colour(143./255., 143./255., 143./255., 1);
    attr_substrate->SetVisibility(true);
    attr_substrate->SetColor(*substrate_color);
    
    
    // DEFINICION DE LOS ELEMENTOS FISICOS Y LOGICOS DE LA CAMARA
    // Contenedor de la cámara
    G4String myName = name + "_layer_" + std::to_string(c_layer);
    
    //Defino el volumen "layer" que englobará todos los elementos.
    G4VSolid* layer = new G4Box(myName, myDetector->getSizeLayer(c_layer).x()/2.0, myDetector->getSizeLayer(c_layer).y()/2.0, total_height/2.);
    G4LogicalVolume* layerLogical = new G4LogicalVolume(layer, materials["air"], myName+"_Log", 0, 0, 0);
    layerLogical->SetVisAttributes(attr_layer);
    
    // ROC
    myName = name + "_ROC_" + std::to_string(c_layer);
    G4VSolid* layer_ROC_top = new G4Box(myName, myDetector->getSizeLayer(c_layer).x()/2.0, myDetector->getSizeLayer(c_layer).y()/2.0, ROC_thickness/2.0);
    G4LogicalVolume* layerLogical_ROC_top = new G4LogicalVolume(layer_ROC_top, materials["copper"], myName+"_Log", 0, 0, 0);
    layerLogical_ROC_top->SetVisAttributes(attr_copper);
    
    // SENSOR
    myName = name + "_SENSOR_" + std::to_string(c_layer);
    G4VSolid* layer_SENSOR_top = new G4Box(myName, myDetector->getSizeLayer(c_layer).x()/2.0, myDetector->getSizeLayer(c_layer).y()/2.0, SENSOR_thickness/2.0);
    G4LogicalVolume* layerLogical_SENSOR_top = new G4LogicalVolume(layer_SENSOR_top, materials["silicon"], myName+"_Log", 0, 0, 0);
    layerLogical_SENSOR_top->SetVisAttributes(attr_silicion);

    // SUBSTRATE
    myName = name + "_SUBSTRATE_" + std::to_string(c_layer);
    G4VSolid* layer_SUBSTRATE_top = new G4Box(myName, myDetector->getSizeLayer(c_layer).x()/2.0, myDetector->getSizeLayer(c_layer).y()/2.0, SUBSTRATE_thickness/2.0);
    G4LogicalVolume* layerLogical_SENSOR_top = new G4LogicalVolume(layer_SENSOR_top, materials["silicon"], myName+"_Log", 0, 0, 0);
    layerLogical_SUBSTRATE_top->SetVisAttributes(attr_substrate);
    
    // POSICIONAMIENTO ESPACIAL
    // posciono los volumenes en las cámaras superiores del conjunto XY
    // las cámaras pares tendrán perfiles en su lado superior

    // posiciono el ROC 
    myName = name + "_ROC_top_p_" + std::to_string(c_layer);
    new G4PVPlacement(0, G4ThreeVector(0, 0, SENSOR_thickness/2.0 + ROC_thickness/2.0), layerLogical_ROC_top, myName, layerLogical, false, c_layer, checkSurface);
    // posiciono el SENSOR
    myName = name + "_ROC_top_p_" + std::to_string(c_layer);
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), layerLogical_SENSOR_top, myName, layerLogical, false, c_layer, checkSurface);
    // posiciono el SUBSTRATO
    myName = name + "_SUBSTRATE_top_p_" + std::to_string(c_layer);
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), layerLogical_SUBSTRATE_top, myName, layerLogical, false, c_layer, checkSurface);
       
    
    // Asociamos el sensor a la par sensible del detector
    G4String SDname;
    DriftChamberLayer *theChamber = new DriftChamberLayer(SDname = name + "_Chamber_" + std::to_string(c_layer), "HitsCollection_"+name+"_"+std::to_string(c_layer));
    theChamber->SetStructure(myDetector);
    SDman->AddNewDetector(theChamber);
    layerLogical_SENSOR->SetSensitiveDetector(theChamber);
    
    return layerLogical;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Creates a chamber with a sensitive detection volumen
//----------------------------------------------------------------------//
G4ThreeVector  DetectorConstruction::getMaterialColor(G4String material_name) {
    
    G4double red, green, blue;

    if(material_name == "steel") {
        red = 202./255.;
        green = 111./255.;
        blue = 30./255.;
    }else if(material_name == "iron") {
        red = 255./255.;
        green = 175./255.;
        blue = 51./255.;    
    }else if(material_name == "silicon") {
        red = 242./255.;
        green = 255./255.;
        blue = 51./255.;        
    }else if(material_name == "scrap") {
        red = 72./255.;
        green = 25./255.;
        blue = 5./255.;
    }else if(material_name == "air") {
        red = 52./255.;
        green = 12./255.;
        blue = 219./255.;
    }else if(material_name == "cascarilla") {
        red = 72./255.;
        green = 25./255.;
        blue = 5./255.;
    }else if(material_name == "water") {
        red = 72./255.;
        green = 25./255.;
        blue = 5./255.;
    }else if(material_name == "muscle") {
        red = 161./255.;
        green = 44./255.;
        blue = 44./255.;
    }else if(material_name == "boneCompact") {
        red = 227./255.;
        green = 218./255.;
        blue = 201./255.;
    }else if(material_name == "slag") {
        red = 168./255.;
        green = 167./255.;
        blue = 164./255.;
    }else if(material_name == "tailing_2.1") {
        red = 133./255.;
        green = 87./255.;
        blue = 35./255.;
    }else if(material_name == "tailing_1.9") {
        red = 146./255.;
        green = 104./255.;
        blue = 41./255.;
    }else if(material_name == "tailing_1.7") {
        red = 185./255.;
        green = 156./255.;
        blue = 107./255.;
    }else if(material_name == "tailing_1.5") {
        red = 234./255.;
        green = 182./255.;
        blue = 79./255.;
    }else if(material_name == "uranium") {
        red = 0./255.;
        green = 255./255.;
        blue = 0./255.;
    }else if(material_name == "aluminium") {
        red = 143./255.;
        green = 143./255.;
        blue = 143./255.;
    }else if(material_name == "argon") {
        red = 52./255.;
        green = 12./255.;
        blue = 219./255.;
    }else if(material_name == "lead") {
        red = 121./255.;
        green = 128./255.;
        blue = 129./255.;
    }else if(material_name == "copper") {
        red = 184./255.;
        green = 115./255.;
        blue = 51./255.;
    }else if(material_name == "copper") {
        red = 203./255.;
        green = 109./255.;
        blue = 81./255.;
    }else if(material_name == "concrete") {
        red = 210./255.;
        green = 209./255.;
        blue = 205./255.;
    }else if(material_name == "concrete_2.4") {
        red = 168./255.;
        green = 167./255.;
        blue = 164./255.;
    }else if(material_name == "carbonRefractory") {
        red = 25./255.;
        green = 25./255.;
        blue = 25./255.;
    }else if(material_name == "liquidSteel") {
        red = 211./255.;
        green = 34./255.;
        blue = 19./255.;
    }else if(material_name == "rockwool") {
        red = 243./255.;
        green = 255./255.;
        blue = 105./255.;
    }else if(material_name == "glassfiber") {
        red = 0./255.;
        green = 187./255.;
        blue = 45./255.;
    }else {
        red = 255./255.;
        green = 0./255.;
        blue = 255./255.;
    }

    G4ThreeVector color_vect(red,green,blue);
    return color_vect;


}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//



//----------------------------------------------------------------------//
// Construct all the materials                                          //
//----------------------------------------------------------------------//
void DetectorConstruction::ConstructMaterials() {

    G4NistManager* man = G4NistManager::Instance();

    //G4double density, fractionmass;
    //G4int nElem;

    materials.insert(std::pair<G4String, G4Material *>("air", man->FindOrBuildMaterial("G4_AIR")));
    materials.insert(std::pair<G4String, G4Material *>("iron", man->FindOrBuildMaterial("G4_Fe")));
    materials.insert(std::pair<G4String, G4Material *>("uranium", man->FindOrBuildMaterial("G4_U")));
    materials.insert(std::pair<G4String, G4Material *>("aluminium", man->FindOrBuildMaterial("G4_Al")));
    materials.insert(std::pair<G4String, G4Material *>("argon", man->FindOrBuildMaterial("G4_Ar")));
    materials.insert(std::pair<G4String, G4Material *>("lead", man->FindOrBuildMaterial("G4_Pb")));
    materials.insert(std::pair<G4String, G4Material *>("silicon", man->FindOrBuildMaterial("G4_Si")));
    materials.insert(std::pair<G4String, G4Material *>("copper", man->FindOrBuildMaterial("G4_Cu")));
    materials.insert(std::pair<G4String, G4Material *>("wolframio", man->FindOrBuildMaterial("G4_W")));
    
    //G4Material* liquidSteel = new G4Material("liquidSteel", density = 6.00*g/cm3, nElem = 6);
    //liquidSteel->AddElement(man->FindOrBuildElement("G4_C"), fractionmass=0.1*perCent);
    //materials.insert(std::pair<G4String, G4Material *>("liquidSteel", liquidSteel));
    //


    G4double z, a, fractionmass, density;
    G4String name, symbol;
    G4int ncomponents;
    a = 14.01*g/mole;
    G4Element* elN  = new G4Element(name="Nitrogen",symbol="N", z= 7., a);

    a = 16.00*g/mole;
    G4Element* elO  = new G4Element(name="oxygen",symbol="O", z= 8., a);

    a = 1.01*g/mole;
    G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
    a = 12.01*g/mole;
    G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
    a = 22.99*g/mole;
    G4Element* elNa= new G4Element(name="Sodio", symbol="Na", z=11., a);
    a = 24.32*g/mole;
    G4Element* elMg= new G4Element(name="Magnesium", symbol="Mg", z=12., a);
    a = 26.98*g/mole;
    G4Element* elAl= new G4Element(name="aluminium", symbol="Al", z=13., a);
    a = 28*g/mole;
    G4Element* elSi= new G4Element(name="silicon", symbol="Si", z=14., a);
    a = 30.97*g/mole;
    G4Element* elP= new G4Element(name="Phosphorous", symbol="P", z=15., a);
    a = 32.06*g/mole;
    G4Element* elS= new G4Element(name="Sulphur", symbol="S", z=16., a);
    a = 35.45*g/mole;
    G4Element* elCl= new G4Element(name="Chlorine", symbol="Cl", z=17., a);
    a = 39.09*g/mole;
    G4Element* elK= new G4Element(name="Potassium", symbol="K", z=19., a);
    a = 40.08*g/mole;
    G4Element* elCa= new G4Element(name="Calcium", symbol="Ca", z=20., a);
    a = 55.85*g/mole;
    G4Element* elFe= new G4Element(name="Iron", symbol="Fe", z=26., a);
    a = 65.38*g/mole;
    G4Element* elZn= new G4Element(name="Zinc", symbol="Zn", z=26., a);
    a = 126.9*g/mole;
    G4Element* elI = new G4Element(name="Iodine", symbol="I", z=53., a);
    a = 132.9*g/mole;
    G4Element* elCs= new G4Element(name="Cesium", symbol="Cs", z=55., a);
    
    G4Element* elCr = man->FindOrBuildElement("Cr");
    G4Element* elMn = man->FindOrBuildElement("Mn");
    G4Element* elNi = man->FindOrBuildElement("Ni");
    G4Element* elTi = man->FindOrBuildElement("Ti");
    G4Element* elCu = man->FindOrBuildElement("Cu");
    G4Element* elSn = man->FindOrBuildElement("Sn");
    G4Element* elMo = man->FindOrBuildElement("Mo");
    G4Element* elV = man->FindOrBuildElement("V");
    
    //Concrete
    density = 2.3*g/cm3;
    G4Material* concrete = new G4Material(name="Concrete", density, ncomponents=10);
    concrete->AddElement( elH, 0.01    );
    concrete->AddElement( elC, 0.001   );
    concrete->AddElement( elO, 0.529107);
    concrete->AddElement( elNa, 0.016   );
    concrete->AddElement(elMg, 0.002   );
    concrete->AddElement(elAl, 0.033872);
    concrete->AddElement(elSi, 0.337021);
    concrete->AddElement(elK, 0.013   );
    concrete->AddElement(elCa, 0.044   );
    concrete->AddElement(elFe, 0.014   );
    materials.insert(std::pair<G4String, G4Material *>("concrete", concrete));
    
    //Concrete 2.4
    density = 2.4*g/cm3;
    G4Material* concrete_2_4 = new G4Material(name="Concrete_2.4", density, ncomponents=10);
    concrete_2_4->AddElement( elH, 0.01    );
    concrete_2_4->AddElement( elC, 0.001   );
    concrete_2_4->AddElement( elO, 0.529107);
    concrete_2_4->AddElement( elNa, 0.016   );
    concrete_2_4->AddElement(elMg, 0.002   );
    concrete_2_4->AddElement(elAl, 0.033872);
    concrete_2_4->AddElement(elSi, 0.337021);
    concrete_2_4->AddElement(elK, 0.013   );
    concrete_2_4->AddElement(elCa, 0.044   );
    concrete_2_4->AddElement(elFe, 0.014   );
    materials.insert(std::pair<G4String, G4Material *>("concrete_2.4", concrete_2_4));
    

    //Hematita Fe2 O3
    density = 5.27*g/cm3;
    G4Material* hematita = new G4Material(name="Hematita", density, ncomponents=2);
    hematita->AddElement(elFe, 2);
    hematita->AddElement(elO, 3);
    materials.insert(std::pair<G4String, G4Material *>("hematita", hematita));
    
    
    //Magnetita Fe3 O4
    density = 5.175*g/cm3;
    G4Material* magnetita = new G4Material(name="Magnetita", density, ncomponents=2);
    magnetita->AddElement(elFe, 3);
    magnetita->AddElement(elO, 4);
    materials.insert(std::pair<G4String, G4Material *>("magnetita", magnetita));
    
    //Wustita Fe O
    density = 5.75*g/cm3;
    G4Material* wustita = new G4Material(name="Wustita", density, ncomponents=2);
    wustita->AddElement(elFe, 1);
    wustita->AddElement(elO, 1);
    materials.insert(std::pair<G4String, G4Material *>("wustita", wustita));
    
    //Cascarilla 2% Hematita 4% Magnetita 94% Wustita
    density = 4.0*g/cm3;
    G4Material* cascarilla = new G4Material(name="Cascarilla", density, ncomponents=3);
    cascarilla->AddMaterial(hematita, fractionmass=0.02);
    cascarilla->AddMaterial(magnetita, fractionmass=0.04);
    cascarilla->AddMaterial(wustita, fractionmass=0.94);
    materials.insert(std::pair<G4String, G4Material *>("cascarilla", cascarilla));
    
    density = 3.50*g/cm3;
    G4Material* carbonRefractory = new G4Material(name="carbonRefractory",density,ncomponents = 1);
    carbonRefractory->AddElement(elC, fractionmass=100*perCent);
    materials.insert(std::pair<G4String, G4Material *>("carbonRefractory", carbonRefractory));

    density = 6.00*g/cm3;
    G4Material* liquidSteel = new G4Material("liquidSteel", density, ncomponents = 6);
    liquidSteel->AddElement(elC, fractionmass=0.001);
    liquidSteel->AddElement(elSi, fractionmass=0.007);
    liquidSteel->AddElement(elCr, fractionmass=0.18);
    liquidSteel->AddElement(elMn, fractionmass=0.01);
    liquidSteel->AddElement(elFe, fractionmass=0.712);
    liquidSteel->AddElement(elNi, fractionmass=0.09);
    materials.insert(std::pair<G4String, G4Material *>("liquidSteel", liquidSteel));
    
    density = 7.85*g/cm3;
    G4Material* steel = new G4Material("Steel", density, ncomponents = 14);
    steel->AddElement(elFe, fractionmass=0.98);
    steel->AddElement(elC,  fractionmass=0.001);
    steel->AddElement(elMn, fractionmass=0.007);
    steel->AddElement(elSi, fractionmass=0.002);
    
    steel->AddElement(elS,  fractionmass=0.001);
    steel->AddElement(elP,  fractionmass=0.001);
    steel->AddElement(elCu, fractionmass=0.001);
    steel->AddElement(elSn, fractionmass=0.001);
    steel->AddElement(elNi, fractionmass=0.001);
    steel->AddElement(elMo, fractionmass=0.001);
    steel->AddElement(elCr, fractionmass=0.001);
    steel->AddElement(elV,  fractionmass=0.001);
    steel->AddElement(elAl, fractionmass=0.001);
    steel->AddElement(elZn, fractionmass=0.001);
    materials.insert(std::pair<G4String, G4Material *>("steel", steel));
        
    density = 2.00*g/cm3;
    G4Material* slag = new G4Material("slag", density, ncomponents = 6);
    slag->AddElement(elC, fractionmass=0.001);
    slag->AddElement(elSi, fractionmass=0.007);
    slag->AddElement(elCr, fractionmass=0.18);
    slag->AddElement(elMn, fractionmass=0.01);
    slag->AddElement(elFe, fractionmass=0.712);
    slag->AddElement(elNi, fractionmass=0.09);
    materials.insert(std::pair<G4String, G4Material *>("slag", slag));

    density = 3.00*g/cm3;
    G4Material* scrap = new G4Material("scrap", density, ncomponents = 6);
    scrap->AddElement(elC, fractionmass=0.001);
    scrap->AddElement(elSi, fractionmass=0.007);
    scrap->AddElement(elCr, fractionmass=0.18);
    scrap->AddElement(elMn, fractionmass=0.01);
    scrap->AddElement(elFe, fractionmass=0.712);
    scrap->AddElement(elNi, fractionmass=0.09);
    materials.insert(std::pair<G4String, G4Material *>("scrap", scrap));

    density=70*kg/m3;
    G4Material* rockwool = new G4Material("rockwool", density, ncomponents = 8 );
    rockwool->AddElement(elO, fractionmass=0.481);
    rockwool->AddElement(elSi, fractionmass=0.277);
    rockwool->AddElement(elAl, fractionmass=0.081);
    rockwool->AddElement(elFe, fractionmass=0.05);
    rockwool->AddElement(elCa, fractionmass=0.036);
    rockwool->AddElement(elNa, fractionmass=0.028);
    rockwool->AddElement(elK, fractionmass=0.026);
    rockwool->AddElement(elMg, fractionmass=0.021);
    materials.insert(std::pair<G4String, G4Material *>("rockwool", rockwool));
    
    
    density = 1.000*g/cm3;
    G4int ncomp = 2;
    G4Material* water = new G4Material("Water",density,ncomp=2);
    G4int natoms;
    water->AddElement(elH, natoms=2);
    water->AddElement(elO, natoms=1);
    materials.insert(std::pair<G4String, G4Material *>("water", water));



    //Cuarzo
    density = 2.65*g/cm3;
    ncomp = 2;
    G4Material* SiO2 = new G4Material("SiO2",density,ncomp=2);
    SiO2->AddElement(elSi, natoms=1);
    SiO2->AddElement(elO, natoms=2);



    //Alumina
    density = 2.65*g/cm3;
    ncomp = 2;
    G4Material* Al2O3 = new G4Material("Al2O3",density,ncomp=2);
    Al2O3->AddElement(elAl, natoms=2);
    Al2O3->AddElement(elO, natoms=3);


    //Oxido de hierro III
    density = 5.242*g/cm3;
    ncomp = 2;
    G4Material* Fe2O3 = new G4Material("Fe2O3",density,ncomp=2);
    Fe2O3->AddElement(elFe, natoms=2);
    Fe2O3->AddElement(elO, natoms=3);


    //Oxido de manganeso
    density = 5.43*g/cm3;
    ncomp = 2;
    G4Material* MnO = new G4Material("MnO",density,ncomp=2);
    MnO->AddElement(elMn, natoms=1);
    MnO->AddElement(elO, natoms=1);


    //Oxido de magnesio
    density =3.6*g/cm3;
    ncomp = 2;
    G4Material* MgO = new G4Material("MgO",density,ncomp=2);
    MgO->AddElement(elMg, natoms=1);
    MgO->AddElement(elO, natoms=1);


    //Oxido de Calcioden
    density =3.3*g/cm3;
    ncomp = 2;
    G4Material* CaO = new G4Material("CaO",density,ncomp=2);
    CaO->AddElement(elCa, natoms=1);
    CaO->AddElement(elO, natoms=1);


    //Oxido de Sodio
    density =2.27*g/cm3;
    ncomp = 2;
    G4Material* Na2O = new G4Material("Na2O",density,ncomp=2);
    Na2O->AddElement(elNa, natoms=2);
    Na2O->AddElement(elO, natoms=1);


    //Oxido de Potasio
    density =2.13*g/cm3;
    ncomp = 2;
    G4Material* K2O = new G4Material("K2O",density,ncomp=2);
    K2O->AddElement(elK, natoms=2);
    K2O->AddElement(elO, natoms=1);


    //Oxido de Titanio
    density =4.2*g/cm3;
    ncomp = 2;
    G4Material* TiO2 = new G4Material("TiO2",density,ncomp=2);
    TiO2->AddElement(elTi, natoms=1);
    TiO2->AddElement(elO, natoms=2);


    //Oxido de Fosforo
    density =2.39*g/cm3;
    ncomp = 2;
    G4Material* P2O5 = new G4Material("P2O5",density,ncomp=2);
    P2O5->AddElement(elP, natoms=2);
    P2O5->AddElement(elO, natoms=5);

    //L.O.I. aproximado con agua
    density = 1.000*g/cm3;
    ncomp = 2;
    G4Material* LOI = new G4Material("LOI",density,ncomp=2);
    LOI->AddElement(elH, natoms=2);
    LOI->AddElement(elO, natoms=1);

    //Fiber Glass
    density = 2.5*g/cm3;
    G4Material* glassFiber = new G4Material("glassFiber", density, ncomponents = 2);
    glassFiber->AddElement(elSi, natoms=1);
    glassFiber->AddElement(elO, natoms=4);
    
    
    materials.insert(std::pair<G4String, G4Material *>("glassfiber", glassFiber));

  ///////////////////////////////////////////
  ///          Biological materials       ///
  ///////////////////////////////////////////
  //Bone Compact
  density = 1.85* g/cm3;
  G4int nElem = 0;
  G4Material* boneCompact = new G4Material( "boneCompact", density, nElem = 8 );
  boneCompact -> AddElement ( elH, 0.063984 ); //Z=1
  boneCompact -> AddElement ( elC, 0.278 ); //Z=6
  boneCompact -> AddElement ( elN, 0.027 ); //Z=7
  boneCompact -> AddElement ( elO, 0.410016 ); //Z=8
  boneCompact -> AddElement ( elMg, 0.002 ); //Z=12
  boneCompact -> AddElement ( elP, 0.07 ); //Z=15
  boneCompact -> AddElement ( elS, 0.002 ); //Z=16
  boneCompact -> AddElement ( elCa, 0.147 ); //Z=20
  
  materials.insert(std::pair<G4String, G4Material *>("boneCompact", boneCompact));

  //Bone Cortical
  density = 1.85* g/cm3;
  G4Material* boneCortical = new G4Material( "boneCortical", density, nElem = 9 );
  boneCortical -> AddElement ( elH, 0.047234 ); //Z=1
  boneCortical -> AddElement ( elC, 0.144330 ); //Z=6
  boneCortical -> AddElement ( elN, 0.041990 ); //Z=7
  boneCortical -> AddElement ( elO, 0.446096 ); //Z=8
  boneCortical -> AddElement ( elMg, 0.002200 ); //Z=12
  boneCortical -> AddElement ( elP, 0.104970 ); //Z=15
  boneCortical -> AddElement ( elS, 0.003150 ); //Z=16
  boneCortical -> AddElement ( elCa, 0.209930 ); //Z=20
  boneCortical -> AddElement ( elZn, 0.000100 ); //Z=30

  materials.insert(std::pair<G4String, G4Material *>("boneCortical", boneCortical));

  //Muscle
  density = 1.04* g/cm3;
  G4Material* muscle = new G4Material( "muscleSkeletal", density, nElem = 13 );
  muscle -> AddElement ( elH, 0.100637 ); //Z=1
  muscle -> AddElement ( elC, 0.107830 ); //Z=6
  muscle -> AddElement ( elN, 0.027680 ); //Z=7
  muscle -> AddElement ( elO, 0.754773 ); //Z=8
  muscle -> AddElement ( elNa, 0.000750 ); //Z=11
  muscle -> AddElement ( elMg, 0.000190 ); //Z=12
  muscle -> AddElement ( elP, 0.001800 ); //Z=15
  muscle -> AddElement ( elS, 0.002410 ); //Z=16
  muscle -> AddElement ( elCl, 0.000790 ); //Z=17
  muscle -> AddElement ( elK, 0.003020 ); //Z=19
  muscle -> AddElement ( elCa, 0.000030 ); //Z=20
  muscle -> AddElement ( elFe, 0.000040 ); //Z=26
  muscle -> AddElement ( elZn, 0.000050 ); //Z=30

  materials.insert(std::pair<G4String, G4Material *>("muscle", muscle));
  
  //Blood
  density = 1.06* g/cm3;
  G4Material* blood = new G4Material( "blood", density, nElem = 14 );
  blood -> AddElement (elH, 0.101866); //Z=1
  blood -> AddElement (elC, 0.100020); //Z=6
  blood -> AddElement (elN, 0.029640); //Z=7
  blood -> AddElement (elO, 0.759414); //Z=8
  blood -> AddElement (elNa, 0.001850); //Z=11
  blood -> AddElement (elMg, 0.000040); //Z=12
  blood -> AddElement (elSi, 0.000030); //Z=14
  blood -> AddElement (elP, 0.000350); //Z=15
  blood -> AddElement (elS, 0.001850); //Z=16
  blood -> AddElement (elCl, 0.002780); //Z=17
  blood -> AddElement (elK, 0.001630); //Z=19
  blood -> AddElement (elCa, 0.000060); //Z=20
  blood -> AddElement (elFe, 0.000460); //Z=26
  blood -> AddElement (elZn, 0.000010); //Z=30
  
  materials.insert(std::pair<G4String, G4Material *>("muscle", muscle));

  //Skin
  density = 1.1* g/cm3;
  G4Material* skin = new G4Material( "skin", density, nElem = 13 );
  skin -> AddElement (elH, 0.100588); //Z=1
  skin -> AddElement (elC, 0.228250); //Z=6
  skin -> AddElement (elN, 0.046420); //Z=7
  skin -> AddElement (elO, 0.619002); //Z=8
  skin -> AddElement (elNa, 0.000070); //Z=11
  skin -> AddElement (elMg, 0.000060); //Z=12
  skin -> AddElement (elP, 0.000330); //Z=15
  skin -> AddElement (elS, 0.001590); //Z=16
  skin -> AddElement (elCl, 0.002670); //Z=17
  skin -> AddElement (elK, 0.000850); //Z=19
  skin -> AddElement (elCa, 0.000150); //Z=20
  skin -> AddElement (elFe, 0.000010); //Z=26
  skin -> AddElement (elZn, 0.000010); //Z=30

  materials.insert(std::pair<G4String, G4Material *>("skin", skin));
  
  //AdiposeTissue
  density = 0.92* g/cm3;
  G4Material* adiposeTissue = new G4Material( "adiposeTissue", density, nElem = 13 );
  adiposeTissue -> AddElement (elH, 0.119477); //Z=1
  adiposeTissue -> AddElement (elC, 0.637240); //Z=6
  adiposeTissue -> AddElement (elN, 0.007970); //Z=7
  adiposeTissue -> AddElement (elO, 0.232333); //Z=8
  adiposeTissue -> AddElement (elNa, 0.000500); //Z=11
  adiposeTissue -> AddElement (elMg, 0.000020); //Z=12
  adiposeTissue -> AddElement (elP, 0.000160); //Z=15
  adiposeTissue -> AddElement (elS, 0.000730); //Z=16
  adiposeTissue -> AddElement (elCl, 0.001190); //Z=17
  adiposeTissue -> AddElement (elK, 0.000320); //Z=19
  adiposeTissue -> AddElement (elCa, 0.000020); //Z=20
  adiposeTissue -> AddElement (elFe, 0.000020); //Z=26
  adiposeTissue -> AddElement (elZn, 0.000020); //Z=30
  
  materials.insert(std::pair<G4String, G4Material *>("adiposeTissue", adiposeTissue));

}
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
