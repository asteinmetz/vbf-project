
#include "PrintDef.h"

#include "TFile.h"

#include "HistGroupEvent.h"
#include "Services.h"
#include "Selectors.h"
#include "MyFirstAnalysis.h"

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"

#include <sstream>
#include <algorithm>
#include "math.h"

MyFirstAnalysis::MyFirstAnalysis(const std::string& name,
				 DataFrame* dataServer)
  : UserAnalysisBase(name,dataServer)
  , h_eta(0)
  , h_phi(0)
  , h_pt(0)
{ }

MyFirstAnalysis::~MyFirstAnalysis()
{ }

bool MyFirstAnalysis::analyze(Event& pEvt)
{
  // allocate all internal lists
  std::vector<fastjet::PseudoJet> pjSignal;

  // process flags

  ////////////
  // Signal //
  ////////////

  // not really needed but jusat in case
  if ( this->fHaveSignal() )
    {
      // get signal particles 
      pjSignal = pEvt.pseudoJets(Vertex::SIGNAL);
      // loop on list of input particles
      std::vector<fastjet::PseudoJet>::iterator fPart(pjSignal.begin());
      std::vector<fastjet::PseudoJet>::iterator lPart(pjSignal.end());
      for ( ; fPart != lPart; ++fPart )
	{
	  h_eta->Fill(fPart->pseudorapidity());
	  h_phi->Fill(fPart->phi_std());
	  h_pt->Fill(fPart->perp());
	}

      // Define which jets we want
      //
      // This defines the AntiKt jet algorithm Rcone=0.4
      fastjet::JetDefinition jetDef(fastjet::antikt_algorithm,0.4);
      fastjet::ClusterSequence cseq(pjSignal,jetDef);
      //
      // Fancy way of writing all this is:
      //
      // fastjet::ClusterSequence cseq(pEvt.pseudoJets(Veretxe::SIGNAL),
      //                               fastjet::JetDefinition(fastjet::antikt_algorithm,0.4));
      //
      // get the jets with pT > 10 GeV
      std::vector<fastjet::PseudoJet> aktJets = cseq.inclusive_jets(10.);
      //
      //Pickup Number of jets
      double njets=(double)aktJets.size();
      h_njets->Fill(njets);

      // loop jets
      std::vector<fastjet::PseudoJet>::iterator fJet(aktJets.begin());
      std::vector<fastjet::PseudoJet>::iterator lJet(aktJets.end());
      for ( ; fJet != lJet; ++fJet )
	{
	  h_jets_eta->Fill(fJet->pseudorapidity());
	  h_jets_phi->Fill(fJet->phi_std());
	  h_jets_pt->Fill(fJet->perp());
	}
      // sort the jets by pT
      if ( njets >= 2. )
	{
	  std::sort(aktJets.begin(),aktJets.end(),jetPtSorter());
	  //	  printf("pT(jet0) = %5.2f, pT(jet1) = %5.2f\n",aktJets.at(0).perp(),aktJets.at(1).perp());
	  fastjet::PseudoJet z0jet = aktJets.at(0);
	  fastjet::PseudoJet z1jet = aktJets.at(1);
	  fastjet::PseudoJet zJet  = aktJets.at(0);
	  zJet += z1jet; // candidate massive particle
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
	  // mass of the combined jet
	  h_comb_mass->Fill(zJet.m());
	  // mass versus pt of combined jet
	  d_comb_mass_pt->Fill(zJet.perp(),zJet.m());
	  // combined eta and diff eta
	  d_diff_eta_comb_eta->Fill(zJet.pseudorapidity(),z0jet.pseudorapidity()-z1jet.pseudorapidity());

	  // My Graphs
	  // lead_eta versus lead_eta - sub_eta
	  d_lead_eta_diff_eta->Fill(z0jet.pseudorapidity(),z0jet.pseudorapidity()-z1jet.pseudorapidity());
	  // lead_pt versus sub_pt
	  h_lead_pt_minus_sub_pt->Fill(z0jet.perp()-z1jet.perp());
	  // Jet Distance on phi-eta plane
	  h_distance->Fill(z0jet.delta_R(z1jet));
	}
    }

  return true;
}

