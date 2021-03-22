// G4 headers
#include "G4ParticleTypes.hh"

// gemc headers
#include "MSteppingAction.h"
#include "MEventAction.h"

MSteppingAction::MSteppingAction(goptions Opt) {
	gemcOpt = Opt;
	energyCut = gemcOpt.optMap["ENERGY_CUT"].arg;
	max_x_pos = gemcOpt.optMap["MAX_X_POS"].arg;
	max_y_pos = gemcOpt.optMap["MAX_Y_POS"].arg;
	max_z_pos = gemcOpt.optMap["MAX_Z_POS"].arg;
	evt_action = 0;

	//get here the relevant parameters for the jpos trigger.

}

MSteppingAction::~MSteppingAction() {
	cout << " > Closing Stepping Action." << endl;
}

void MSteppingAction::UserSteppingAction(const G4Step *aStep) {
	G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();      ///< Global Coordinates of interaction
	G4Track *track = aStep->GetTrack();

	if (fabs(pos.x()) > max_x_pos || fabs(pos.y()) > max_y_pos || fabs(pos.z()) > max_z_pos) track->SetTrackStatus(fStopAndKill);   ///< Killing track if outside of interest region

	if (track->GetKineticEnergy() < energyCut) track->SetTrackStatus(fStopAndKill);

	// Anything passing material "Kryptonite" is killed
	if (track->GetMaterial()->GetName() == "Kryptonite") {
		track->SetTrackStatus(fStopAndKill);
	}

	// Anything passing material "KryptoniteLight" and is a primary and is in the first step is killed
	if ((track->GetMaterial()->GetName() == "KryptoniteLight") && (track->GetParentID() == 0) && (aStep->GetPreStepPoint()->GetGlobalTime() == 0)) {
		track->SetTrackStatus(fKillTrackAndSecondaries);
	}

	if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
		// killing photon if above 100 steps
		// notice we rarely go above 20 steps for all normal CC detectors
		if (track->GetCurrentStepNumber() > 100) track->SetTrackStatus(fStopAndKill);

		if (track->GetLogicalVolumeAtVertex()->GetMaterial()->GetName() == "SemiMirror") track->SetTrackStatus(fStopAndKill);
	}

	// limiting steps in one volume to 10000
	// it may be the new version of geant4, or
	// accurate magnetic fields, but it does happen that sometimes
	// a track get stuck into a magnetic field infinite loop
	if (track->GetCurrentStepNumber() > 10000) track->SetTrackStatus(fStopAndKill);

	//JPOS_CRS part
	if (evt_action->do_JPOS_TRG) {
		if (aStep->GetPreStepPoint()->GetSensitiveDetector() != 0) {
			G4VSensitiveDetector *SD = aStep->GetPreStepPoint()->GetSensitiveDetector();
			  if (((string)SD->GetName()) == evt_action->SDprompt) {
				if ((aStep->GetPreStepPoint()->GetGlobalTime() > evt_action->Tprompt_MIN) && (aStep->GetPreStepPoint()->GetGlobalTime() < evt_action->Tprompt_MAX)) {
					evt_action->Eprompt += aStep->GetTotalEnergyDeposit();
				}
			}
		}
	}

	//JPOS_CRS part. If a step is at the boundary of two volumes A(exiting from) and B (entering into):
	//The PRE_STEP_POINT is in A
	//The POST_STEP_POINT is in B
	//Here I check all particles EXITING from A and ENTERING B, by the name of the sensitive detector of the exit-from volum
	if (evt_action->do_JPOS_TRG_2) {
		if (aStep->GetPreStepPoint()->GetPhysicalVolume() != aStep->GetPostStepPoint()->GetPhysicalVolume()) {
			if (aStep->GetPreStepPoint()->GetSensitiveDetector() != 0) {
				G4VSensitiveDetector *SD = aStep->GetPreStepPoint()->GetSensitiveDetector();
				if (((string)SD->GetName()) == evt_action->SDprompt_2) {
					if ((aStep->GetPreStepPoint()->GetGlobalTime() >= evt_action->Tprompt_MIN) && (aStep->GetPreStepPoint()->GetGlobalTime() <= evt_action->Tprompt_MAX) && (track->GetParentID()!=0)) {
						evt_action->Eprompt_2 += track->GetKineticEnergy();
					}
				}
			}
		}
	}

//	// checking if a step is stuck in the same position
//	// for more than 10 steps
//    // this should be revisited
//	if(sqrt( (pos - oldpos).x() *  (pos - oldpos).x() +
//			 (pos - oldpos).y() *  (pos - oldpos).y() +
//			 (pos - oldpos).z() *  (pos - oldpos).z() ) < 0.0000001*mm)
//		
//	{
//		nsame++;
//		if(nsame > 100)
//		{
//			cout << " Track is stuck. PID: " <<  track->GetDefinition()->GetPDGEncoding() << " Volume: " << volname ;
//			cout << " Last step at :  " << pos << " old step was at " << oldpos <<  "    track id: " << track->GetTrackID() << endl;
//			
//			cout << ". Killing this track. " << endl;
//			
//			track->SetTrackStatus(fStopAndKill);
//			
//		}
//	}
//	else
//		nsame = 0;
//	
//	oldpos = pos;

	// limiting steps in one volume to 1000
	// int nsteps = aStep->GetTrack()->GetCurrentStepNumber();
	//if(nsteps >= 1000) aStep->GetTrack()->SetTrackStatus(fStopAndKill);

}

