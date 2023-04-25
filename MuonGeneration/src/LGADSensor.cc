#include "LGADSensor.hh"
#include "LGADSensorHit.hh"
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
LGADSensor::LGADSensor(G4String name, G4String collection)
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
LGADSensor::~LGADSensor() {
    ;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
void LGADSensor::Initialize(G4HCofThisEvent*HCE)
{
    hitsCollection = new LGADSensorHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
    HCE->AddHitsCollection(HCID,hitsCollection);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Process the hits and fills the relevant information                  //
//----------------------------------------------------------------------//
G4bool LGADSensor::ProcessHits(G4Step*aStep,G4TouchableHistory*  /*ROhist*/) {

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
    LGADSensorHit* aHit = new LGADSensorHit(copyNo);
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
// Setting the structure                                                //
//----------------------------------------------------------------------//
void LGADSensor::SetStructure(Detector *a) {
    det = a;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Needed by the parent class                                           //
//----------------------------------------------------------------------//
void LGADSensor::EndOfEvent(G4HCofThisEvent* /*HCE*/) {
    ;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

