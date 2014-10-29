
#include "Services.h"
#include "Event.h"

#include "HistGroupCfg.h"
#include "HistGroupEvent.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"

#define BOOK_TH1 Services::Histogramming::book<TH1D>
#define BOOK_TH2 Services::Histogramming::book<TH2D>
#define BOOK_TP1 Services::Histogramming::book<TProfile>
#define BOOK_TP2 Services::Histogramming::book<TProfile2D>

#define PRINT_INFO Services::Print::info
#define PRINT_WARN Services::Print::warning
#define PRINT_ERROR Services::Print::error

using namespace HistGroupCfg;

HistGroupEvent::HistGroupEvent(const std::string& name)
  : HistGroupBase(name)
  , _numberOfInteractions(0)
  , h_mu((TH1D*)0)
{ }

HistGroupEvent::~HistGroupEvent()
{ }

void HistGroupEvent::book()
{
  // global
  h_mu = BOOK_TH1(this->name(),"NumberOfInteractions",
		  "Number of interactions #mu",i_muBins,x_muMin,x_muMax,
		  s_muTitleX,s_muTitleY);

  // particles
  fParticle.h_pt  = BOOK_TH1(this->name(),"ParticlePt","Particle p_{T}",
			     i_ptpBins,x_ptpMin,x_ptpMax,
			     s_ptpTitleX,s_ptpTitleY);
  fParticle.h_rap = BOOK_TH1(this->name(),"ParticleRap","Particle y",
			     i_rapBins,x_rapMin,x_rapMax,
			     s_rapTitleX,s_rapTitleY);
  fParticle.h_phi = BOOK_TH1(this->name(),"ParticlePhi","Particle #phi",
			     i_phiBins,x_phiMin,x_phiMax,
			     s_phiTitleX,s_phiTitleY);
  fParticle.d_pt_vs_rap = BOOK_TH2(this->name(),"ParticlePtVsRap",
				   "Particle p_{T}(y)",
				   i_rapBins,x_rapMin,x_rapMax,
				   i_ptpBins,x_ptpMin,x_ptpMax,
				   s_rapTitleX,s_ptpTitleX); 
  fParticle.d_phi_vs_rap = BOOK_TH2(this->name(),"ParticlePhiVsRap",
				    "Particle #phi(y)",
				    i_rapBins,x_rapMin,x_rapMax,
				    i_phiBins,x_phiMin,x_phiMax,
				    s_rapTitleX,s_phiTitleX);

  // event
  fEvent.h_multi = BOOK_TH1(this->name(),"ParticleMultiEvent", 
			    "Number of particles per event",
			    i_nptBins,x_nptMin,x_nptMax,
			    s_nptTitleX,s_nptTitleY);	     
  fEvent.h_multi_at_vtx = BOOK_TH1(this->name(),"ParticleMultiVertex",
				   "N_{part} per vertex",
				   i_vpnBins,x_vpnMin,x_vpnMax,
				   s_vpnTitleX,s_vpnTitleY);
  fEvent.h_multi_at_hardest_vtx = BOOK_TH1(this->name(),
					   "ParticleMultiHardestVertex",
					   "N_{part} at hardest vertex",
					   i_vpnBins,x_vpnMin,x_vpnMax,
					   s_vpnTitleX,s_vpnTitleY);
  fEvent.h_met =  BOOK_TH1(this->name(),"MET","Missing E_{T}",
			   i_metBins,x_metMin,x_metMax,
			   s_metTitleX,s_metTitleY);
  fEvent.h_set = BOOK_TH1(this->name(),"SETEvt","#sigma E_{T}^{evt}",
			  i_setBins,x_setMin,x_setMax,
			  s_setTitleX,s_setTitleY);
  fEvent.h_set_at_vtx = BOOK_TH1(this->name(),"SETVtx","#sigma E_{T}^{vtx}",
				 i_setBins,x_setMin,x_setMax,
				 s_setTitleX,s_setTitleY);
  fEvent.h_set_at_hardest_vtx = BOOK_TH1(this->name(),"SETAtHardestVtx",
					 "#sigma E_{T}^{hardest vtx}",
					 i_setBins,x_setMin,x_setMax,
					 s_setTitleX,s_setTitleY);
  fEvent.d_set_vs_vtx = BOOK_TH2(this->name(),"SETvsMulti",
				 "#sigma E_{T}(N_{part})",
				 i_nptBins,x_nptMin,x_nptMax,
				 i_setBins,x_setMin,x_setMax,
				 s_vpnTitleX,s_setTitleX);
  fEvent.d_vtx_vs_mu = BOOK_TH2(this->name(),"MultiEvtVsMu",
				"N_{part}(#mu)",
				i_muBins,x_muMin,x_muMax,
				i_vtxBins,x_vtxMin,x_vtxMax,
				s_muTitleX,s_vpnTitleX);
  fEvent.d_vtx_over_mu_vs_mu = BOOK_TH2(this->name(),"MultiOverMuVsMu",
					"N_{part}/#mu(#mu)",
					i_muBins,x_muMin,x_muMax,
					i_frcBins,x_frcMin,x_frcMax,
					s_muTitleX,s_frcTitleX);

  fEvent.d_ptsum_vs_rap = BOOK_TH2(this->name(),"PtEvtVsRap",
				   "p_{T}^{evt}(y)",
				   i_rapBins,x_rapMin,x_rapMax,
				   i_ptjBins,x_ptjMin,x_ptjMax,
				   s_rapTitleX,s_ptpTitleX);
  fEvent.d_multisum_vs_rap = BOOK_TH2(this->name(),"MultiEvtVsRap",
				      "N_{part}(y)",
				      i_rapBins,x_rapMin,x_rapMax,
				      i_vpnBins,x_vpnMin,x_vpnMax,
				      s_rapTitleX,s_vpnTitleX);
  fEvent.p_multisum_vs_rap_phi = BOOK_TP2(this->name(),"MultiEvtVsRapPhi",
					  "N_{part}(y,#phi)",
					  i_rapBins,x_rapMin,x_rapMax,
					  i_phiBins,x_phiMin,x_phiMax,
					  s_rapTitleX,s_phiTitleX);
  fEvent.p_ptsum_vs_rap_phi = BOOK_TP2(this->name(),"PtVsRapPhi",
				       "p_{T}^{evt}(y,#phi)",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_phiBins,x_phiMin,x_phiMax,
				       s_rapTitleX,s_phiTitleX);
  // event buffers
  std::string tmpStr(this->name());
  fAccu.h_pt_vs_rap = new TH1D((tmpStr+std::string("hist__0")).c_str(),
			       (tmpStr+std::string("hist__0")).c_str(),
			       i_rapBins,x_rapMin,x_rapMax);
  fAccu.d_pt_vs_rap_phi = new TH2D((tmpStr+std::string("dist__0")).c_str(),
				   (tmpStr+std::string("dist__0")).c_str(),
				   i_rapBins,x_rapMin,x_rapMax,
				   i_phiBins,x_phiMin,x_phiMax);
  fAccu.h_multi_vs_rap = new TH1D((tmpStr+std::string("hist__1")).c_str(),
				  (tmpStr+std::string("hist__1")).c_str(),
				  i_rapBins,x_rapMin,x_rapMax);
  fAccu.d_multi_vs_rap_phi = new TH2D((tmpStr+std::string("dist__1")).c_str(),
				      (tmpStr+std::string("dist__1")).c_str(),
				      i_rapBins,x_rapMin,x_rapMax,
				      i_phiBins,x_phiMin,x_phiMax);
}

