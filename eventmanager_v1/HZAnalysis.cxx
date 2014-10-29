#include "HZAnalysis.h"

#include "Event.h"
#include "matching.hh" 
#include "SelectorArea.hh"
#include "ExtraSelectors.hh"
#include "MDTaggerWithSub.hh"
#include <fastjet/ClusterSequenceArea.hh>

#include "FilterFromCmdLine.hh"
#include "SimpleBTagger.h"

// make the typing easier
using namespace std;
using namespace fastjet;

namespace serv_hist = Services::Histogramming; // again lazyness!

HZAnalysis::~HZAnalysis(){
  if (_bge) delete _bge;
}

bool HZAnalysis::book(){
  // check if any of the parameters set in the command line have an
  // impact for us
  //--------------------------------------------------
  double tmp;
  if (Configuration::getConfig("jet_R",      tmp)) set_jet_R(tmp);
  if (Configuration::getConfig("jet_ptmin",  tmp)) set_jet_ptmin(tmp);
  if (Configuration::getConfig("jet_rapmax", tmp)) set_jet_rapmax(tmp);
  if (Configuration::getConfig("emu_ptmin",  tmp)) set_emu_ptmin(tmp);
  if (Configuration::getConfig("emu_rapmax", tmp)) set_emu_rapmax(tmp);
  
  _do_btagging = true;
  Configuration::getConfig("btagging", _do_btagging);
    

  // jet selection and background estimation
  //--------------------------------------------------
  _jet_def = JetDefinition(antikt_algorithm, _R);

  // initialise cuts
  _sel_visible = !(SelectorAbsId(12) || SelectorAbsId(14) || SelectorAbsId(16));
  _sel_emus = (SelectorAbsId(11) || SelectorAbsId(13));
  _sel_extra_emus = SelectorAbsRapMax(_emu_rapmax) * SelectorPtMin(_emu_ptmin);
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

  // initialise the pre-filtering step
  //--------------------------------------------------
  if (!Configuration::getFilter(_prefilter, _R, _bge, "prefilter")){
    _prefilter = Filter(JetDefinition(cambridge_algorithm,0.2),
                        SelectorAreaRhoThreshold(_bge, 0.0, 2.0));
  }
  _prefilter.set_subtractor(&_subtractor);

  //--------------------------------------------------
  //--------  HERE:  specify your filter   -----------
  //--------------------------------------------------
  hists["Z_full"] = serv_hist::book<TH1D>("Z_full", "Zmass,full",
                                          200,0.,200., "m [GeV]");

  hists["H_init_hard"] = serv_hist::book<TH1D>("H_init_hard", "Hmass,init,hard",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_hard"] = serv_hist::book<TH1D>("H_tagd_hard", "Hmass,tagged,hard",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_hard"] = serv_hist::book<TH1D>("H_btag_hard", "Hmass,b-tagged,hard",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_hard"] = serv_hist::book<TH1D>("H_filt_hard", "Hmass,filtered,hard",
                                               200,0.,200., "m [GeV]");

  hists["H_init_hsub"] = serv_hist::book<TH1D>("H_init_hsub", "Hmass,init,hsub",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_hsub"] = serv_hist::book<TH1D>("H_tagd_hsub", "Hmass,tagged,hsub",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_hsub"] = serv_hist::book<TH1D>("H_btag_hsub", "Hmass,b-tagged,hsub",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_hsub"] = serv_hist::book<TH1D>("H_filt_hsub", "Hmass,filtered,hsub",
                                               200,0.,200., "m [GeV]");

  hists["H_init_hpre"] = serv_hist::book<TH1D>("H_init_hpre", "Hmass,init,hpre",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_hpre"] = serv_hist::book<TH1D>("H_tagd_hpre", "Hmass,tagged,hpre",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_hpre"] = serv_hist::book<TH1D>("H_btag_hpre", "Hmass,b-tagged,hpre",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_hpre"] = serv_hist::book<TH1D>("H_filt_hpre", "Hmass,filtered,hpre",
                                               200,0.,200., "m [GeV]");

  hists["H_init_full"] = serv_hist::book<TH1D>("H_init_full", "Hmass,init,full",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_full"] = serv_hist::book<TH1D>("H_tagd_full", "Hmass,tagged,full",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_full"] = serv_hist::book<TH1D>("H_btag_full", "Hmass,b-tagged,full",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_full"] = serv_hist::book<TH1D>("H_filt_full", "Hmass,filtered,full",
                                               200,0.,200., "m [GeV]"); 

  hists["H_init_subt"] = serv_hist::book<TH1D>("H_init_subt", "Hmass,init,subt",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_subt"] = serv_hist::book<TH1D>("H_tagd_subt", "Hmass,tagged,subt",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_subt"] = serv_hist::book<TH1D>("H_btag_subt", "Hmass,b-tagged,subt",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_subt"] = serv_hist::book<TH1D>("H_filt_subt", "Hmass,filtered,subt",
                                               200,0.,200., "m [GeV]"); 

  hists["H_init_pref"] = serv_hist::book<TH1D>("H_init_pref", "Hmass,init,pref",
                                               200,0.,200., "m [GeV]");
  hists["H_tagd_pref"] = serv_hist::book<TH1D>("H_tagd_pref", "Hmass,tagged,pref",
                                               200,0.,200., "m [GeV]");
  hists["H_btag_pref"] = serv_hist::book<TH1D>("H_btag_pref", "Hmass,b-tagged,pref",
                                               200,0.,200., "m [GeV]");
  hists["H_filt_pref"] = serv_hist::book<TH1D>("H_filt_pref", "Hmass,filtered,pref",
                                               200,0.,200., "m [GeV]"); 

  // an interesting measure of how the pileup affects tagging or plain
  // kinematic reconstruction is to look at the Rbb distance
  hists["Rbb_hard"] = serv_hist::book<TH1D>("Rbb_hard", "Rbb,hard",150,0.0,1.5, "Rbb"); 
  hists["Rbb_hsub"] = serv_hist::book<TH1D>("Rbb_hsub", "Rbb,hsub",150,0.0,1.5, "Rbb"); 
  hists["Rbb_hpre"] = serv_hist::book<TH1D>("Rbb_hpre", "Rbb,hpre",150,0.0,1.5, "Rbb"); 
  hists["Rbb_full"] = serv_hist::book<TH1D>("Rbb_full", "Rbb,full",150,0.0,1.5, "Rbb"); 
  hists["Rbb_subt"] = serv_hist::book<TH1D>("Rbb_subt", "Rbb,subt",150,0.0,1.5, "Rbb"); 
  hists["Rbb_pref"] = serv_hist::book<TH1D>("Rbb_pref", "Rbb,pref",150,0.0,1.5, "Rbb"); 

  return true;
}


