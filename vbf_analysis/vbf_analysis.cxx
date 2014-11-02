#include "PrintDef.h"
#include "TFile.h"
#include "HistGroupParticle.h"
#include "HistGroupCount.h"
#include "HistGroupEvent.h"
#include "Services.h"
#include "Selectors.h"
#include "Configuration.h"

#include "Event.h"
#include "vbf_analysis.h"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include "fastjet/tools/Filter.hh"

#include "TowerGrid.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

Vbf_Analysis::Vbf_Analysis(const std::string& name,
			   DataFrame* signalServer,
			   DataFrame* pileupServer)

  : UserAnalysisBase(name,signalServer,pileupServer)
  , m_towerGridAll(100,-5.,5.)
  , m_towerGridSig(100,-5.,5.)
  , m_towerGridPup(100,-5.,5.)
    //  , h_eta(0)
    //  , h_phi(0)
    //  , h_pt(0)
    // this instantiates the objects righ away -> FAST!
    // initiates HistGroups
    //  , g_eventin("EventIn")
  , g_partsig("SigParticles")
  , g_partall("AllParticles")
  , g_partpup("PupParticles")
  , g_towergridall("AllTowers")
  , g_towergridsig("SigTowers")
  , g_towergridpup("PupTowers")
  , g_jet("AllJets")
  , g_jet_sub("TrimJets")
  , g_pup("Pileup")
  , g_njets("CountJets")
  , g_njets_sub("CountSubJets")
  , m_jet_radius(0.4)
  , m_lptcut(10.0)
  , m_jptcut(20.0)
  , m_cut(0)
{
  PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","Constructor\n");
  m_exclusions.push_back(11);
  m_exclusions.push_back(12);
  m_exclusions.push_back(13);
  m_exclusions.push_back(14);

  // check on external parameters
  if ( Configuration::getConfig("jet_radius",m_jet_radius) )
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","New jet size %4.2f\n",m_jet_radius); }
  else
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","Default jet size %4.2f\n",m_jet_radius); }
  if ( Configuration::getConfig("lptcut",m_lptcut) )
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","New lepton pT cut %4.2f GeV\n",m_lptcut); }
  else
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","Default lepton pT cut %4.2f GeV\n",m_lptcut); }
  if ( Configuration::getConfig("jptcut",m_jptcut) )
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","New jet pT cut %4.2f GeV\n",m_jptcut); }
  else
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","Default jet pT cut %4.2f GeV\n",m_jptcut); }
  if ( Configuration::getConfig("dolcut",m_cut) )
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","New m_cut %i\n",m_cut); }
  else
    { PRINT_INFO("Vbf_Analysis::Vbf_Analysis(...)","Default m_cut %i\n",m_cut); }

  // book all histograms in the all groups
  //  g_eventin.book();
  g_partsig.book();
  g_partall.book();
  g_partpup.book();
  g_towergridall.book();
  g_towergridsig.book();
  g_towergridpup.book();
  g_jet.book();
  g_jet_sub.book();
  g_pup.book();
  g_njets.book();
  g_njets_sub.book();
}

Vbf_Analysis::~Vbf_Analysis()
{ }

