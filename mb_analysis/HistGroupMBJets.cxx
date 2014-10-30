
#include "BookDef.h"
#include "PrintDef.h"

#include "Services.h"

#include "HistGroupCfg.h"
#include "HistGroupMBJets.h"

#include <algorithm>

#define CHECK( _x_  )					\
  if ( _x_ == 0 )					\
    { PRINT_WARN("HistGroupMBJets_JetPlotter",		\
		 "missing histogram %s\n",#_x_		\
		 );					\
      _isOk = false; }
			       

using namespace HistGroupCfg;

HistGroupMBJets::HistGroupMBJets()
  : HistGroupBase("HistGroupMBJets")
  , _jvfThreshold(0.5)
  , _coreRadius(0.2)
  , _jetPlotter(this->name())
  , _evtPlotter(this->name())
  , _isBooked(false)
{ }

HistGroupMBJets::HistGroupMBJets(const std::string& name,double jvfThreshold,
				 double coreRadius)
  : HistGroupBase(name)
  , _jvfThreshold(jvfThreshold)
  , _coreRadius(coreRadius)
  , _jetPlotter(name)
  , _evtPlotter(name)
  , _isBooked(false)
{ }

HistGroupMBJets::~HistGroupMBJets()
{ }

void HistGroupMBJets::book()
{
  if ( _isBooked ) return;
  //
  _jetPlotter.h_multi = BOOK_TH1(this->name(),
			       "JetMultiplicity","N_{jet}",
			       i_jmlBins,x_jmlMin,x_jmlMax,
			       s_jmlTitleX,s_jmlTitleY);
  _jetPlotter.h_pt = BOOK_TH1(this->name(),
			    "JetN","p_{T}^{jet}",
			    i_ptjBins,x_ptjMin,x_ptjMax,
			    s_ptjTitleX,s_ptjTitleY);
  _jetPlotter.h_rap = BOOK_TH1(this->name(),"JetRap","y^{jet}",
			     i_rapBins,x_rapMin,x_rapMax,
			     s_rapTitleX,s_rapTitleY);
  _jetPlotter.h_jvf = BOOK_TH1(this->name(),"JetJVF","JVF^{jet}",
			     i_jvfBins,x_jvfMin,x_jvfMax,
			     s_jvfTitleX,s_jvfTitleY);
  _jetPlotter.h_nvtx = BOOK_TH1(this->name(),"JetNVtx","N_{vtx}^{jet}",
			      i_vpnBins,x_vpnMin,x_vpnMax,
			      s_vpnTitleX,s_vpnTitleY);
  _jetPlotter.h_core = BOOK_TH1(this->name(),"JetCoreFration","#Psi",
			      i_psiBins,x_psiMin,x_psiMax,
			      s_psiTitleX,s_psiTitleY);
  _jetPlotter.d_phi_vs_rap = BOOK_TH2(this->name(),"JetPhiVsRap",
				      "#phi^{jet}(y)",
				      i_rapBins,x_rapMin,x_rapMax,
				      i_phiBins,x_phiMin,x_phiMax,
				      s_rapTitleX,s_phiTitleX);
  _jetPlotter.d_pt_vs_rap = BOOK_TH2(this->name(),"JetPtVsRap",
				     "p_{T}^{jet}(y)",
				     i_rapBins,x_rapMin,x_rapMax,
				     i_ptjBins,x_ptjMin,x_ptjMax,
				     s_rapTitleX,s_ptjTitleX);
  _jetPlotter.d_jvf_vs_rap = BOOK_TH2(this->name(),"JetJVFVsRap",
				      "JVF^{jet}(y)",
				      i_rapBins,x_rapMin,x_rapMax,
				      i_jvfBins,x_jvfMin,x_jvfMax,
				      s_rapTitleX,s_jvfTitleX);
  _jetPlotter.d_core_vs_rap = BOOK_TH2(this->name(),"JetPsiVsRap","#Psi(y)",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_psiBins,x_psiMin,x_psiMax,
				       s_rapTitleX,s_psiTitleX);
  _jetPlotter.d_nvtx_vs_rap = BOOK_TH2(this->name(),"JetNvtxVsRap",
				       "N_{vtx}^{jet}(y)",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_vpnBins,x_vpnMin,x_vpnMax,
				       s_rapTitleX,s_vpnTitleX);
  _jetPlotter.d_jvf_vs_pt = BOOK_TH2(this->name(),"JetJVFVsPt",
				     "JVF^{jet}(p_{T}^{jet}",
				     i_ptjBins,x_ptjMin,x_ptjMax,
				     i_jvfBins,x_jvfMin,x_jvfMax,
				     s_ptjTitleX,s_jvfTitleX);
  _jetPlotter.d_core_vs_pt = BOOK_TH2(this->name(),"JetPsiVsPt",
				      "#Psi^{jet}(p_{T}^{jet})",
				      i_ptjBins,x_ptjMin,x_ptjMax,
				      i_psiBins,x_psiMin,x_psiMax,
				      s_ptjTitleX,s_psiTitleX);
  _jetPlotter.d_pt_vs_nvtx = BOOK_TH2(this->name(),"JetNvtxVsPt",
				      "p_{T}^{jet}(N_{vtx}^{jet})",
				      i_ptjBins,x_ptjMin,x_ptjMax,
				      i_vpnBins,x_vpnMin,x_vpnMax,
				      s_ptjTitleX,s_vpnTitleX);
				    
  _jetPlotter.d_jvf_vs_nvtx = BOOK_TH2(this->name(),"JetJVFVsNvtx",
				     "JVF^{jet}(N_{vtx}^{jet})",
				     i_vpnBins,x_vpnMin,x_vpnMax,
				     i_jvfBins,x_jvfMin,x_jvfMax,
				     s_vpnTitleX,s_jvfTitleX);
  _jetPlotter.d_core_vs_nvtx = BOOK_TH2(this->name(),"JetPsiVsNvtx",
				      "#Psi(N_{vtx}^{jet}",
				      i_vpnBins,x_vpnMin,x_vpnMax,
				      i_psiBins,x_psiMin,x_psiMax,
				      s_vpnTitleX,s_psiTitleX);
  _jetPlotter.d_core_vs_jvf = BOOK_TH2(this->name(),"JetPsiVsJVF",
				     "#Psi(JVF^{jet})",
				     i_psiBins,x_psiMin,x_psiMax,
				     i_jvfBins,x_jvfMin,x_jvfMax,
				     s_psiTitleX,s_jvfTitleX);
  _jetPlotter.z_jvf_vs_pt_rap = BOOK_TH3(this->name(),"JetJVFVsPtRap",
				       "JVF^{jet}(y^{jet},p_{T}^{jet})",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_ptjBins,x_ptjMin,x_ptjMax,
				       i_jvfBins,x_jvfMin,x_jvfMax,
				       s_rapTitleX,s_ptjTitleX,s_jvfTitleX);
  _jetPlotter.z_core_vs_pt_rap = BOOK_TH3(this->name(),"JetPsiVsPtRap",
					"#Psi(y^{jet},p_{T}^{jet})",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_ptjBins,x_ptjMin,x_ptjMax,
				       i_psiBins,x_psiMin,x_psiMax,
				       s_rapTitleX,s_ptjTitleX,s_psiTitleX);
  _jetPlotter.z_nvtx_vs_pt_rap = BOOK_TH3(this->name(),"JetNvtxVsPtRap",
					"N_{vtx}^{jet}(y^{jet},p_{T}^{jet})",
				       i_rapBins,x_rapMin,x_rapMax,
				       i_ptjBins,x_ptjMin,x_ptjMax,
				       i_vpnBins,x_vpnMin,x_vpnMax,
				       s_rapTitleX,s_ptjTitleX,s_vpnTitleX);
  _jetPlotter.z_pt_vs_jvf_nvtx = BOOK_TH3(this->name(),"JetPtVsJVFNvtx",
					"p_{T}^{jet}(N_{vtx}^{jet},JVF^{jet})",
					i_vpnBins,x_vpnMin,x_vpnMax,
					i_jvfBins,x_jvfMin,x_jvfMax,
					i_ptjBins,x_ptjMin,x_ptjMax,
					s_vpnTitleX,s_jvfTitleX,s_ptjTitleX);
  _jetPlotter.z_core_vs_jvf_nvtx = BOOK_TH3(this->name(),"JetPsiVsJVFNvtx",
					  "#Psi(N_{vtx}^{jet},JVF^{jet})",
					  i_vpnBins,x_vpnMin,x_vpnMax,
					  i_jvfBins,x_jvfMin,x_jvfMax,
					  i_psiBins,x_psiMin,x_psiMax,
					  s_vpnTitleX,s_jvfTitleX,s_psiTitleX);
  // event plots
  _evtPlotter.d_ptsum_vs_rap = BOOK_TP1(this->name(),"EvtPtJetVsRap",
					"#sum_{evt}p_{T}^{jet}(y)",
					i_rapBins,x_rapMin,x_rapMax,
					s_rapTitleX,s_ptjTitleX);
  _evtPlotter.d_multisum_vs_rap = BOOK_TP1(this->name(),"EvtNjetsVsRap",
					   "N_{jet}(y)",
					   i_rapBins,x_rapMin,x_rapMax,
					   s_rapTitleX,s_jmlTitleX);
  _evtPlotter.d_cache_ptsum_vs_rap = 
    (TH1D*)_evtPlotter.d_ptsum_vs_rap->Clone(std::string(this->name()+"__x0").c_str());
  _evtPlotter.d_cache_multisum_vs_rap =
    (TH1D*)_evtPlotter.d_multisum_vs_rap->Clone(std::string(this->name()+
							    "__x1").c_str());

  _isBooked = _jetPlotter.isOk() && _evtPlotter.isOk();
  if ( !_isBooked )
    {
      PRINT_WARN("HistGroupMBJets_%s::book()",
		 "booking was not successful, flag not reset!",
		 this->name().c_str());
    }
}