bool MyFirstAnalysis::book()
{
  PRINT_INFO("MyFirstBase_%s::book()","book histograms\n",
	     this->name().c_str());
  bool bookedAny(false);


  int netaBins = 120;
  double etaMin = -6.;
  double etaMax =  6.;

  int nptBins = 100;
  double ptMin = 0.;
  double ptMax = 1000.;

  int nmBins = 100;
  double mMin = 0.;
  double mMax = 1000.;

  int njBins = 30;
  double njMin = -0.5;
  double njMax = 29.5;

  int phiBins = 64;
  double phiMin = -3.2;
  double phiMax = 3.2;

  int ndisBins = 100;

  h_eta = Services::Histogramming::book<TH1D>(this->name(),
					      "Eta","Eta of all particles",
					      netaBins,etaMin,etaMax,"#eta","dN/d#eta");
  h_phi = Services::Histogramming::book<TH1D>(this->name(),
					      "Phi","Phi of all particles",
					      phiBins,phiMin,phiMax,"#phi","dN/d#phi");
  h_pt =  Services::Histogramming::book<TH1D>(this->name(),
					      "Pt","Pt of all particles",
					      nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");

  h_jets_eta = Services::Histogramming::book<TH1D>(this->name(),
						   "EtaJet","Eta of all jets",				       
						   netaBins,etaMin,etaMax,"#eta","dN/d#eta");

  h_jets_phi = Services::Histogramming::book<TH1D>(this->name(),
						   "PhiJet","Phi of all jets",
						   phiBins,phiMin,phiMax,"#phi","dN/d#phi");
  h_jets_pt =  Services::Histogramming::book<TH1D>(this->name(),
						   "PtJet","Pt of all jets",
						   nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");

  h_njets =  Services::Histogramming::book<TH1D>(this->name(),
						   "nJet","Number of jets per event",
						   njBins,njMin,njMax,"N_{jets}","dN/dN_{jets}");

  // new plots

  h_comb_pt =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_pt","Combined pt",
						   nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  h_comb_lead_pt =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_lead_pt","Leading pt",
						   nptBins,ptMin,ptMax,"p_{T} [GeV]","dN/dpt");
  h_comb_sub_pt =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_sub_pt","Sub Leading pt",	
						   nptBins,ptMin,ptMax, "p_{T} [GeV]","dN/dpt");
  h_comb_eta =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_eta","Combine eta",				   
						   netaBins,etaMin,etaMax,"#eta","dN/d#eta");

  h_comb_lead_eta =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_lead_eta","Leading eta",
						   netaBins,etaMin,etaMax,"Lead #eta","dN/d#eta");
  h_comb_sub_eta =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_sub_eta","Sub leading eta",		     
						   netaBins,etaMin,etaMax,"Sub #eta","dN/d#eta");

  h_comb_mass =  Services::Histogramming::book<TH1D>(this->name(),
						   "comb_mass","Combine mass",
						   nmBins,mMin,mMax, "mass [GeV]","dN/dm");
  d_comb_mass_pt =  Services::Histogramming::book<TH2D>(this->name(),
						   "comb_mass_pt","Combined mass versus pt",
							nptBins,ptMin,ptMax,nmBins,mMin,mMax,
							"p_{T} [GeV]","mass [GeV]");
  d_diff_eta_comb_eta =  Services::Histogramming::book<TH2D>(this->name(),
						   "diff_eta_comb_eta","Difference eta versus Combined eta",
							netaBins,etaMin,etaMax,netaBins,etaMin,etaMax,
							"Lead #eta + Sub #eta","#Delta #eta");

  // My Graphs

  d_lead_eta_diff_eta = Services::Histogramming::book<TH2D>(this->name(),
							    "lead_eta_diff_eta","Lead Eta versus Difference Eta",
							    netaBins,etaMin,etaMax,netaBins,etaMin,etaMax,
							    "Lead #eta","#Delta #eta");
  h_lead_pt_minus_sub_pt = Services::Histogramming::book<TH1D>(this->name(),
							       "lead_pt_minus_sub_pt","Lead P_{T} minus Sub P_{T}",
							       nptBins,ptMin,ptMax,
							       "Lead P_{T} - Sub P_{T} [GeV]","Counts");
  h_distance = Services::Histogramming::book<TH1D>(this->name(),
						   "distance","Distance in #eta #phi Space",
						   ndisBins,0,10,
						   "Distance","Counts");



  return bookedAny;
}
