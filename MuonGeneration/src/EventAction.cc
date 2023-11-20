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
#include <map>

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

    std::map<G4int, LGADDigi*> digis;

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
                    LGADDigi *digi = new LGADDigi(aHit);
                    auto it = digis.find(digi->hitID);
                    if(it == digis.end()) {
                        G4cout << "Inserting hit" << G4endl;
                        digis.insert(std::make_pair(digi->hitID, digi));
                    } else {
                        G4cout << "Adding up hit" << G4endl;
                        it->second->charge += digi->charge;
                        it->second->TOA = (it->second->TOA < digi->TOA) ? it->second->TOA : digi->TOA;
                    }    
                }
                    
            }
        }
    }
    
    for(auto i = digis.begin(); i != digis.end(); ++i) {
        if(i->second->Digitize()) {     
            man->FillNtupleIColumn(0, i->second->eventNumber);
            man->FillNtupleIColumn(1, i->second->GetDet());
            man->FillNtupleIColumn(2, i->second->GetLayer());
            man->FillNtupleIColumn(3, i->second->GetLGAD());
            man->FillNtupleIColumn(4, i->second->GetPadx());
            man->FillNtupleIColumn(5, i->second->GetPady());
            man->FillNtupleDColumn(6, i->second->TOA/CLHEP::second);
            man->FillNtupleDColumn(7, i->second->TOT/CLHEP::second);
            man->FillNtupleDColumn(8, i->second->charge);
            man->AddNtupleRow(); 
        }
    }
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
