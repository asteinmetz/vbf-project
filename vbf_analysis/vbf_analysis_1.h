// -*- c++ -*-
#ifndef VBF_ANALYSIS_H
#define VBF_ANALYSIS_H

#include "Configuration.h"
#include "HistGroup.h"
#include "Event.h"
#include "DataFrame.h"

// -- commonly used histogram types for derived classes -- 
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

#include "UserAnalysisBase.h"

class Vbf_Analysis : public UserAnalysisBase 
{
public:

  Vbf_Analysis(const std::string& name,DataFrame* signalServer,DataFrame* pileupServer=0);
  //  Vbf_Analysis(const std::string& name,DataFrame* signalServer,
  //		   DataFrame* pileupServer);
  virtual ~Vbf_Analysis();

  virtual bool book();

  virtual bool analyze(Event& pEvt);

private:

protected:
  
  TH1D* h_jets_eta_pup;
  TH1D* h_jets_phi_pup;
  TH1D* h_jets_pt_pup;

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
  //  TH1D* h_comb_mass;
  //  TH2D* d_comb_mass_pt;
  //  TH2D* d_diff_eta_comb_eta;

  TH2D* d_lead_eta_diff_eta;
  TH1D* h_lead_pt_minus_sub_pt;
  TH1D* h_distance;

  TH1D* h_pidsum;
  TH1D* h_higgs_mass;
  TH2D* d_higgs_mass_pidsum;
  TH1D* h_ndecay;
  TH2D* d_higgs_mass_ndecay;
  TH2D* d_pidsum_ndecay;
  TH1D* h_eta_cut;

  TH1D* h_higgs_lpt;
  TH2D* d_higgs_ndecay_lpt;
  TH2D* d_deta_dphi;
  TH2D* d_lead_sub;

  // list of particles to be excluded from jet finding
  std::vector<int> m_exclusions;

  double m_jet_radius;
  double m_lptcut;
  double m_jptcut;
  int m_cut;
  
  struct jetPtSorter
  {
    bool operator()(const fastjet::PseudoJet& pj0,const fastjet::PseudoJet& pj1)
    { return pj0.perp() > pj1.perp(); }
  };

};
#endif
