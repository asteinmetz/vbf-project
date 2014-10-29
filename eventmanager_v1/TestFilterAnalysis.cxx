#include "TestFilterAnalysis.h"

#include "matching.hh" 
#include "SelectorArea.hh"
#include <fastjet/ClusterSequenceArea.hh>

// make the typing easier
using namespace std;
using namespace fastjet;

namespace serv_hist = Services::Histogramming; // again lazyness!

TestFilterAnalysis::~TestFilterAnalysis(){
  if (_bge) delete _bge;
}

bool TestFilterAnalysis::book(){
  // jet selection and background estimation
  //--------------------------------------------------
  _jet_def = JetDefinition(antikt_algorithm, _R);

  // a handy tool to get apply cuts on the jets
  _sel_jets = SelectorAbsRapMax(_jet_rapmax) * SelectorPtMin(_jet_ptmin);

  // the type of area we want (many tools require explicit ghosts)
  _area_def
    = AreaDefinition(active_area_explicit_ghosts, 
                     GhostedAreaSpec(SelectorAbsRapMax(_jet_rapmax+1)));

  // the object that does the estimation of rho. The argument provided
  // are the following:
  //
  //   - which jets enter the estimation (here we use all jets up to
  //     jet_rapmax and exclude the 2 hardest (not a big effect but
  //     easily motivated)
  //   - the jet definition used to break the event into patched
  //     [Watch out: this is used for rho estimation, not to cluster
  //     the jets in your event]
  //   - the area definition (same as above)
  _bge = new JetMedianBackgroundEstimator((!SelectorNHardest(2)) * SelectorAbsRapMax(_jet_rapmax),
                                          JetDefinition(kt_algorithm, 0.4), _area_def);

  // once you have this you can apply a correction for the rapidity dependence
  // [the numbers below are for Pythia8(4C) at 13 TeV
  _rescaling = BackgroundRescalingYPolynomial(1.1685397, 0, -0.0246807, 0, 5.94119e-05);  
  _bge->set_rescaling_class(&_rescaling);

  // a tool that subtracts the jets
  _subtractor = SubtractorWithMass(_bge);

  //--------------------------------------------------
  //--------  HERE:  specify your filter   -----------
  //--------------------------------------------------

  // filtering keeping the hardest subjets
  // _filter=Filter(JetDefinition(cambridge_algorithm, _R/2),
  //                SelectorNHardest(2));

  // trimming
  // _filter=Filter(JetDefinition(cambridge_algorithm, 0.2),
  //                SelectorPtFraction(0.03));

  // cut at n * sigma * sqrt(A) [assuming we'll use subtraction]
  _filter=Filter(JetDefinition(cambridge_algorithm, 0.2),
                 SelectorAreaRhoThreshold(_bge, 0.0, 3.0));

  // automatic subtraction in a filter [this subtracts the pileup before
  // applying the "selection cut" which is what you want]
  _filter.set_subtractor(&_subtractor);


  // we're going to look at the pt and mass distributionns of the jet
  //  wo pileup, with pileup, with "strandard" PU subtraction, after
  //  filtering
  hists["pt_hard"] = serv_hist::book<TH1D>("Spectra", "pt_hard", "pt,hard",
                                           200,0.,1000., "p_t [GeV]");
  hists["pt_full"] = serv_hist::book<TH1D>("Spectra", "pt_full", "pt,full",
                                           200,0.,1000., "p_t [GeV]");
  hists["pt_subt"] = serv_hist::book<TH1D>("Spectra", "pt_subt", "pt,subtracted",
                                           200,0.,1000., "p_t [GeV]");
  hists["pt_filt"] = serv_hist::book<TH1D>("Spectra", "pt_filt", "pt,filtered",
                                           200,0.,1000., "p_t [GeV]");

  hists["m_hard"] = serv_hist::book<TH1D>("Spectra", "m_hard", "mass,hard",
                                          200,0.,1000., "m [GeV]");
  hists["m_full"] = serv_hist::book<TH1D>("Spectra", "m_full", "mass,full",
                                          200,0.,1000., "m [GeV]");
  hists["m_subt"] = serv_hist::book<TH1D>("Spectra", "m_subt", "mass,subtracted",
                                          200,0.,1000., "m [GeV]");
  hists["m_filt"] = serv_hist::book<TH1D>("Spectra", "m_filt", "mass,filtered",
                                          200,0.,1000., "m [GeV]");

  // and then a few profiles of the errors as a function of "mu"
  profiles["delta_pt_full"] = serv_hist::book<TProfile>("Shifts", "pt_full", "pt,full",
                                                        201,-0.5,200.5, "#mu");
  profiles["delta_pt_subt"] = serv_hist::book<TProfile>("Shifts", "pt_subt", "pt,subtracted",
                                                        201,-0.5,200.5, "#mu");
  profiles["delta_pt_filt"] = serv_hist::book<TProfile>("Shifts", "pt_filt", "pt,filtered",
                                                        201,-0.5,200.5, "#mu");

  profiles["delta2_pt_full"] = serv_hist::book<TProfile>("Squares", "pt_full", "pt,full",
                                                         201,-0.5,200.5, "#mu");

  profiles["disp_pt_full"] = serv_hist::book<TProfile>("Disp", "pt_full", "pt,full",
                                                       201,-0.5,200.5, "#mu");

  profiles["delta_m_full"] = serv_hist::book<TProfile>("Shifts", "m_full", "mass,full",
                                                       201,-0.5,200.5, "#mu");
  profiles["delta_m_subt"] = serv_hist::book<TProfile>("Shifts", "m_subt", "mass,subtracted",
                                                       201,-0.5,200.5, "#mu");
  profiles["delta_m_filt"] = serv_hist::book<TProfile>("Shifts", "m_filt", "mass,filtered",
                                                       201,-0.5,200.5, "#mu");

  return true;
}