bool HZAnalysis::analyze(Event& pEvt){
  // in practice, we've noticed that the over-simplified Z->ll tagging
  // we do is insensitive to PU, so we'll do it once and for all
  // (that allows to limit some clustering later on)
  vector<PseudoJet> full_event = _sel_visible(pEvt.pseudoJets());

  //----------------------------------------------------------------------
  // do the Z tagging
  // 
  // we select all leptons (e/mu) passing our kinematic cuts.  Then
  // find the pair of opposite-sign, same-flavour leptons that is
  // closest to the Z mass

  // separate leptons from the rest
  vector<PseudoJet> full_leptons, full_hadrons;
  _sel_emus.sift(full_event, full_leptons, full_hadrons);

  // do the Z tagging
  if (!_tagZ(full_leptons, _sel_Zmass)) return false;

  // require that there's no lepton left
  if ((_sel_extra_emus(full_leptons)).size()) return false;

  // append the remaining leptons to the hadrons
  for (unsigned int j=0; j<full_leptons.size(); j++){
    full_hadrons.push_back(full_leptons[j]);
  }  

  //----------------------------------------------------------------------
  // do all the clustering pre-analysis to avoid unneeded repetitions
  vector<PseudoJet> hard_hadrons = SelectorHardVertex()(full_hadrons);
  ClusterSequenceArea cs_hard(hard_hadrons, _jet_def, _area_def);
  ClusterSequenceArea cs_full(full_hadrons, _jet_def, _area_def);

  _bge->set_particles(hard_hadrons);
  _processHadronic(cs_hard, "hard", false, false);
  _processHadronic(cs_hard, "hsub", true , false);
  _processHadronic(cs_hard, "hpre", true , true );

  _bge->set_particles(full_hadrons);
  _processHadronic(cs_full, "full", false, false);
  _processHadronic(cs_full, "subt", true , false);
  _processHadronic(cs_full, "pref", true , true );

  return true;
}


