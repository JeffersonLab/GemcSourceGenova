/*
 * G4Scalar.cc
 *
 *  Created on: May 6, 2018
 *      Author: celentan
 */

#include "G4Scalar.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"



// ######################################################################
// ###                         G4Scalar                               ###
// ######################################################################

G4Scalar* G4Scalar::theInstance = 0;
G4Scalar* G4Scalar::Definition(G4double mass)
{
  if (theInstance !=0) return theInstance;
  const G4String name = "scalar";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  anInstance = new G4ParticleDefinition(
                 name, mass, 0.0*MeV, 0,
		    0,               0,                0,
		    0,               0,                0,
	     "boson",            0,                0,        0,
		true,     -1,             NULL,
             false,           "scalar"
              );


  }
  theInstance = reinterpret_cast<G4Scalar*>(anInstance);
  return theInstance;
}

G4Scalar*  G4Scalar::ScalarDefinition(G4double mass)
{
  return Definition(mass);
}

G4Scalar*  G4Scalar::Scalar(G4double mass)
{
  return Definition(mass);
}
