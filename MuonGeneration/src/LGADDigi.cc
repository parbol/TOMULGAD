#include "LGADDigi.hh"

//----------------------------------------------------------------------//
// Constructor                                                          //
//----------------------------------------------------------------------//
LGADDigi::LGADDigi(LGADSensorHit *h, ConfigurationGeometry *g) {
    
    G4int det = h->GetDetectorID();
    G4int layer = h->GetLayerID();
    G4int lgad = h->GetLGADId();
    G4ThreeVector size = g->getDetector(det)->GetLayer(layer)->GetSensor(lgad)->getSizes();
    G4float sizeX = size.x();
    G4float sizeY = size.y();
    G4int npadsX = g->getDetector(det)->GetLayer(layer)->GetSensor(lgad)->npadx();
     
    G4int padx = GetPadx(det, layer, lgad, h, g);
    G4int pady = GetPady(h, g);
    G4int detS = det << 19;
    G4int layerS = layer << 14;
    G4int lgadS = lgad << 8;
    G4int padxS = padx << 4;
    G4int padyS = pady;
    hitID = detS | layerS | lagS | padxS | padyS;

};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
// GetPadX                                                          //
//----------------------------------------------------------------------//
G4int LGADDigi::GetPadx(LGADSensorHit *h, ConfigurationGeometry *g) {

  G4ThreeVector localpos = h->GetLocalPos();

                    G4double e = aHit->GetEnergy();  

}