void HistGroupMBJets::fill()
{ this->fill(this->eventCache()); }

void HistGroupMBJets::fill(Event& pEvt)
{
//   PRINT_INFO("HistGroupMBJets_%s::fill(...)",
// 	     "fill from event %p with parent at %p\n",this->name().c_str(),
// 	     (void*)(&pEvt),(void*)pEvt.parent());
  // find hardest vertex (by ptsum2)
  int nvtx(0);
  int vtxId(-42);
  Event* parent = pEvt.parent();
  if ( parent != 0 )
    {
      const std::vector<Vertex>& vtxLst = parent->vertices();
//       PRINT_INFO("HistGroupMBJets_%s::fill(...)",
// 		 "found %i vertices in parent\n",
// 		 this->name().c_str(),(int)vtxLst.size());
      double ptSum2(0.);
      size_t vtxMax(size_t(-1));
      std::vector<Vertex>::const_iterator fVtx(vtxLst.begin());
      std::vector<Vertex>::const_iterator lVtx(vtxLst.end());
      for ( ; fVtx != lVtx; ++fVtx )
	{ if ( fVtx->ptSum2() > ptSum2 ) 
	    { ptSum2 = fVtx->ptSum2(); vtxMax = fVtx-vtxLst.begin(); }
	}
      if ( vtxMax == size_t(-1) )
	{ 
	  PRINT_WARN("HistGroupMBJets::fill(...)",
		     "Cannot find hardest vertex!\n"); 
	  return;
	}
      else
	{ vtxId = vtxLst.at(vtxMax).id(); }
    }

  std::vector<fastjet::PseudoJet> pJet = pEvt.pseudoJets();

//   if ( !_jetPlotter.isOk() )
//     {
//       PRINT_WARN("HistGroupMBJets_%s::fill(...)",
// 		 "incomplete booking in jet plots at %p - try again!\n",
// 		 this->name().c_str(),(void*)&_jetPlotter);
//       this->book();
//       if ( !_jetPlotter.isOk() )
// 	{
// 	  PRINT_ERROR("HistGroupMBJets_%s::fill(...)",
// 		      "failed to book jet plots at %p - try again!\n",
// 		      this->name().c_str(),(void*)&_jetPlotter);
// 	  Services::Process::terminate("Invalid pointers to histograms!");
// 	}
//     }
  double njets((double)pJet.size()); 
  _jetPlotter.h_multi->Fill(njets);
  //
  _evtPlotter.reset(false);
  //
  std::vector<fastjet::PseudoJet>::iterator fJet(pJet.begin());
  std::vector<fastjet::PseudoJet>::iterator lJet(pJet.end());

  for ( ; fJet != lJet; ++fJet )
    {
      // jet kinematics
      double pt(fJet->perp());
      double rap(fJet->rap());
      double phi(fJet->phi_std());

      // fill kinematics
      _jetPlotter.h_pt->Fill(pt);
      _jetPlotter.h_rap->Fill(rap);

      // get information on jet
      std::vector<fastjet::PseudoJet> jCons = fJet->constituents();
      std::vector<fastjet::PseudoJet>::iterator fCons(jCons.begin());
      std::vector<fastjet::PseudoJet>::iterator lCons(jCons.end());
      double ptHardest(0.);
      double coreE(0.);
      std::vector<int> vids;
      for ( ; fCons != lCons; ++fCons )
	{ 
	  // core energy
	  if ( fCons->delta_R(*fJet) < _coreRadius ) { coreE += fCons->e(); }
	  // vertex
	  int idv(Features::vertexId(*fCons));
	  if ( std::find(vids.begin(),vids.end(),idv) == vids.end() )
	    { vids.push_back(idv); }
	  if ( idv  == vtxId ) ptHardest += fCons->perp();
	}

      // calculate more quantities
      double jvf(ptHardest/pt);
      double jvx((double)vids.size());
      double cfr(coreE/fJet->e());
      
      _jetPlotter.h_jvf->Fill(jvf);
      _jetPlotter.h_nvtx->Fill(jvx);
      _jetPlotter.h_core->Fill(cfr);

      _jetPlotter.d_phi_vs_rap->Fill(rap,phi);
      _jetPlotter.d_pt_vs_rap->Fill(rap,pt);
      _jetPlotter.d_jvf_vs_rap->Fill(rap,jvf);
      _jetPlotter.d_core_vs_rap->Fill(rap,cfr);
      _jetPlotter.d_nvtx_vs_rap->Fill(rap,jvx);

      _jetPlotter.d_jvf_vs_pt->Fill(pt,jvf);
      _jetPlotter.d_core_vs_pt->Fill(pt,cfr);

      _jetPlotter.d_pt_vs_nvtx->Fill(jvx,pt);				    
      _jetPlotter.d_jvf_vs_nvtx->Fill(jvx,jvf);
      _jetPlotter.d_core_vs_nvtx->Fill(jvx,cfr);

      _jetPlotter.d_core_vs_jvf->Fill(jvf,cfr);

      _jetPlotter.z_jvf_vs_pt_rap->Fill(rap,pt,jvf);
      _jetPlotter.z_core_vs_pt_rap->Fill(rap,pt,cfr);
      _jetPlotter.z_nvtx_vs_pt_rap->Fill(rap,pt,jvx);
      
      _jetPlotter.z_pt_vs_jvf_nvtx->Fill(jvx,jvf,pt);
      _jetPlotter.z_core_vs_jvf_nvtx->Fill(jvx,jvf,cfr);
  
      // event plots (cache)
      _evtPlotter.d_cache_ptsum_vs_rap->Fill(rap,pt);
      _evtPlotter.d_cache_multisum_vs_rap->Fill(rap);
    }

  _evtPlotter.transfer();

}

