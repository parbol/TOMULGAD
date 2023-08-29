#include "EventAction.hh"
#include "EventActionMessenger.hh"

#include "G4AnalysisManager.hh"

#include "Analysis.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "LGADSensorHit.hh"

#include "ConfigurationGeometry.hh"

#include <math.h>

#include <iostream>

#define PI 3.141592653589793238462643383279502884



//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
EventAction::EventAction(ConfigurationGeometry *geom_) {

    geom = geom_;
    G4String colName;
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4int numberOfDetectors = SDman->GetCollectionCapacity();
    SDman->ListTree();

    for(auto i : geom->collections) {
        DHCID.push_back(SDman->GetCollectionID(i));
    }
    messenger = new EventActionMessenger(this);

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Destructor                                                           //
//----------------------------------------------------------------------//
EventAction::~EventAction() {
    delete messenger;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//



//----------------------------------------------------------------------//
// Begin of the event                                                   //
//----------------------------------------------------------------------//
void EventAction::BeginOfEventAction(const G4Event*) {}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// End of the event -> the action goes here                             //
//----------------------------------------------------------------------//
void EventAction::EndOfEventAction(const G4Event* evt) {

    auto man = G4AnalysisManager::Instance();
    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
    
    std::vector<LGADSensorHitsCollections> DHCs;
    //G4SDManager* SDman = G4SDManager::GetSDMpointer();

    if(HCE) {
        for (auto i : DHCID) {
            LGADSensorHitsCollection *a = 0;
            a = (LGADSensorHitsCollection*)(HCE->GetHC(i));
            LGADSensorHitsCollections DHCcoll;
            DHCcoll.push_back(a);
            DHCs.push_back(DHCcoll);
        }
        for (auto i : DHCs) {
            if(i.at(0)) {
                G4int n_hit = i.at(0)->entries();
                for(G4int hit = 0; hit < n_hit; hit++) {
                    LGADSensorHit* aHit = (*(i.at(0)))[hit];
                    G4ThreeVector localpos = aHit->GetLocalPos();
                    G4ThreeVector globalpos = aHit->GetGlobalPos();
                    G4double e = aHit->GetEnergy();
                    G4int detectorID = aHit->GetDetectorID();
                    G4int layerID = aHit->GetLayerID();
                    G4int lgadID = aHit->GetLGADID();
                    
                    man->FillNtupleIColumn(0, 1, aHit->GetEventNumber());
                    man->FillNtupleIColumn(1, 1, aHit->GetDetectorID());
                    man->FillNtupleIColumn(2, 1, aHit->GetLayerID());
                    man->FillNtupleIColumn(3, 1, aHit->GetLGADID());
                    man->FillNtupleIColumn(4, 1, aHit->GetPadx());
                    man->FillNtupleIColumn(5, 1, aHit->GetPady());
                    man->FillNtupleDColumn(6, 1, aHit->GetTOA()/CLHEP::second);                                        man->FillNtupleDColumn(6, 1, aHit->GetTOA()/CLHEP::second);
                    man->FillNtupleDColumn(7, 1, aHit->GetTOT()/CLHEP::second);
                    man->FillNtupleDColumn(8, 1, aHit->GetTime()/CLHEP::second);
                    man->FillNtupleDColumn(9, 1, aHit->GetEnergy()/CLHEP::eV);
                    man->FillNtupleDColumn(10, 1, localpos.x()/CLHEP::cm);
                    man->FillNtupleDColumn(11, 1, localpos.y()/CLHEP::cm);
                    man->FillNtupleDColumn(12, 1, localpos.z()/CLHEP::cm);
                    man->FillNtupleDColumn(13, 1, globalpos.x()/CLHEP::cm);
                    man->FillNtupleDColumn(14, 1, globalpos.y()/CLHEP::cm);
                    man->FillNtupleDColumn(15, 1, globalpos.z()/CLHEP::cm);
                }
                man->AddNtupleRow();    
            }
        }
    }
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
