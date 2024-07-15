#include <iomanip>
#include "PrimaryGeneratorAction.hh"
#include <sstream>
#include <sys/time.h>
#include "G4Event.hh"

using namespace std;



//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
PrimaryGeneratorAction::PrimaryGeneratorAction(ConfigurationGeometry *myGeom_, G4long randomSeed_, G4double pt_) {

    myGeom = myGeom_;
    randomSeed = randomSeed_;
    pt = pt_;
    zcry = (myGeom->getZOffsetCRY()/CLHEP::cm)/100.0;
    gen = new EcoMug();
    gen->SetUseHSphere(); 
    // The radious will be a fraction k of 1/4 * sqrt(Lx^2 +Lz^2)
    gen->SetHSphereRadius(0.35); //This is in meters
    gen->SetHSphereCenterPosition({{-120.0, 0., 1.5}}); //This is in meters
    gen->SetSeed(randomSeed);

    particleGun = new G4ParticleGun();
    timeSimulated=0.0;

    G4String particleName;

    // Create the table containing all particle names
    particleTable = G4ParticleTable::GetParticleTable();
    fMuonPlus = particleTable->FindParticle(particleName="mu+");
    fMuonMinus = particleTable->FindParticle(particleName="mu-");
    particleGun->SetParticleDefinition(fMuonMinus);
    particleGun->SetParticleMomentum(0.0);
    // Create the messenger file
    gunMessenger = new PrimaryGeneratorMessenger(this);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
// Destructuor                                                          //
//----------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Real generation of the primaries                                     //
//----------------------------------------------------------------------//
std::array<G4double, 6> PrimaryGeneratorAction::getProjection(std::array<G4double, 3> p, G4double phi, G4double theta) {

    std::array<G4double, 6> v;
    G4double vx = cos(phi) * sin(theta);
    G4double vy = sin(phi) * sin(theta);
    G4double vz = cos(theta);

    G4double l = (zcry - p[2])/vz;

    G4double x = p[0] + l * vx;
    G4double y = p[1] + l * vy;
    G4double z = zcry;
    
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = vx;
    v[4] = vy;
    v[5] = vz;

    return v;

}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// Real generation of the primaries                                     //
//----------------------------------------------------------------------//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

        
    // The array storing muon generation position
    std::array<G4double, 3> muon_position;
    gen->Generate();
    muon_position = gen->GetGenerationPosition();
    G4double muon_p = gen->GetGenerationMomentum();
    G4double muon_theta = gen->GetGenerationTheta();
    G4double muon_phi = gen->GetGenerationPhi();
    G4double muon_charge = gen->GetCharge();
    if (muon_charge > 0.0) {
        particleGun->SetParticleDefinition(fMuonPlus);
    } else {
        particleGun->SetParticleDefinition(fMuonMinus);
    }

    std::array<G4double, 6> muon_projection = getProjection(muon_position, muon_phi, muon_theta);
    particleGun->SetParticlePosition(G4ThreeVector(muon_projection[0]*CLHEP::m, muon_projection[1]*CLHEP::m, muon_projection[2]*CLHEP::m));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(muon_projection[3], muon_projection[4], muon_projection[5]));
    particleGun->SetParticleTime(0.0);
    particleGun->SetParticleCharge(muon_charge);
    particleGun->SetParticleMomentum(muon_p * CLHEP::GeV);

    particleGun->GeneratePrimaryVertex(anEvent);

}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

