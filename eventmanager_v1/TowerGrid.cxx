
#include "TowerGrid.h"

#include <algorithm>

#include "PrintDef.h"

#include "Services.h"

#include <cmath>

Tower::Tower(double energy,double eta,double phi)
  : _e(energy)
  , _eta(eta)
  , _phi(phi)
  , _coshEtaInv(1./std::cosh(_eta))
  , _pt(_e*_coshEtaInv)
  , _iEta(-1)
  , _iPhi(-1)
  , _pjet(fastjet::PtYPhiM(_pt,_eta,_phi,0.))
{ _PHI_CONV( _phi ) ; }

Tower::Tower()
#ifndef _NO_CPP11
  : Tower(0.,0.,0.)
{ }
#else
  : _e(0.)
  , _eta(0.)
  , _phi(0.)
  , _coshEtaInv(1.)
  , _pt(0.)
  , _iEta(-1)
  , _iPhi(-1)
  , _pjet(0.,0.,0.,0.)
{ _PHI_CONV( _phi ) ; }
#endif

Tower::~Tower()
{ }

Tower Tower::operator+(const Tower& entry)
{
  double e(_e+entry._e);
  double eta((_eta+entry._eta)/2.);

  double x(std::cos(_phi)+std::cos(entry._phi));
  double y(std::sin(_phi)+std::sin(entry._phi));
  double phi(std::atan2(y,x)); 

  return Tower(e,eta,phi);
}


TowerGrid::TowerGrid(int nEta,double etaMin,double etaMax,
		     int nPhi,double phiMin,double phiMax)
  : _nEta(nEta)
  , _etaMin(etaMin)
  , _etaMax(etaMax)
  , _deltaEta(0.)
  , _nPhi(nPhi)
  , _phiMin(phiMin)
  , _phiMax(phiMax)
  , _deltaPhi(0.)
{ if ( !this->setup() ) delete this; } 

TowerGrid::TowerGrid()
#ifdef _NO_CPP11
  : _nEta(10)
  , _etaMin(-5.)
  , _etaMax(5.)
  , _deltaEta((_etaMax-_etaMin)/((double)_nEta))
  , _nPhi(64)
  , _phiMin(-_pi)
  , _phiMax(_pi)
  , _deltaPhi((_phiMax-_phiMin)/((double)_nPhi))
{ if ( !this->setup() ) delete this; }
#else
: TowerGrid(10,-5.,5.,64,-_pi,_pi)
{ }
#endif

TowerGrid::~TowerGrid()
{ }

bool TowerGrid::add(double eta,double phi,double energy)
{ 
  int iEta(-1);
  int iPhi(-1);
  return this->checkRange(eta,phi,iEta,iPhi) && this->add(iEta,iPhi,energy);
}

// bool TowerGrid::fill(double eta,double phi,towertower_t entry)
// { 
//   int iEta = eta >  _etaMin && eta < _etaMax ? (eta-_etaMin)/_deltaEta : -1;
//   int iPhi = phi >= _phiMin && phi < _phiMax ? (phi=_phiMin)/_deltaPhi : -1;
//   return this->fill(iEta,iPhi,entry);
// }

// bool TowerGrid::fill(int iEta,int iPhi,towertower_t entry)
// {
//   size_t ie((size_t)iEta);
//   size_t ip((size_t)iPhi);
//   _towerGrid[ie][ip] += entry;
//   return true;
// }

// bool TowerGrid::set(double eta,double phi,towertower_t entry)
// { 
//   int iEta(-1);
//   int iPhi(-1);
//   return this->checkRange(eta,phi,iEta,iPhi) && this->set(iEta,iPhi,entry); 
// }

// bool TowerGrid::set(int iEta,int iPhi,towertower_t entry)
// {
//   if ( iEta < 0 || iPhi < 0 ) return false;

//   size_t ie((size_t)iEta);
//   size_t ip((size_t)iPhi);
//   _towerGrid[ie][ip] = value;
//   return true;
// }