void HistGroupEvent::fill(Event& rEvt) 
{ 
  // plot mu
  if ( h_mu == 0 ) { PRINT_WARN ("HistGroupEvent_%s::fill(...)",
				 "Histograms are not booked\n",
				 this->name().c_str());
    return;}
  _numberOfInteractions =  (double)rEvt.nVertices();
  h_mu->Fill(_numberOfInteractions);
  if ( _numberOfInteractions > 0. ) this->fill(rEvt.pseudoJets());
}

void HistGroupEvent::fill(const std::vector<fastjet::PseudoJet>& vJet)
{
  // reset/create work spaces
  fAccu.reset();

  double metx(0.); // MET component
  double mety(0.); // MET component
  double set(0.);  // SET (overall)

  std::vector<double> vtxset;  // SET (per vertex)
  std::vector<int>    vtxmult; // NPart (per vertex)

  size_t ipbm(0);  // # problematic entries
  size_t iall(0);  // # all entries

  int nvtx(0);  // vertex counter 
  int mvtx(0);  // vertex counter

  // fill distributions
  std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
  for ( ; fJet != lJet; ++fJet )
    {
      int  vtx(Features::vertexId(*fJet));
      // valid vertex
      if ( vtx >= 0 )
	{
	  ++iall;
	  // get variables
	  double pt(fJet->perp());
	  double px(fJet->px());
	  double py(fJet->py());
	    
	  double rap(fJet->rap());
	  double phi(fJet->phi_std());
	  bool   isCharged(Features::isCharged(*fJet));

	  // accumulate by event
	  metx -= px;
	  mety -= py;
	  set  += pt;

	  // accumulate by vertex
	  accumulateAtVtx(vtxmult,vtx,nvtx);
	  accumulateAtVtx(vtxset,vtx,mvtx,pt);

	  // fill for each particle
	  fParticle.h_pt->Fill(pt);
	  fParticle.h_rap->Fill(rap);
	  fParticle.h_phi->Fill(phi);
	  fParticle.d_pt_vs_rap->Fill(rap,pt);
	  fParticle.d_phi_vs_rap->Fill(rap,phi);

	  // fill store
	  fAccu.h_pt_vs_rap->Fill(rap,pt);
	  fAccu.d_pt_vs_rap_phi->Fill(rap,phi,pt);
	  fAccu.h_multi_vs_rap->Fill(rap);
	  fAccu.d_multi_vs_rap_phi->Fill(rap,phi);
	}
      else
	{ ++ipbm; }
    }

  // problematic entries
  if ( ipbm > 0 )
    {
      PRINT_WARN("HistGroupEvent_%s::fill(...)",
		 "Number of problematic entries %i > 0\n",
		 this->name().c_str(),ipbm);
    }

  // number of particles/problems
  double xall((double)iall);
  fEvent.h_multi->Fill(xall);

  // SET and MET
  double met(sqrt(metx*metx+mety*mety));
  fEvent.h_met->Fill(met);
  fEvent.h_set->Fill(set);

  // number of vertices
  double xvtx((double)nvtx);
  fEvent.d_set_vs_vtx->Fill(xvtx,set);
  fEvent.d_vtx_vs_mu->Fill(_numberOfInteractions,xvtx);
  fEvent.d_vtx_over_mu_vs_mu->Fill(_numberOfInteractions,
				   xvtx/_numberOfInteractions);

  // per vertex data
  double ptMax(0.);
  size_t idxMax(size_t(-1));
  for (size_t iv(0); iv<vtxset.size(); ++iv )
    {
      // find hardest vertex
      if ( vtxset.at(iv) > ptMax ) { ptMax = vtxset.at(iv); idxMax = iv; }
      // fill
      fEvent.h_set_at_vtx->Fill(vtxset.at(iv));
      fEvent.h_multi_at_vtx->Fill((double)vtxmult.at(iv));
    }
  if ( idxMax != size_t(-1) )
    {
      fEvent.h_set_at_hardest_vtx->Fill(vtxset.at(idxMax));
      fEvent.h_multi_at_hardest_vtx->Fill(vtxmult.at(idxMax));
    }

  // event averages
  fAccu.transfer(fAccu.h_pt_vs_rap,fEvent.d_ptsum_vs_rap);
  fAccu.transfer(fAccu.d_pt_vs_rap_phi,fEvent.p_ptsum_vs_rap_phi);
  fAccu.transfer(fAccu.h_multi_vs_rap,fEvent.d_multisum_vs_rap);
  fAccu.transfer(fAccu.d_multi_vs_rap_phi,fEvent.p_multisum_vs_rap_phi);
}