bool TestFilterAnalysis::analyze(Event& pEvt){
  // get the particles (and add a simple flag for matching later on)
  vector<PseudoJet> full_event = pEvt.pseudoJets();
  for (unsigned int i=0; i< full_event.size(); i++) 
    full_event[i].set_user_index(i);
  
  vector<PseudoJet> hard_event = pEvt.pseudoJets(Vertex::SIGNAL);
  for (unsigned int i=0; i< hard_event.size(); i++) 
    hard_event[i].set_user_index(i);
  
  // cluster the hard event (probably no need for area support here)
  ClusterSequence cs_hard(hard_event, _jet_def);
  vector<PseudoJet> hard_jets = _sel_jets(cs_hard.inclusive_jets());
                          
  // if there is no hard jet in the event, just skip it
  if (! hard_jets.size()) return true;
  
  // cluster the full event (with PU --- including area computation)
  ClusterSequenceArea cs_full(full_event, _jet_def, _area_def);
  vector<PseudoJet> full_jets = cs_full.inclusive_jets(); // all the jets, no cuts applied
  
  // start the matching
  MatchableSet matching(hard_jets);

  // now deal with the background estimation
  // all that's needed is to tell the estimator the particles to work with
  _bge->set_particles(full_event);

  // loop over all the full jet and select those matching a hard one
  for (unsigned int ijet=0; ijet<full_jets.size(); ijet++){
    const PseudoJet &full_jet = full_jets[ijet];

    // check if we have a matching (watch out: you'll get a pointer)
    matching.match_probe(full_jet);
    const PseudoJet  * hard_jet = matching.matched_jet_ptr();
    if (! hard_jet) continue;

    // get the subtracted jet
    PseudoJet subtracted_jet = _subtractor(full_jet);

    // filter the jet
    PseudoJet filtered_jet = _filter(full_jet);

    hists["pt_hard"]->Fill(hard_jet->pt());
    hists["pt_full"]->Fill(full_jet.pt());
    hists["pt_subt"]->Fill(subtracted_jet.pt());
    hists["pt_filt"]->Fill(filtered_jet.pt());

    hists["m_hard"]->Fill(hard_jet->m());
    hists["m_full"]->Fill(full_jet.m());
    hists["m_subt"]->Fill(subtracted_jet.m());
    hists["m_filt"]->Fill(filtered_jet.m());

    double delta_full = full_jet.pt()-hard_jet->pt();
    profiles["delta_pt_full"]->Fill(pEvt.nVertices(), delta_full);
    profiles["delta_pt_subt"]->Fill(pEvt.nVertices(), subtracted_jet.pt()-hard_jet->pt());
    profiles["delta_pt_filt"]->Fill(pEvt.nVertices(), filtered_jet.pt()-hard_jet->pt());

    profiles["delta2_pt_full"]->Fill(pEvt.nVertices(), delta_full*delta_full);

    profiles["delta_m_full"]->Fill(pEvt.nVertices(), full_jet.m()-hard_jet->m());
    profiles["delta_m_subt"]->Fill(pEvt.nVertices(), subtracted_jet.m()-hard_jet->m());
    profiles["delta_m_filt"]->Fill(pEvt.nVertices(), filtered_jet.m()-hard_jet->m());
  }

  return true;
}

bool TestFilterAnalysis::write(){
  profiles["disp_pt_full"]->Reset();
  for (unsigned int i=1; i<=profiles["disp_pt_full"]->GetNbinsX(); i++){
    double w  = profiles["delta_pt_full"]->GetBinContent(i);
    double w2 = profiles["delta2_pt_full"]->GetBinContent(i);
    double s  = w2 - w*w;
    profiles["disp_pt_full"]->Fill(profiles["delta_pt_full"]->GetBinCenter(i),
                                   sqrt((s>0) ? s : 0.0));
  }

  UserAnalysisBase::write();

  return true;
}