TowerGrid::tower_t TowerGrid::entry(double eta,double phi) const
{
  int iEta(-1);
  int iPhi(-1);
  return checkRange(eta,phi,iEta,iPhi) ? _towerGrid.at(static_cast<size_t>(iEta)).at(static_cast<size_t>(iPhi)) : tower_t(); 
}

TowerGrid::tower_t TowerGrid::entry(int iEta,int iPhi) const
{ return checkRange(iEta,iPhi) ? _towerGrid.at(static_cast<size_t>(iEta)).at(static_cast<size_t>(iPhi)) : tower_t(); }

std::vector<TowerGrid::tower_t> TowerGrid::towers(bool excludeEmpty) const
{ std::vector<tower_t> t;  this->fillTowers(t,excludeEmpty); return t; }

std::vector<fastjet::PseudoJet> TowerGrid::pseudoJets(bool excludeEmpty) const
{ std::vector<fastjet::PseudoJet> pjets; this->fillPseudoJets(pjets,excludeEmpty); return pjets; }

bool TowerGrid::fillTowers(std::vector<tower_t>& towers,bool excludeEmpty) const
{
  size_t oldSize(towers.size());
  grid_t::const_iterator fGrid(_towerGrid.begin());
  grid_t::const_iterator lGrid(_towerGrid.end());

  // exclude empties
  if ( excludeEmpty )
    {
      for ( ; fGrid != lGrid; ++fGrid )
	{
	  line_t::const_iterator fLine(fGrid->begin());
	  line_t::const_iterator lLine(fGrid->end());
	  for ( ; fLine != lLine; ++fLine ) { if ( fLine->energy() > 0. ) { towers.push_back((*fLine)); } } 
	}
    }
  // include empties
  else
    {
      for ( ; fGrid != lGrid; ++fGrid )
	{
	  line_t::const_iterator fLine(fGrid->begin());
	  line_t::const_iterator lLine(fGrid->end());
	  for ( ; fLine != lLine; ++fLine ) { towers.push_back((*fLine)); } 
	}
    }

  return oldSize < towers.size();
} 

bool TowerGrid::fillPseudoJets(std::vector<fastjet::PseudoJet>& pjets,bool excludeEmpty) const
{
  size_t oldSize(pjets.size());
  grid_t::const_iterator fGrid(_towerGrid.begin());
  grid_t::const_iterator lGrid(_towerGrid.end());

  // exclude empties
  if ( excludeEmpty ) 
    {
      for ( ; fGrid != lGrid; ++fGrid )
	{
	  line_t::const_iterator fLine(fGrid->begin());
	  line_t::const_iterator lLine(fGrid->end());
	  for ( ; fLine != lLine; ++fLine ) { if ( fLine->energy() > 0. ) { pjets.push_back(fLine->pseudoJet()); } } 
	}
    }
  // include empties
  else
    { 
      for ( ; fGrid != lGrid; ++fGrid )
	{
	  line_t::const_iterator fLine(fGrid->begin());
	  line_t::const_iterator lLine(fGrid->end());
	  for ( ; fLine != lLine; ++fLine ) { pjets.push_back(fLine->pseudoJet()); } 
	}  
    }

  return oldSize < pjets.size();
}

void TowerGrid::reset()
{
  grid_t::iterator fGrid(_towerGrid.begin());
  grid_t::iterator lGrid(_towerGrid.end());
  for ( ; fGrid != lGrid; ++fGrid )
    {
      line_t::iterator fLine(fGrid->begin());
      line_t::iterator lLine(fGrid->end());
      for ( ; fLine != lLine; ++fLine ) { fLine->reset(); }
    }
}



