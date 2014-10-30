// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2008 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. It fits on one slide in a talk. 
// It studies the charged multiplicity distribution at the LHC.

#include "Pythia.h"
#include <stdio.h>
#include <stdlib.h>

#include <vector> 
#include "TTree.h"
#include "TFile.h"

using namespace Pythia8;
using namespace std;
 //----------------------------------------------------------------------
/// a function that pretty prints a list of jets

int main(int argc, char* argv[]) {
  TString FileName="outputJ3_";
  Long64_t ifile=0;
  //Booking the tree
  TFile *myfile;
  TTree *theTree;

  int Nentry;
  int Npartons;
  int Nparticles;
  int ID[10000];
  int Stat[10000];
  double Charge[10000];
  double Px[10000];
  double Py[10000];
  double Pz[10000];
  double P0[10000];
  double Pm[10000];
  double Pt[10000];
  double Rap[10000];
  double Phi[10000];
  double Eta[10000];


  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  //  pythia.readString("PartonLevel:MI = on");
  //  pythia.readString("PartonLevel:ISR = on");
  //  pythia.readString("PartonLevel:FSR = on"); 
  //  pythia.readString("HadronLevel:Hadronize = on");
  pythia.readString("HardQCD:All = on");
  pythia.readString("PhaseSpace:pTHatMin = 70.0");
  pythia.readString("PhaseSpace:pTHatMax = 140.0");    
pythia.readString("Tune:pp = 5");
  //J0  0 17 
  //J1  17 35 
  //J2  35 70 
  //J3  70 140 
  //J4  140 280 
  //J5  280 560 
  //J6  560 1120 
  //J7  1120 2240
  //J8  2240 inf
  pythia.init( 2212, 2212, 7000.);

  // Begin event loop. Generate event. Skip if error. List first one.
  int totevent;
  if (argc>1)totevent=atoi(argv[1]);
  else totevent=100;
  cout << "Total Number of events: "<<totevent <<endl;
  int iEvent=0;
  while (iEvent < totevent){
    if (!pythia.next()) continue;
    if (iEvent < 1) {pythia.info.list(); pythia.event.list();}   
    if (iEvent%50000==0) {
      if (iEvent!=0){
	cout <<"Wrinting the ntuple"<<endl;
	theTree->Write();
	cout <<"Closing the file N = "<<ifile-1<<endl;
	myfile->Close();
	cout <<"Deleting the tree"<<endl;
	//delete(theTree);
	cout <<"Deleting the file"<<endl;
	//delete(myfile);
      }
      cout <<"Creating  the file N = "<<ifile<<endl;
      myfile=new TFile(FileName+ifile+".root","RECREATE");
      cout <<"Creating the ntuple"<<endl;
      theTree=new TTree("h76","myAnalysis root tree", 1);
      cout <<"Setting the branchs"<<endl;
      theTree->Branch ("Nentry",  &Nentry, "Nentry/I");
      theTree->Branch ("Npartons",   &Npartons,  "Npartons/I");
      theTree->Branch ("Nparticles",   &Nparticles,  "Nparticles/I");
      theTree->Branch ("ID",      ID,      "ID[Nentry]/I");
      theTree->Branch ("Stat",    Stat,    "Stat[Nentry]/I");
      theTree->Branch ("Charge",      Charge,      "Charge[Nentry]/D");
      theTree->Branch ("Px",      Px,      "Px[Nentry]/D");
      theTree->Branch ("Py",      Py,      "Py[Nentry]/D");
      theTree->Branch ("Pz",      Pz,      "Pz[Nentry]/D");
      theTree->Branch ("P0",      P0,      "P0[Nentry]/D");
      theTree->Branch ("Pm",      Pm,      "Pm[Nentry]/D");
      theTree->Branch ("Pt",      Pt,      "Pt[Nentry]/D");       
      theTree->Branch ("Rap",     Rap,     "Rap[Nentry]/D");       
      theTree->Branch ("Phi",     Phi,     "Phi[Nentry]/D");       
      theTree->Branch ("Eta",     Eta,     "Eta[Nentry]/D");  
      ifile++;
    }
   
    Nentry=0;
    Npartons=0;
    Nparticles=0;
    
    for (int i=0;i<pythia.event.size();i++){
      if (pythia.event[i].status()==-21||
	  pythia.event[i].status()==-23){
	if (pythia.event[i].status()==-21){
	  Stat[Npartons] = -2;
	}
	if (pythia.event[i].status()==-23){
	  Stat[Npartons] = -1;
	}
      
	ID[Npartons] = pythia.event[i].id();
        Charge[Npartons]=pythia.event[i].charge();
	Px[Npartons]=pythia.event[i].px();
	Py[Npartons]=pythia.event[i].py();
	Pz[Npartons]=pythia.event[i].pz();
	P0[Npartons]=pythia.event[i].e();
	Pm[Npartons]=pythia.event[i].m();
        Pt[Npartons]=pythia.event[i].pT();
        Rap[Npartons]=pythia.event[i].y();
        Phi[Npartons]=pythia.event[i].phi();
        Eta[Npartons]=pythia.event[i].eta();

	Npartons++;
      }
    }
    for (int i=0;i<pythia.event.size();i++){	
      if (pythia.event[i].isFinal()){
	Stat[Npartons+Nparticles] = 2;
	ID[Npartons+Nparticles] = pythia.event[i].id();
	Charge[Npartons+Nparticles]=pythia.event[i].charge();
	Px[Npartons+Nparticles]=pythia.event[i].px();
	Py[Npartons+Nparticles]=pythia.event[i].py();
	Pz[Npartons+Nparticles]=pythia.event[i].pz();
	P0[Npartons+Nparticles]=pythia.event[i].e();
	Pm[Npartons+Nparticles]=pythia.event[i].m();
	Pt[Npartons+Nparticles]=pythia.event[i].pT();
	Rap[Npartons+Nparticles]=pythia.event[i].y();
	Phi[Npartons+Nparticles]=pythia.event[i].phi();
	Eta[Npartons+Nparticles]=pythia.event[i].eta();

	Nparticles++;
      }
    }
    Nentry=Npartons+Nparticles;
    if (iEvent%100==0)    cout << iEvent<<endl;
    theTree->Fill();
    iEvent=iEvent+1;   
  }
  theTree->Write();
  myfile->Close();
  pythia.statistics();
  return 0;
}
