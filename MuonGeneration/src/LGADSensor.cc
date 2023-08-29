#include "LGADSensor.hh"
#include "LGADSensorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
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

    G4Event *event = G4EventManager::GetEventManager()->GetEventManager()->GetNonconstCurrentEvent();
    if(event == NULL) false;
    G4int enumber = event->GetEventID();
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHistory* theTouchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  
    G4ThreeVector worldPos = preStepPoint->GetPosition();
    G4ThreeVector localPos = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
    G4int detector = lgad->detId();
    G4int layer = lgad->layerId();
    G4int sensor = lgad->sensorId();
    G4int xpad = 0;
    G4int ypad = 0;
    G4double energy = preStepPoint->GetTotalEnergy();

    //Simulating resolution
    LGADSensorHit* aHit = new LGADSensorHit();
    aHit->SetEventNumber(enumber);
    aHit->SetDetectorID(detector);
    aHit->SetLayerID(layer);
    aHit->SetLGADID(sensor);
    aHit->SetLocalPos(localPos);
    aHit->SetGlobalPos(worldPos);
    aHit->SetTime(preStepPoint->GetGlobalTime());
    aHit->SetTOA(0);
    aHit->SetTOT(0);
    aHit->SetPadx(0);
    aHit->SetPady(0);
    aHit->SetEnergy(energy);
    
    hitsCollection->insert(aHit);

    return true;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Set lgad                                                             //
//----------------------------------------------------------------------//
void LGADSensor::setLGAD(LGAD *a) {
    lgad = a;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Get lgad                                                             //
//----------------------------------------------------------------------//
LGAD * LGADSensor::getLGAD() {
    return lgad;
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