bool Vbf_Analysis::analyze(Event& pEvt)
{ 
  // reset tower grid
  m_towerGridAll.reset();
  m_towerGridSig.reset();
  m_towerGridPup.reset();

  // incoming events
  //  g_eventin.fill(pEvt);

  // allocate all internal lists
  std::vector<fastjet::PseudoJet> pjSignal;
  std::vector<fastjet::PseudoJet> pjAll;
  std::vector<fastjet::PseudoJet> pjPileup;

  // get all particles lists
  pjSignal = pEvt.pseudoJets(Vertex::SIGNAL);
  pjAll = pEvt.pseudoJets();
  pjPileup = pEvt.pseudoJets(Vertex::PILEUP);

  // HistGroupParticle fills for all particles
  g_partsig.fill(pjSignal);
  g_partall.fill(pjAll);
  g_partpup.fill(pjPileup);

  // fill towers from particles (Note: In future, export to HistGroupTower)
  std::vector<fastjet::PseudoJet>::const_iterator fAll(pjAll.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lAll(pjAll.end());
  for ( ; fAll != lAll; ++fAll )
    { m_towerGridAll.add(fAll->pseudorapidity(),fAll->phi_std(),fAll->e()); }
  std::vector<fastjet::PseudoJet>::const_iterator fSig(pjSignal.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lSig(pjSignal.end());
  for ( ; fSig != lSig; ++fSig )
    { m_towerGridSig.add(fSig->pseudorapidity(),fSig->phi_std(),fSig->e()); }
  std::vector<fastjet::PseudoJet>::const_iterator fPup(pjPileup.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lPup(pjPileup.end());
  for ( ; fPup != lPup; ++fPup )
    { m_towerGridPup.add(fPup->pseudorapidity(),fPup->phi_std(),fPup->e()); }

  // get new jets from towers
  std::vector<fastjet::PseudoJet> towerAll;
  std::vector<fastjet::PseudoJet> towerSig;
  std::vector<fastjet::PseudoJet> towerPup;
  if ( !m_towerGridAll.fillPseudoJets(towerAll) )
    { /* print an error message because there are no tower jets! */ }
  if ( !m_towerGridSig.fillPseudoJets(towerSig) )
    { /* print an error message because there are no tower jets! */ }
  if ( !m_towerGridPup.fillPseudoJets(towerPup) )
    { /* print an error message because there are no tower jets! */ }

  // HisGroupParticle fills for all towers (No jet finding!)
  g_towergridall.fill(towerAll);
  g_towergridsig.fill(towerSig);
  g_towergridpup.fill(towerPup);

  // background estimator and event rho
  fastjet::GridMedianBackgroundEstimator bge(5.0,0.5);
  bge.set_particles(pjAll);
  fastjet::Filter trimmer(0.2,fastjet::SelectorPtFractionMin(0.05));
  fastjet::Subtractor subtractor(&bge);
  trimmer.set_subtractor(&subtractor);
  double rho = bge.rho();
  d_rho->Fill((double)pEvt.nVertices(),rho);

  // loop on list of input particles - Replaced by HistGroupParticle
  std::vector<fastjet::PseudoJet>::iterator fPart(pjAll.begin());
  std::vector<fastjet::PseudoJet>::iterator lPart(pjAll.end());
  //for ( ; fPart != lPart; ++fPart )
  //  {
  //    h_eta->Fill(fPart->pseudorapidity());
  //    h_phi->Fill(fPart->phi_std());
  //    h_pt->Fill(fPart->perp());
  //  }

  // Remove Final Higgs Decay Products
  std::vector<fastjet::PseudoJet> vbfSignal;
  std::vector<fastjet::PseudoJet> rejsignal;       
  int ndecay(0);
  int pidsum(0);
  fastjet::PseudoJet Higgs;
  
  fPart = pjAll.begin();
  for ( ; fPart != lPart; ++fPart )
    {
      // ask for the identity of the particle
      int pid = Features::particleId(*fPart);
      if ( pid == 0 )
	{ PRINT_WARN("%s::analyze(...)","ParticleId = %i is invalid\n",this->name().c_str(),pid); }
      // find final state particle definitively not from Higgs->ZZ->llll(llnunu)(nunununu) decays
      if ( std::find(m_exclusions.begin(),m_exclusions.end(),abs(pid)) == m_exclusions.end() ) 
	{ vbfSignal.push_back(*fPart); }
      else { 
	if ( m_cut == 1 )
	  {
	    if ( fPart->perp() > m_lptcut ) // decay products need to have pT
	      {
		pidsum += pid;
		Higgs += *fPart;
		++ndecay;
		rejsignal.push_back(*fPart);
	      }
	    else
	      {	vbfSignal.push_back(*fPart); }
	  }
	else
	  {
	    pidsum += pid;
	    Higgs += *fPart;
	    ++ndecay;
	    rejsignal.push_back(*fPart);
	  }
      }
    }

  // Define which jets we want
  // This defines the AntiKt jet algorithm Rcone=0.4
  double maxrap = 5.0;
  unsigned int n_repeat = 1;
  double ghost_area = 0.01;
  fastjet::GhostedAreaSpec area_spec(maxrap,n_repeat,ghost_area);
  fastjet::JetDefinition jetDef(fastjet::antikt_algorithm,m_jet_radius);
  fastjet::AreaDefinition area_def(fastjet::active_area_explicit_ghosts,area_spec);
  fastjet::ClusterSequenceArea cseq(vbfSignal,jetDef,area_def);
  fastjet::ClusterSequenceArea puseq(pjPileup,jetDef,area_def);

  // get the jets with pT > cut
  std::vector<fastjet::PseudoJet> aktJets;
  std::vector<fastjet::PseudoJet> aktJets_sub = cseq.inclusive_jets(m_jptcut);
  std::vector<fastjet::PseudoJet> pupJets = puseq.inclusive_jets(m_jptcut);

  // subtractor and trimming
  //int s_trim = 1;
  //if ( s_trim == 1 )
  //{
  aktJets_sub = trimmer(aktJets_sub); // with trimming
  //}
  //else if ( s_trim == 0 )
  //{
  //aktJets_sub = subtractor(aktJets_sub); // without trimming
  //}
  std::vector<fastjet::PseudoJet>::iterator fsub(aktJets_sub.begin());
  std::vector<fastjet::PseudoJet>::iterator lsub(aktJets_sub.end());
  for ( ; fsub != lsub; ++fsub )
    {
      if (fsub->perp() >= m_jptcut)
	{
	  aktJets.push_back(*fsub);
	}
    }

  //Pickup Number of jets - Replaced by HistGroupCount
  double njets=(double)aktJets.size();
  //double njets_sub=(double)aktJets_sub.size();
  g_njets.fill(aktJets);
  g_njets_sub.fill(aktJets_sub);
  //  h_njets->Fill(njets);
  //  h_njets_sub->Fill(njets_sub);
  //  if (njets_sub != 0)
  //    {
  //      h_njets_ratio->Fill(njets/njets_sub);  
  //    }



  // loop jets - Replaced by HistGroupParticle
  g_jet.fill(aktJets);
  g_jet_sub.fill(aktJets_sub);
  g_pup.fill(pupJets);
  //  std::vector<fastjet::PseudoJet>::iterator fJet(aktJets.begin());
  //  std::vector<fastjet::PseudoJet>::iterator lJet(aktJets.end());
  //  for ( ; fJet != lJet; ++fJet )
  //    {
  //      h_jets_eta->Fill(fJet->pseudorapidity());
  //      h_jets_phi->Fill(fJet->phi_std());
  //      h_jets_pt->Fill(fJet->perp());
  //    }
  //
  //  std::vector<fastjet::PseudoJet>::iterator fpup(pupJets.begin());
  //  std::vector<fastjet::PseudoJet>::iterator lpup(pupJets.end());
  //  for ( ; fpup != lpup; ++fpup )
  //    {
  //      h_jets_eta_pup->Fill(fpup->pseudorapidity());
  //      h_jets_phi_pup->Fill(fpup->phi_std());
  //      h_jets_pt_pup->Fill(fpup->perp());
  //    }

  // sort the jets by pT
  if ( njets >= 2. )
    {
      // accepted events
      //g_eventfiltered->fill(pEvt);

      std::sort(aktJets.begin(),aktJets.end(),jetPtSorter());
      fastjet::PseudoJet z0jet = aktJets.at(0);
      fastjet::PseudoJet z1jet = aktJets.at(1);
      fastjet::PseudoJet zJet  = aktJets.at(0);
      // candidate massive particle
      zJet += z1jet;
      // pT of the combined jet
      h_comb_pt->Fill(zJet.perp());
      // pT of the leading jet
      h_comb_lead_pt->Fill(z0jet.perp());
      // pT of the sub-leading jet
      h_comb_sub_pt->Fill(z1jet.perp());
      // eta of combined jet
      h_comb_eta->Fill(zJet.pseudorapidity());
      // eta of leading jet
      h_comb_lead_eta->Fill(z0jet.pseudorapidity());
      // eta of sub-leading jet
      h_comb_sub_eta->Fill(z1jet.pseudorapidity());
 
     // My Graphs
      // lead_eta versus lead_eta - sub_eta
      d_lead_eta_diff_eta->Fill(z0jet.pseudorapidity(),z0jet.pseudorapidity()-z1jet.pseudorapidity());
      // lead_pt versus sub_pt
      h_lead_pt_minus_sub_pt->Fill(z0jet.perp()-z1jet.perp());
      // Jet Distance on phi-eta plane
      h_distance->Fill(z0jet.delta_R(z1jet));
      
      // Sum of Particle IDs from Higgs Decay, expect to be zero.
      h_pidsum->Fill((double)pidsum);
      // Histogram of Higgs Mass, reconstructed.
      h_higgs_mass->Fill(Higgs.m());
      // Mass against the PIDSUM
      d_higgs_mass_pidsum->Fill(Higgs.m(),(double)pidsum);
      // Number of Higgs Decay Products
      h_ndecay->Fill((double)ndecay);
      // Mass against the number of decay products
      d_higgs_mass_ndecay->Fill(Higgs.m(),(double)ndecay);
      // PIDSUM versus Ndecay
      d_pidsum_ndecay->Fill((double)pidsum,(double)ndecay);
      
      // delta eta versus delta phi
      d_deta_dphi->Fill(z0jet.pseudorapidity()-z1jet.pseudorapidity(),z0jet.delta_phi_to(z1jet));
      
      // lead eta versus sub lead eta
      d_lead_sub->Fill(z0jet.pseudorapidity(),z1jet.pseudorapidity());
	  
      std::vector<fastjet::PseudoJet>::iterator fRej(rejsignal.begin());
      std::vector<fastjet::PseudoJet>::iterator lRej(rejsignal.end());
      for ( ; fRej != lRej; ++fRej )
	{
	  // P_T of Higgs Decay leptons
	  h_higgs_lpt->Fill(fRej->perp());
	  // P_T of Higgs Decay versus number of decays
	  d_higgs_ndecay_lpt->Fill((double)ndecay,fRej->perp());
	}
      
      if ( (fabs(z1jet.pseudorapidity()) <= 2.0) )
	{
	  h_eta_cut->Fill(z0jet.pseudorapidity());
	}      
    }
  return true;
}

bool Vbf_Analysis::book()
{
  PRINT_INFO("MyFirstBase_%s::book()","book histograms\n",
	     this->name().c_str());
  bool bookedAny(false);

  int netaBins = 120;
  double etaMin = -6.;
  double etaMax =  6.;

  double detaMin = -12.;
  double detaMax = 12.;

  int nptBins = 100;
  double ptMin = 0.;
  double ptMax = 500.;

  int nmBins = 400;
  double mMin = 0.;
  double mMax = 4000.;

  int njBins = 30;
  double njMin = -0.5;
  double njMax = 29.5;

  int phiBins = 64;
  double phiMin = -3.2;
  double phiMax = 3.2;

  int ndisBins = 100;

  int ndecayBins = 31;
  double decayMin = -0.5;
  double decayMax = 30.5;

  int npidBins = 51;
  double pidMin = -25.5;
  double pidMax = 25.5;

  //  h_jets_eta_pup = Services::Histogramming::book<TH1D>(this->name(),
  //						   "EtaJetpup","Eta of pileup jets",			
  //	       
  //						   netaBins,etaMin,etaMax,"#eta","dN/d#eta");
  //
  //  h_jets_phi_pup = Services::Histogramming::book<TH1D>(this->name(),
  //						   "PhiJetpup","Phi of pileup jets",
  //						   phiBins,phiMin,phiMax,"#phi","dN/d#phi");
  //  h_jets_pt_pup =  Services::Histogramming::book<TH1D>(this->name(),
  //						   "PtJetpup","Pt of pileup jets",
  //						   nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  //
  //  h_eta = Services::Histogramming::book<TH1D>(this->name(),
  //					      "Eta","Eta of all particles",
  //				      netaBins,etaMin,etaMax,"#eta","dN/d#eta");
  //  h_phi = Services::Histogramming::book<TH1D>(this->name(),
  //					      "Phi","Phi of all particles",
  //					      phiBins,phiMin,phiMax,"#phi","dN/d#phi");
  //  h_pt =  Services::Histogramming::book<TH1D>(this->name(),
  //					      "Pt","Pt of all particles",
  //					      nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  //
  //  h_jets_eta = Services::Histogramming::book<TH1D>(this->name(),
  //  						   "EtaJet","Eta of all jets",				  
  //     
  //  						   netaBins,etaMin,etaMax,"#eta","dN/d#eta");
  //  
  //  h_jets_phi = Services::Histogramming::book<TH1D>(this->name(),
  //						   "PhiJet","Phi of all jets",
  //						   phiBins,phiMin,phiMax,"#phi","dN/d#phi");
  //  h_jets_pt =  Services::Histogramming::book<TH1D>(this->name(),
  //						   "PtJet","Pt of all jets",
  //						   nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  //
  //  h_njets =  Services::Histogramming::book<TH1D>(this->name(),
  //						   "nJet","Number of jets per event",
  //						   njBins,njMin,njMax,"N_{jets}","dN/dN_{jets}");
  //  h_njets_sub =  Services::Histogramming::book<TH1D>(this->name(),
  //						   "nJet_sub","Number of jets_sub per event",
  //						   njBins,njMin,njMax,"N_{jets_sub}","dN/dN_{jets_sub}");
  //  h_njets_ratio =  Services::Histogramming::book<TH1D>(this->name(),
  //						   "nJet_ratio","trim ratio jets per event",
  //						   100,0,2,"N_{jets}","dN/dN_{jets}");
  //
  // new plots
  d_rho =  Services::Histogramming::book<TH2D>(this->name(),"rho","#rho versus pileup events",100,0,200,100,0,50,"Pileup Events","#rho");
  h_comb_pt =  Services::Histogramming::book<TH1D>(this->name(),"comb_pt","Combined pt",nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  h_comb_lead_pt =  Services::Histogramming::book<TH1D>(this->name(),"comb_lead_pt","Leading pt",nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  h_comb_sub_pt =  Services::Histogramming::book<TH1D>(this->name(),"comb_sub_pt","Sub Leading pt",nptBins,ptMin,ptMax, "p_{T} [GeV]","dN/dpt");
  h_comb_eta =  Services::Histogramming::book<TH1D>(this->name(),"comb_eta","Combine eta",netaBins,etaMin,etaMax,"#eta of combined jets","dN/d#eta");
  h_comb_lead_eta =  Services::Histogramming::book<TH1D>(this->name(),"comb_lead_eta","Leading eta",netaBins,etaMin,etaMax,"Lead #eta","dN/d#eta");
  h_comb_sub_eta =  Services::Histogramming::book<TH1D>(this->name(),"comb_sub_eta","Sub leading eta",netaBins,etaMin,etaMax,"Sub #eta","dN/d#eta");
  // My Graphs
  d_lead_eta_diff_eta = Services::Histogramming::book<TH2D>(this->name(),"lead_eta_diff_eta","Lead Eta versus Difference Eta",netaBins,etaMin,etaMax,netaBins,detaMin,detaMax,"Lead #eta","#Delta #eta");
  h_lead_pt_minus_sub_pt = Services::Histogramming::book<TH1D>(this->name(),"lead_pt_minus_sub_pt","Lead P_{T} minus Sub P_{T}",nptBins,ptMin,ptMax,"Lead P_{T} - Sub P_{T} [GeV]","Counts");
  h_distance = Services::Histogramming::book<TH1D>(this->name(),"distance","Distance in #eta #phi Space",ndisBins,0,10,"Distance","Counts");
  h_pidsum = Services::Histogramming::book<TH1D>(this->name(),"pidsum","PID Sum",npidBins,pidMin,pidMax,"Pidsum","Counts");
  h_higgs_mass = Services::Histogramming::book<TH1D>(this->name(),"higgs_mass","Higgs Mass",nmBins,mMin,mMax,"Higgs Mass [GeV]","Counts");
  d_higgs_mass_pidsum = Services::Histogramming::book<TH2D>(this->name(),"higgs_mass_pidsum","PIDSUM versus Higgs Mass",nmBins,mMin,mMax,npidBins,pidMin,pidMax,"Higgs Mass [GeV]","PIDSUM");
  h_ndecay = Services::Histogramming::book<TH1D>(this->name(),"ndecay","Number of Higgs Decay Products",ndecayBins,decayMin,decayMax,"Number of Decays","Counts");
  d_higgs_mass_ndecay = Services::Histogramming::book<TH2D>(this->name(),"higgs_mass_ndecay","Number of Decay Products versus Higgs Mass",nmBins,mMin,mMax,ndecayBins,decayMin,decayMax,"Higgs Mass [GeV]","Decay Counts");
  d_pidsum_ndecay = Services::Histogramming::book<TH2D>(this->name(),"pidsum_ndecay","Number of Decays versus PIDSUM",npidBins,pidMin,pidMax,ndecayBins,decayMin,decayMax,"PIDSUM","ndecay");
  h_eta_cut = Services::Histogramming::book<TH1D>(this->name(),"eta_cut","Companion Jets to Jets Under 3.0 #eta",netaBins,etaMin,etaMax,"Companion Jet #eta","Counts");
  // Rejected Signal Graphs
  h_higgs_lpt = Services::Histogramming::book<TH1D>(this->name(),"higgs_lpt","p_{T} of Higgs Decay Leptons",nptBins,ptMin,ptMax,"p_{T} [GeV]","Counts");
  d_higgs_ndecay_lpt = Services::Histogramming::book<TH2D>(this->name(),"higgs_ndecay_lpt","Number of Lepton Decays versus p_{T} of Decays",ndecayBins,decayMin,decayMax,nptBins,ptMin,ptMax,"Number of Decays","p_{T} of Decays");
  d_deta_dphi = Services::Histogramming::book<TH2D>(this->name(),"deta_dphi","#Delta #phi versus #Delta #eta",netaBins,(etaMin-6),(etaMax+6),phiBins,phiMin,phiMax,"#Delta #eta","#Delta #phi");
  d_lead_sub = Services::Histogramming::book<TH2D>(this->name(),"lead_sub","Lead #eta versus Sub #eta",netaBins,etaMin,etaMax,netaBins,etaMin,etaMax,"Lead #eta","Sub #eta");

 return bookedAny;
}