bool HistGroupEvent::accumulateAtVtx(std::vector<int>& list,int vtx,int& nvtx)
{
  // invalid index
  if ( vtx < 0 ) return false;

  if ( (size_t)vtx >= list.size() ) { ++nvtx; list.resize(vtx+1); }
  list[vtx] += 1;
  return true;
}

bool HistGroupEvent::accumulateAtVtx(std::vector<double>& list,int vtx,
				     int& nvtx,double val)
{
  // invalid index
  if ( vtx < 0 ) return false;

  if ( (size_t)vtx >= list.size() ) { ++nvtx; list.resize(vtx+1); }

  list[vtx] += val;
  return true;
}


void HistGroupEvent::_accumulators::transfer(TH1* accHist,TH1* fillHist)
{
  if ( accHist == 0 )
    { 
      if ( fillHist != 0 )
	{ PRINT_WARN("HistGroupEvent::transfer",
		     "Invalid pointer to event histogram (%p) for <%s>\n",
		     (void*)accHist,fillHist->GetName()); }
      else
	{ PRINT_WARN("HistGroupEvent::transfer",
		     "Invalid pointers to input histograms (%p/%p)\n",
		     (void*)accHist,(void*)fillHist);}
      return;
    }

  if ( fillHist == 0 )
    {
      PRINT_WARN("HistGroupEvent::transfer",
		 "Invalid pointer to fill histogram (%p) for <%s>\n",
		 (void*)fillHist,accHist->GetName());
      return;
    }
  
  int nbins(accHist->GetNbinsX());
  for ( int i(1); i<=nbins; ++i )
    {
      double x(accHist->GetBinCenter(i));
      double y(accHist->GetBinContent(i));
      fillHist->Fill(x,y);
    }
}
    
void HistGroupEvent::_accumulators::transfer(TH2* accHist,TH2* fillHist)
{
  if ( accHist == 0 )
    { 
      if ( fillHist != 0 )
	{ PRINT_WARN("HistGroupEvent::transfer",
		     "Invalid pointer to event histogram (%p) for <%s>\n",
		     (void*)accHist,fillHist->GetName()); }
      else
	{ PRINT_WARN("HistGroupEvent::transfer",
		     "Invalid pointers to input histograms (%p/%p)\n",
		     (void*)accHist,(void*)fillHist);}
      return;
    }

  if ( fillHist == 0 )
    {
      PRINT_WARN("HistGroupEvent::transfer",
		 "Invalid pointer to fill histogram (%p) for <%s>\n",
		 (void*)fillHist,accHist->GetName());
      return;
    }

  int xbins(accHist->GetNbinsX());
  int ybins(accHist->GetNbinsY());
  for ( int i(1); i<=xbins; ++i )
    {
      double x(accHist->GetXaxis()->GetBinCenter(i));
      for ( int j(1); j<=ybins; ++j )
	{
	  double y(accHist->GetYaxis()->GetBinCenter(j));
	  double c(accHist->GetBinContent(i,j));
	  fillHist->Fill(x,y,c);
	}
    }
}
