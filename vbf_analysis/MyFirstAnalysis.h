// -*- c++ -*-
#ifndef MYFIRSTANALYSIS_H
#define MYFIRSTANALYSIS_H

#include "Configuration.h"
#include "HistGroup.h"
#include "Event.h"
#include "DataFrame.h"

// -- commonly used histogram types for derived classes -- 
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

#include "UserAnalysisBase.h"

class MyFirstAnalysis : public UserAnalysisBase 
{
public:

  MyFirstAnalysis(const std::string& name,DataFrame* dataServer);
  //  MyFirstAnalysis(const std::string& name,DataFrame* signalServer,
  //		   DataFrame* pileupServer);
  virtual ~MyFirstAnalysis();

  virtual bool book();

  virtual bool analyze(Event& pEvt);

private:

protected:
  
  TH1D* h_eta;
  TH1D* h_phi;
  TH1D* h_pt;

  TH1D* h_jets_eta;
  TH1D* h_jets_phi;
  TH1D* h_jets_pt;

  TH1D* h_njets;

  TH1D* h_comb_pt;
  TH1D* h_comb_lead_pt;
  TH1D* h_comb_sub_pt;
  TH1D* h_comb_eta;
  TH1D* h_comb_lead_eta;
  TH1D* h_comb_sub_eta;
  TH1D* h_comb_mass;
  TH2D* d_comb_mass_pt;
  TH2D* d_diff_eta_comb_eta;

  TH2D* d_lead_eta_diff_eta;
  TH1D* h_lead_pt_minus_sub_pt;
  TH1D* h_distance;

  struct jetPtSorter
  {
    bool operator()(const fastjet::PseudoJet& pj0,const fastjet::PseudoJet& pj1)
    { return pj0.perp() > pj1.perp(); }
  };

};
#endif
