#include "Pythia.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include "TTree.h"
#include "TFile.h"

using namespace Pythia8;
using namespace std;


int main(int argc, char* argv[]) {

  if (argc < 3) {
    cerr << "Usage: runpy8 <paramfile> <outname> [<nevt>]" << endl;
    exit(1);
  }
  string paramfile = argv[1];
  string outfilebase = argv[2];

  TFile* outfile = 0;
  TTree* tree = 0;

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
  /// @todo Read multiple command files
  pythia.readFile(paramfile);
  pythia.init();

  unsigned int totevent = 100;
  if (argc > 3) totevent = atoi(argv[3]);
  cout << totevent << " events will be generated" << endl;
  unsigned int ievt = 0, ifile = 0;
  string filename = "";
  while (ievt < totevent){
    if (!pythia.next()) continue;
    //double pthat = pythia.info.pTHat();
    if (ievt == 0) {
      pythia.info.list();
      pythia.event.list();
    }
    if (ievt % 50000 == 0) {
      if (ievt > 0) {
        cout << "Writing output file " << filename << endl;
        tree->Write();
        outfile->Close();
        delete tree;
        delete outfile;
      }
      stringstream filename;
      filename << outfilebase << "_" << ifile++ << ".root";
      cout << "Creating new output file " << filename.str() << endl;
      outfile = new TFile(filename.str().c_str(), "RECREATE");
      tree = new TTree("h76", "Tree", 1);
      tree->Branch ("Nentry",     &Nentry,     "Nentry/I");
      tree->Branch ("Npartons",   &Npartons,   "Npartons/I");
      tree->Branch ("Nparticles", &Nparticles, "Nparticles/I");
      tree->Branch ("ID",         ID,          "ID[Nentry]/I");
      tree->Branch ("Stat",       Stat,        "Stat[Nentry]/I");
      tree->Branch ("Charge",     Charge,      "Charge[Nentry]/D");
      tree->Branch ("Px",         Px,          "Px[Nentry]/D");
      tree->Branch ("Py",         Py,          "Py[Nentry]/D");
      tree->Branch ("Pz",         Pz,          "Pz[Nentry]/D");
      tree->Branch ("P0",         P0,          "P0[Nentry]/D");
      tree->Branch ("Pm",         Pm,          "Pm[Nentry]/D");
      tree->Branch ("Pt",         Pt,          "Pt[Nentry]/D");
      tree->Branch ("Rap",        Rap,         "Rap[Nentry]/D");
      tree->Branch ("Phi",        Phi,         "Phi[Nentry]/D");
      tree->Branch ("Eta",        Eta,         "Eta[Nentry]/D");
    }

    Nentry = 0;
    Npartons = 0;
    Nparticles = 0;

    for (int i = 0; i < pythia.event.size(); i++) {
      if (pythia.event[i].status() == -21 || pythia.event[i].status() == -23) {
        Stat[Npartons] = (pythia.event[i].status() == -23) ? -1 : -2;
        ID[Npartons] = pythia.event[i].id();
        Charge[Npartons] = pythia.event[i].charge();
        Px[Npartons] = pythia.event[i].px();
        Py[Npartons] = pythia.event[i].py();
        Pz[Npartons] = pythia.event[i].pz();
        P0[Npartons] = pythia.event[i].e();
        Pm[Npartons] = pythia.event[i].m();
        Pt[Npartons] = pythia.event[i].pT();
        Rap[Npartons] = pythia.event[i].y();
        Phi[Npartons] = pythia.event[i].phi();
        Eta[Npartons] = pythia.event[i].eta();
        Npartons += 1;
      }
    }
    for (int i = 0; i < pythia.event.size(); i++) {
      if (pythia.event[i].isFinal()) {
        Stat[Npartons+Nparticles] = 2;
        ID[Npartons+Nparticles] = pythia.event[i].id();
        Charge[Npartons+Nparticles] = pythia.event[i].charge();
        Px[Npartons+Nparticles] = pythia.event[i].px();
        Py[Npartons+Nparticles] = pythia.event[i].py();
        Pz[Npartons+Nparticles] = pythia.event[i].pz();
        P0[Npartons+Nparticles] = pythia.event[i].e();
        Pm[Npartons+Nparticles] = pythia.event[i].m();
        Pt[Npartons+Nparticles] = pythia.event[i].pT();
        Rap[Npartons+Nparticles] = pythia.event[i].y();
        Phi[Npartons+Nparticles] = pythia.event[i].phi();
        Eta[Npartons+Nparticles] = pythia.event[i].eta();
        Nparticles++;
      }
    }

    Nentry = Npartons + Nparticles;
    if (ievt > 0 && ievt % 100 == 0) cout << ievt << endl;
    tree->Fill();
    ievt += 1;
  }

  tree->Write();
  outfile->Close();
  pythia.statistics();
  return 0;
}
