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

    for(auto i : geom->collections()) {
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
            DHCs.push_back(CHCcoll);
        }
        for (auto i : DHCs) {
            if(i.at(0)) {
                G4int n_hit = i.at(0)->entries();
                for(G4int hit = 0; hit < n_hit; hit++) {
                    LGADSensorHit* aHit = (*(i.at(0)))[hit];
                    G4ThreeVector pos = aHit->GetLocalPos();
                    G4double e = aHit->GetEnergy();
                    G4int layerID = aHit->GetLayerID();


                man->FillNtupleDColumn(8*layerID + 0, pos.x()/CLHEP::cm);
                man->FillNtupleDColumn(8*layerID + 1, pos.y()/CLHEP::cm);
                man->FillNtupleDColumn(8*layerID + 2, pos.z()/CLHEP::cm);
                man->FillNtupleDColumn(8*layerID + 3, meas.x()/CLHEP::cm);
                man->FillNtupleDColumn(8*layerID + 4, meas.y()/CLHEP::cm);
                man->FillNtupleDColumn(8*layerID + 5, meas.z()/CLHEP::cm);
                man->FillNtupleIColumn(8*layerID + 6, layerID);
                man->FillNtupleDColumn(8*layerID + 7, e/CLHEP::MeV);
            }
        }

        if(DHC2.at(0)) {
            G4int n_hit = DHC2.at(0)->entries();
            for(G4int hit = 0; hit < n_hit; hit++) {
                LGADSensorHit* aHit = (*(DHC2.at(0)))[hit];
                G4ThreeVector pos = aHit->GetLocalPos();
                G4ThreeVector meas = aHit->GetLocalMeas();
                G4ThreeVector errormeas = aHit->GetLocalMeasError();
                G4double e = aHit->GetEnergy();
                G4int layerID = aHit->GetLayerID();
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 0, pos.x()/CLHEP::cm);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 1, pos.y()/CLHEP::cm);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 2, pos.z()/CLHEP::cm);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 3, meas.x()/CLHEP::cm);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 4, meas.y()/CLHEP::cm);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 5, meas.z()/CLHEP::cm);
                man->FillNtupleIColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 6, layerID);
                man->FillNtupleDColumn(geom->getDetector1()->getNLayers() * 8 + 8*layerID + 7, e/CLHEP::MeV);
            }
        }
        if(DHC1.at(0)) man->AddNtupleRow();
    }

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
