// this is an attempt at merging all of Paolo's code in a single
// one. Since most of teh generation is common excpet for te part that
// specifies the process and the number of events, we just read this
// from the command line
//
// Usage:
//  
//   ./ prod_pythia8_generic <process> [<nevts>] [<outputdir>]
//
// where <process> can be one of
//   dijets200
//   dijets450
//   ttbar200  [Note: includes also single top at teh moment]
//   ttbar450  [Note: includes also single top at teh moment]
//   Dib
//   DibBoost
//   Z
//   ZBoost
//   ZH
//   ZHBoost

#include "Pythia.h"
#include <stdio.h>
#include <stdlib.h>

#include <vector> 
#include "TTree.h"
#include "TFile.h"

#include "make_b_stable.hh"

using namespace Pythia8;
using namespace std;
 //----------------------------------------------------------------------
/// a function that pretty prints a list of jets

int main(int argc, char* argv[]) {
  if (argc<=1){
    cerr << "Usage:" << endl << endl << "   ./ prod_pythia8_generic <process> [<nevts>]" << endl << endl;
    exit(1);
  }

  TString FileName;
  Long64_t ifile=0;
  //Booking the tree
  TFile *myfile;
  TTree *theTree;

  int Nentry;
  int Npartons;
  int Nparticles;
  int ID[10000];
  int Stat[10000];
  float Charge[10000];
  float Pm[10000];
  float Pt[10000];
  float Rap[10000];
  float Phi[10000];

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia(PY8XMLDOC);

  TString procname = argv[1];
  TString odir=".";
  if (argc>=4) odir = argv[3];

  if (procname == TString("dijets200")){
    FileName=odir+TString("/outputJ4_");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
  } else if (procname == TString("dijets450")){
    FileName=odir+TString("/outputJ5_");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
  } else if (procname == TString("ttbar200")){
    FileName=odir+TString("/outputT4_");
    pythia.readString("Top:all = on");
    //// for just the ttbar part
    //pythia.readString("Top:gg2ttbar = on");
    //pythia.readString("Top:qqbar2ttbar = on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
  } else if (procname == TString("ttbar450")){
    FileName=odir+TString("/outputT5_");
    pythia.readString("Top:All = on");
    //// for just the ttbar part
    //pythia.readString("Top:gg2ttbar = on");
    //pythia.readString("Top:qqbar2ttbar = on");
    pythia.readString("PhaseSpace:pTHatMin = 450.0");
  } else if (procname == TString("Dib")){
    FileName=odir+TString("/outputDib_");
    pythia.readString("WeakDoubleBoson:all= on");
  } else if (procname == TString("DibBoost")){
    FileName=odir+TString("/outputDibBoost_");
    pythia.readString("WeakDoubleBoson:all= on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
  } else if (procname == TString("Z")){
    FileName=odir+TString("/outputZ_");
    pythia.readString("WeakBosonAndParton:qqbar2gmZg = on");
    pythia.readString("WeakBosonAndParton:qg2gmZq = on");
  } else if (procname == TString("ZHBoost")){
    FileName=odir+TString("/outputZBoost_");
    pythia.readString("WeakBosonAndParton:qqbar2gmZg = on");
    pythia.readString("WeakBosonAndParton:qg2gmZq = on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
  } else if (procname == TString("ZH")){
    FileName=odir+TString("/outputZH_");
    pythia.readString("HiggsSM:ffbar2HZ= on");
    pythia.particleData.m0(25, 125.0);   
  } else if (procname == TString("ZHBoost")){
    FileName=odir+TString("/outputZHBoost_");
    pythia.readString("HiggsSM:ffbar2HZ= on");
    pythia.readString("PhaseSpace:pTHatMin = 200.0");
    pythia.particleData.m0(25, 125.0);   
  } else {
    cerr << "Process " << procname << " not understood. Giving up..." << endl;
    exit(1);
  }

  // use tune 4C
  pythia.readString("Tune:pp = 5");

  // make B mesons/hadrons stable
  no_b_decay(pythia);

  //J0  0 17 
  //J1  17 35 
  //J2  35 70 
  //J3  70 140 
  //J4  140 280 
  //J5  280 560 
  //J6  560 1120 
  //J7  1120 2240
  //J8  2240 inf
  pythia.init( 2212, 2212, 13000.);

  // Begin event loop. Generate event. Skip if error. List first one.
  int totevent;
  if (argc>2)
    totevent=atoi(argv[2]);
  else
    totevent=100000;

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
      theTree->Branch ("Nentry",     &Nentry,     "Nentry/I");
      theTree->Branch ("Npartons",   &Npartons,   "Npartons/I");
      theTree->Branch ("Nparticles", &Nparticles, "Nparticles/I");
      theTree->Branch ("ID",         ID,          "ID[Nentry]/I");
      theTree->Branch ("Stat",       Stat,        "Stat[Nentry]/I");
      theTree->Branch ("Charge",     Charge,      "Charge[Nentry]/F");
      theTree->Branch ("Pm",         Pm,          "Pm[Nentry]/F");
      theTree->Branch ("Pt",         Pt,          "Pt[Nentry]/F");       
      theTree->Branch ("Rap",        Rap,         "Rap[Nentry]/F");       
      theTree->Branch ("Phi",        Phi,         "Phi[Nentry]/F");       
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
	Pm[Npartons]=pythia.event[i].m();
        Pt[Npartons]=pythia.event[i].pT();
        Rap[Npartons]=pythia.event[i].y();
        Phi[Npartons]=pythia.event[i].phi();

	Npartons++;
      }
    }
    for (int i=0;i<pythia.event.size();i++){	
      if (pythia.event[i].isFinal()){
	Stat[Npartons+Nparticles] = 2;
	ID[Npartons+Nparticles] = pythia.event[i].id();
	Charge[Npartons+Nparticles]=pythia.event[i].charge();
	Pm[Npartons+Nparticles]=pythia.event[i].m();
	Pt[Npartons+Nparticles]=pythia.event[i].pT();
	Rap[Npartons+Nparticles]=pythia.event[i].y();
	Phi[Npartons+Nparticles]=pythia.event[i].phi();

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