HistGroupMBJets::_JetPlotter::_JetPlotter(const std::string& name)
  : h_multi((TH1D*)0)
  , h_pt((TH1D*)0)
  , h_rap((TH1D*)0)
  , h_jvf((TH1D*)0)
  , h_nvtx((TH1D*)0)
  , h_core((TH1D*)0)
  , d_phi_vs_rap((TH2D*)0)
  , d_pt_vs_rap((TH2D*)0)
  , d_jvf_vs_rap((TH2D*)0)
  , d_core_vs_rap((TH2D*)0)
  , d_nvtx_vs_rap((TH2D*)0)
  , d_jvf_vs_pt((TH2D*)0)
  , d_core_vs_pt((TH2D*)0)
  , d_pt_vs_nvtx((TH2D*)0)
  , d_jvf_vs_nvtx((TH2D*)0)
  , d_core_vs_nvtx((TH2D*)0)
  , d_core_vs_jvf((TH2D*)0)
  , z_jvf_vs_pt_rap((TH3D*)0)
  , z_core_vs_pt_rap((TH3D*)0)
  , z_nvtx_vs_pt_rap((TH3D*)0)
  , z_pt_vs_jvf_nvtx((TH3D*)0)
  , z_core_vs_jvf_nvtx((TH3D*)0)
  , _isOk(false)
  , _name(name)
{ }