TH2D* TowerGrid::distribution(const std::string& name,
			      const std::string& title) const
{

  PRINT_INFO("TowerGrid::distribution(\042%s\042,\042%s\042)","found tower grid with %i bins\n",name.c_str(),title.c_str(),(int)_towerGrid.size());

  // structural problem 
  if ( _towerGrid.empty() || _towerGrid.front().empty() ) return (TH2D*)0; 

  TH2D* h = Services::Histogramming::book<TH2D>(name,title,(int)_nEta,_etaMin,_etaMax,(int)_nPhi,_phiMin,_phiMax,"Tower #eta","Tower #phi","Tower p_{T}");

  // loop grid for valid towers
  if ( _towerGrid.front().front().haveIndices() )
    {
      PRINT_INFO("TowerGrid::distribution(\042%s\042,\042%s\042)","towers have indices\n",name.c_str(),title.c_str());
      grid_t::const_iterator fGrid(_towerGrid.begin());
      grid_t::const_iterator lGrid(_towerGrid.end());
      for ( ; fGrid != lGrid; ++fGrid )
	{
	  line_t::const_iterator fLine(fGrid->begin());
	  line_t::const_iterator lLine(fGrid->end());
	  for ( ; fLine != lLine; ++fLine ) 
	    { h->SetBinContent(fLine->etaIndex()+1,fLine->phiIndex()+1,fLine->pt()); }
	}
    }
  // no indices attached to towers (????)
  else
    {
      for ( size_t iEta(0); iEta < _towerGrid.size(); ++iEta )
	{
	  for ( size_t iPhi(0); iPhi < _towerGrid.at(iEta).size(); ++iPhi )
	    { h->SetBinContent(iEta+1,iPhi+1,_towerGrid.at(iEta).at(iPhi).pt()); }
	}
    }
  return h;
}

bool TowerGrid::setup()
{
  // check eta bin definitions
  if ( _etaMin >= _etaMax || _nEta == 0 )
    {
      PRINT_ERROR("TowerGrid","Invalid descriptor for pseudorapidity (# bins,min,max) = (%i,%4.1f,%4.1)\n",_nEta,_etaMin,_etaMax); 
      return false;
    }

  // check phi bin definitions
  if ( _phiMin >= _phiMax || _nPhi == 0 )
    {
      PRINT_ERROR("TowerGrid","Invalid descriptor for azimuth (# bins,min,max) = (%i,%4.1f,%4.1)\n",_nPhi,_phiMin,_phiMax); 
      return false;
    }

  // derived parameters
  _deltaEta = (_etaMax-_etaMin)/((double)_nEta);
  _deltaPhi = (_phiMax-_phiMin)/((double)_nPhi);
    
  // fill in workspace entries
  double eta(_etaMin+_deltaEta/2.);
  for ( int iEta(0); iEta<_nEta; ++iEta )
    {
      _towerGrid.push_back(line_t());
      double phi(_phiMin+_deltaPhi/2.);
      for ( int iPhi(0); iPhi<_nPhi; ++iPhi )
	{
	  Tower t(0.,eta,phi);
	  t.setIndices(iEta,iPhi);
	  _towerGrid.back().push_back(t);
	  phi += _deltaPhi;
	}
      eta += _deltaEta;
    }
  return true; 
}

// TowerExtractorBase::TowerExtractorBase(const TowerCfg& towerCfg)
//   : _config(towerCfg)  
//   , _gridTowers(towerCfg.etaBoundaries.size(),std::vector<tower_t>())
// {
//   for ( size_t i(0); i<_config.etaBoundaries.size(); ++i )
//     {
//       size_t nEta = 2*_config.etaBoundaries.at(i)/_config.deltaEta.at(i);
//       size_t nPhi = static_cast<size_t>(_twoPi/_config.deltaPhi.at(i));
//       switch ( nPhi )
// 	{
// 	case 62:
// 	case 63:
// 	case 64:
// 	  nPhi = 64; break;
// 	case 31:
// 	case 32:
// 	  nPhi = 32; break;
// 	case 15:
// 	case 16:
// 	  nPhi = 16; break;
// 	case 7:
// 	case 8:
// 	  nPhi = 8; break;
// 	default:
// 	  break;
// 	}
//       _gridTowers.at(i).resize(nphi,tower_t());
//     }
// }

// TowerExtractorBase::~TowerExtractorBase()
// { }

// bool TowerExtractorBase::fillPseudoJets(const TowerGrid& towerGrid,std::vector<fastjet::PseudoJet>& pjets) const
// { return towerGrid.fillPseudoJets(pjets); }

// bool TowerExtractorBase::fillTowers(const TowerGrid& towerGrid,std::vector<tower_t>& towers) const
// { return towerGrid.fillTowers(towers); }

