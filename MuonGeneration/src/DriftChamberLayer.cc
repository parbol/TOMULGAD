#include "DriftChamberLayer.hh"
#include "DriftChamberLayerHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
DriftChamberLayer::DriftChamberLayer(G4String name, G4String collection)
    :G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(collection);
    HCID = -1;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
DriftChamberLayer::~DriftChamberLayer() {
    ;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
void DriftChamberLayer::Initialize(G4HCofThisEvent*HCE)
{
    hitsCollection = new DriftChamberLayerHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
    HCE->AddHitsCollection(HCID,hitsCollection);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Process the hits and fills the relevant information                  //
//----------------------------------------------------------------------//
G4bool DriftChamberLayer::ProcessHits(G4Step*aStep,G4TouchableHistory*  /*ROhist*/) {

    G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
    if(charge==0.) return true;
    G4int type = abs(aStep->GetTrack()->GetDefinition()->GetPDGEncoding());
    if(type != 13) return true;


    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4VPhysicalVolume* theMotherPhysical = theTouchable->GetVolume(1); // mother
    G4VPhysicalVolume* thePhysical = theTouchable->GetVolume(0); // child
    G4int copyNo = thePhysical->GetCopyNo();

    //Simulating efficiency
    if(CLHEP::RandFlat::shoot(0.0, 1.0) > det->getEffLayer(copyNo)) return false;

    G4ThreeVector worldPos = preStepPoint->GetPosition();
    G4ThreeVector ChamberPos = det->toLocal(worldPos);
    G4ThreeVector LayerPos = det->toLocal(ChamberPos, copyNo);
    G4double energy = preStepPoint->GetTotalEnergy();

    std::vector<G4double> pair = getWirePos(LayerPos.x(), copyNo);
    if(pair[1] < 0) return false;
    G4ThreeVector LayerMeas(pair[0], 0, 0);
    G4ThreeVector ChamberMeas = det->toGlobal(LayerMeas, copyNo);

    //Simulating resolution
    DriftChamberLayerHit* aHit = new DriftChamberLayerHit(copyNo);
    aHit->SetLocalPos(ChamberPos);
    aHit->SetLocalMeas(ChamberMeas);
    aHit->SetTime(preStepPoint->GetGlobalTime());
    aHit->SetEnergy(energy);
    aHit->SetLayerID(copyNo);

    hitsCollection->insert(aHit);

    return true;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Get the position of the closest wire                                 //
//----------------------------------------------------------------------//
std::vector<G4double> DriftChamberLayer::getWirePos(G4double w, G4int a) {

    std::vector<G4double> val;
    G4int closestWire = -1;
    G4double minimumSoFar = 20*CLHEP::m;
    std::vector<G4double> wires = (det->getWires())[a];
    for(G4int i = 0; i < wires.size(); i++) {
        G4double dist = fabs(wires[i] - w);
        if(dist < minimumSoFar) {
            minimumSoFar = dist;
            closestWire = i;
        }
    }
    if(closestWire == 0 || closestWire == wires.size()-1) {
	    val.push_back(-999);
	    val.push_back(-999);
    }
    val.push_back(wires[closestWire]);
    if(closestWire == wires.size()-1) {
        val.push_back(fabs(wires[wires.size()-1]-wires[wires.size()-2]));
    } else {
        val.push_back(fabs(wires[closestWire+1]-wires[closestWire]));
    }
    return val;

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Setting the resolution of the hit                                    //
//----------------------------------------------------------------------//
void DriftChamberLayer::SetStructure(Detector *a) {
    det = a;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Needed by the parent class                                           //
//----------------------------------------------------------------------//
void DriftChamberLayer::EndOfEvent(G4HCofThisEvent* /*HCE*/) {
    ;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