HistGroupMBJets::_JetPlotter::~_JetPlotter()
{ }

bool HistGroupMBJets::_JetPlotter::isOk()
{
  if ( !_isOk )
    {
      _isOk = true;
      CHECK( h_multi )
	CHECK( h_pt )
	CHECK( h_rap )
	CHECK( h_jvf )
	CHECK( h_nvtx )
	CHECK( h_core )
	CHECK( d_phi_vs_rap )
	CHECK( d_pt_vs_rap )
	CHECK( d_jvf_vs_rap )
                             
 	CHECK( d_jvf_vs_rap )     
 	CHECK( d_core_vs_rap )    
 	CHECK( d_nvtx_vs_rap )    
 	CHECK( d_jvf_vs_pt )      
 	CHECK( d_core_vs_pt )     
 	CHECK( d_pt_vs_nvtx )     
 	CHECK( d_jvf_vs_nvtx )    
 	CHECK( d_core_vs_nvtx )   
 	CHECK( d_core_vs_jvf )    
 	CHECK( z_jvf_vs_pt_rap )  
 	CHECK( z_core_vs_pt_rap ) 
 	CHECK( z_nvtx_vs_pt_rap ) 
 	CHECK( z_pt_vs_jvf_nvtx ) 
 	CHECK( z_core_vs_jvf_nvtx )
    }
  return _isOk;
}