// reconstruct the Z boson in the event from a given set of leptons
//
// We will follow the description in the BDRS paper: reconstruct a
// Z with 80<mass<100 GeV and pT>200 GeV [We'll use 225].
//
// We further impose that there are no lepton left with pt>30 GeV
// and |y|<2.5
//
// QUESTION: do we also impose the 30 GeV cut on the original list
//           of leptons or only after the reconstruction of the Z??
//           The paper seems to indicate the latter... so the latter
//           it will be
bool HZAnalysis::_tagZ(vector<PseudoJet> &leptons, Selector &_sel_Z){
  // we'll need at least 2 leptons!
  if (leptons.size() < 2) return false;

  int i1=-1, i2=-1;
  double delta_m_min=1000.0;

  Selector sel_Z = _sel_Zmass * SelectorPtMin(_jet_ptmin);

  for (unsigned int j1=0; j1<leptons.size()-1; j1++){
    int id1 = leptons[j1].user_info<ParticleInfo>().id();

    for (unsigned int j2=j1+1; j2<leptons.size(); j2++){
      int id2 = leptons[j2].user_info<ParticleInfo>().id();

      // the opposite-sign-smae-flavour test means the sum of their
      // psg_id should be 0
      if (id1+id2) continue;

      // check if this is compatible with a boosted Z
      if (! (sel_Z.pass(join(leptons[j1], leptons[j2])))) continue;

      // if we have multiple options, we'll keep the one closest to
      // the Z mass
      double delta_m = abs((leptons[j1]+leptons[j2]).m()-91.2);
      if (delta_m<delta_m_min){
        i1=j1; i2=j2;
        delta_m_min = delta_m;
      }
    }
  }
  
  if (i1<0) return false;

  // now we have our Z candidate we can cut on
  PseudoJet Z = join(leptons[i1], leptons[i2]);
  hists["Z_full"]->Fill(Z.m());
  if (! (_sel_Z.pass(Z))) return false; 
  
  // remove the 2 leptons
  // (not 100% eficcient but not so many leptons...)
  leptons.erase(leptons.begin()+i2);
  leptons.erase(leptons.begin()+i1);

  return true;
}


// Given a set of hadrons and a cluster sequence, check for a fat
// jet. If there is one, apply a mass drop tagger and if this
// succeeds, filter the resulting tagged PseudoJet.
//
//   cs            the cluster sequence from which to get the jets
//                 (avoids multiple clusterings)
//   do_areas      apply PU subtraction wherever it's applicable
//   do_prefilter  before applying teh tagger, pre-filtere the jet
//
// Note that we do not impose the presence of no extra jet
bool HZAnalysis::_processHadronic(const ClusterSequence &cs,
				  const string &name,
				  bool do_areas, bool do_prefilter){
  double weight=1.0;

  //----------------------------------------------------------------------
  // get the hardest jet
  vector<PseudoJet> jets;
  if (do_areas){
    // decide if we do prefiltering
    if (do_prefilter){
      jets = _sel_jets(_prefilter(cs.inclusive_jets()));
    } else {
      jets = _sel_jets(_subtractor(cs.inclusive_jets()));
    }
    _tagger.set_subtractor(&_subtractor);
  } else {
    jets = _sel_jets(cs.inclusive_jets());
    _tagger.set_subtractor(0);
  }
  if (jets.size() != 1){
    return false;
  }
  
  hists[string("H_init_")+name]->Fill(jets[0].m(), weight);

  //----------------------------------------------------------------------
  // apply the tagger
  PseudoJet Hcandidate = _tagger(jets[0]);
  if ( Hcandidate == 0){ return false;}
  hists[string("H_tagd_")+name]->Fill(Hcandidate.m(), weight);

  vector<PseudoJet> bs = Hcandidate.pieces();
  double Rbb = bs[0].delta_R(bs[1]);
  hists[string("Rbb_")+name]->Fill(Rbb);

  //----------------------------------------------------------------------
  // apply b tagging if requested
  if (_do_btagging){
    SimpleBTagger btagger(0.6, 0.02);
    weight *= btagger(bs[0]) * btagger(bs[1]);
    hists[string("H_btag_")+name]->Fill(Hcandidate.m(), weight);
  }

  //----------------------------------------------------------------------
  // apply the filter
  //
  // Note that we allow for specification of the filter from the
  // command line. This has to be done here because Rbb is not known
  // before!
  if (Rbb>0.6) Rbb=0.6;

  if (!Configuration::getFilter(_filter, 0.5*Rbb, _bge))
    _filter = Filter(JetDefinition(cambridge_algorithm, Rbb/2), SelectorNHardest(3));

  if (do_areas)
    _filter.set_subtractor(&_subtractor);
  else
    _filter.set_subtractor(0);

  PseudoJet H = _filter(Hcandidate);
  hists[string("H_filt_")+name]->Fill(H.m(), weight);  

  return true;
}
