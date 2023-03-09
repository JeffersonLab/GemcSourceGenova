// G4 headers
#include "G4Poisson.hh"
#include "Randomize.hh"

// gemc headers
#include "poker_hcal_hitprocess.h"

// CLHEP units
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

map<string, double> poker_hcal_HitProcess::integrateDgt(MHit* aHit, int hitn) {
	map<string, double> dgtz;
	vector<identifier> identity = aHit->GetId();

	int sector = identity[0].id;
    int zch = identity[1].id;

	vector<G4ThreeVector> Lpos = aHit->GetLPos();
	vector<G4double> Edep = aHit->GetEdep();
	vector<G4double> Dx = aHit->GetDx();

    double Etot = 0;

	for (unsigned int s = 0; s < Edep.size(); s++) {
		Etot = Etot + Edep[s];
	}
	dgtz["hitn"] = hitn;
	dgtz["sector"] = sector;
	dgtz["channel"] = zch;
	dgtz["dene"] = Etot;	  //


	return dgtz;
}

vector<identifier> poker_hcal_HitProcess::processID(vector<identifier> id, G4Step *step, detector Detector) {
	id[id.size() - 1].id_sharing = 1;
	return id;
}

map<string, vector<int> > poker_hcal_HitProcess::multiDgt(MHit* aHit, int hitn) {
    map<string, vector<int> > MH;

    return MH;
}

// - electronicNoise: returns a vector of hits generated / by electronics.
vector<MHit*> poker_hcal_HitProcess::electronicNoise() {
    vector<MHit*> noiseHits;

    // first, identify the cells that would have electronic noise
    // then instantiate hit with energy E, time T, identifier IDF:
    //
    // MHit* thisNoiseHit = new MHit(E, T, IDF, pid);

    // push to noiseHits collection:
    // noiseHits.push_back(thisNoiseHit)

    return noiseHits;
}

// - charge: returns charge/time digitized information / step
map<int, vector<double> > poker_hcal_HitProcess::chargeTime(MHit* aHit, int hitn) {
    map<int, vector<double> > CT;

    return CT;
}

// - voltage: returns a voltage value for a given time. The inputs are:
// charge value (coming from chargeAtElectronics)
// time (coming from timeAtElectronics)
double poker_hcal_HitProcess::voltage(double charge, double time, double forTime) {
    return 0.0;
}