const std::string& HistGroupMBJets::_JetPlotter::name()
{ return _name; }

void HistGroupMBJets::_JetPlotter::reset()
{ 
  h_multi->Reset();          
  h_pt->Reset();             
  h_rap->Reset();            
  h_jvf->Reset();            
  h_nvtx->Reset();           
  h_core->Reset();           
  d_phi_vs_rap->Reset();     
  d_pt_vs_rap->Reset();      
  d_jvf_vs_rap->Reset();     
  d_core_vs_rap->Reset();    
  d_nvtx_vs_rap->Reset();    
  d_jvf_vs_pt->Reset();      
  d_core_vs_pt->Reset();     
  d_pt_vs_nvtx->Reset();     
  d_jvf_vs_nvtx->Reset();    
  d_core_vs_nvtx->Reset();   
  d_core_vs_jvf->Reset();    
  z_jvf_vs_pt_rap->Reset();  
  z_core_vs_pt_rap->Reset(); 
  z_nvtx_vs_pt_rap->Reset(); 
  z_pt_vs_jvf_nvtx->Reset(); 
  z_core_vs_jvf_nvtx->Reset();
}

HistGroupMBJets::_EventPlotter::_EventPlotter(const std::string& name)
  : d_ptsum_vs_rap((TProfile*)0)
  , d_multisum_vs_rap((TProfile*)0)
  , d_cache_ptsum_vs_rap((TH1D*)0)
  , d_cache_multisum_vs_rap((TH1D*)0)
  ,_isOk(false)
  , _name(name)
{ }

HistGroupMBJets::_EventPlotter::~_EventPlotter()
{ }

bool HistGroupMBJets::_EventPlotter::isOk()
{
  if ( !_isOk )
    {
      _isOk = 
	d_ptsum_vs_rap != 0        &&
	d_multisum_vs_rap != 0     &&
	d_cache_ptsum_vs_rap != 0  &&
	d_cache_multisum_vs_rap != 0;
    }
  return _isOk;
}

void HistGroupMBJets::_EventPlotter::reset(bool everything)
{
  if ( everything )
    { 
      d_ptsum_vs_rap->Reset();
      d_multisum_vs_rap->Reset();
    }
  d_cache_ptsum_vs_rap->Reset();
  d_cache_multisum_vs_rap->Reset();
}

const std::string& HistGroupMBJets::_EventPlotter::name()
{ return _name; }

bool HistGroupMBJets::_EventPlotter::transfer()
{
  if ( !isOk() ) return false;
  //
  return 
    transfer(d_cache_ptsum_vs_rap,d_ptsum_vs_rap) &&
    transfer(d_cache_multisum_vs_rap,d_multisum_vs_rap);
}

bool HistGroupMBJets::_EventPlotter::transfer(TH1D* tmp,TProfile* prof)
{
  for ( int i(1); i<=tmp->GetNbinsX(); ++i )
    {
      double x(tmp->GetBinCenter(i));
      double y(tmp->GetBinContent(i));
      prof->Fill(x,y);
    }
  return true;
}
