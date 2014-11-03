// -*- c++ -*-
#ifndef TOWERGRID_H
#define TOWERGRID_H

#include "TH2D.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/GhostedAreaSpec.hh"

#include <vector>
#include <utility>

#define _PHI_CONV( _X_ ) \
  if ( _X_ > _pi ) { _X_ += _twoPi; } else if ( _X_ < -_pi ) { _X_ += _twoPi; }

class ITowerSelector;
class IPJSelector;

namespace
{ 
  static double _pi     = 2.*asin(1.); 
  static double _twoPi  = 2.*_pi; 
  static double _tinyPt = fastjet::gas::def_mean_ghost_pt;  
}

class Tower
{
public:
  Tower();
  Tower(double energy,double eta,double phi);
  ~Tower();

  void add(double energy);
  void reset();

  const fastjet::PseudoJet& pseudoJet() const;

  double operator+=(double energy);
  Tower operator+(const Tower& entry); 

  double energy() const;
  double eta()    const;
  double phi()    const;
  double pt()     const;

  void setIndices(int iEta,int iPhi);

  int  etaIndex() const;
  int  phiIndex() const;

  bool haveIndices() const;

private:

  double _e;
  double _eta;
  double _phi;
  double _coshEtaInv;
  double _pt;

  int _iEta;
  int _iPhi;

  fastjet::PseudoJet _pjet;
};

inline void Tower::add(double energy) { _e += energy; _pt = _e * _coshEtaInv; _pjet.reset_PtYPhiM(_pt,_eta,_phi,0.); }
inline void Tower::reset() { _e = 0.; _pt = 0.; _pjet.reset_PtYPhiM(_tinyPt,_eta,_phi,0.); }

inline double Tower::operator+=(double energy) { add(energy); }

inline double Tower::eta()    const { return _eta; }
inline double Tower::phi()    const { return _phi; }
inline double Tower::energy() const { return _e; }
inline double Tower::pt()     const { return _pt; } 

inline const fastjet::PseudoJet&  Tower::pseudoJet()  const { return _pjet; }

inline int  Tower::etaIndex()    const { return _iEta; }
inline int  Tower::phiIndex()    const { return _iPhi; } 
inline bool Tower::haveIndices() const { return _iEta >= 0 && _iPhi >= 0; }

inline void Tower::setIndices(int iEta,int iPhi) { _iEta = iEta; _iPhi = iPhi; }

class TowerGrid
{
public: 

  typedef Tower                tower_t;
  typedef std::vector<tower_t> line_t;
  typedef std::vector<line_t>  grid_t;

  TowerGrid();
  TowerGrid(int nEta,double etaMin,double etaMax,
	    int nPhi=64,double phiMin=-_pi,double phiMax=_pi);
  virtual ~TowerGrid();

  bool add(double eta,double phi,double energy);

  // bool fill(double eta,double stdPhi,tower_t entry); 
  // bool fill(int iEta,int iPhi,tower_t entry); 

  // bool set(double eta,double phi,tower_t entry);
  // bool set(int iEta,int iPhi,tower_t entry);

  TH2D* distribution(const std::string& title,const std::string& name) const;

  tower_t entry(double eta,double phi) const;
  tower_t entry(int iEta,int iPhi)     const;

  std::vector<tower_t> towers(bool excludeEmpty=true)                         const;
  std::vector<tower_t> towers(const ITowerSelector& entrySelector)            const;
  std::vector<fastjet::PseudoJet> pseudoJets(bool excludeEmpty=true)          const;
  std::vector<fastjet::PseudoJet> pseudoJets(const IPJSelector& pjetSelector) const;

  bool fillTowers(std::vector<tower_t>& towers,bool excludeEmpty=true)               const;
  bool fillPseudoJets(std::vector<fastjet::PseudoJet>& pjets,bool excludeEmpty=true) const;

  int    etaBins() const;
  int    phiBins() const;
  
  double etaMin() const;
  double etaMax() const;

  double phiMin() const;
  double phiMax() const;

  int etaIndex(double eta) const;
  int phiIndex(double phi) const;

  void reset();

protected:

  size_t _nEta;
  double _etaMin;
  double _etaMax;
  double _deltaEta; 

  size_t _nPhi;
  double _phiMin;
  double _phiMax;
  double _deltaPhi;

  grid_t  _towerGrid;

  bool setup();

  bool checkRange(double eta,double phi,int& iEta,int& iPhi) const;
  bool checkRange(int iEta,int iPhi)                         const;
  bool checkEta(double eta,int& iEta)                        const;
  bool checkEta(int iEta)                                    const;
  bool checkPhi(double phi,int& iPhi)                        const;
  bool checkPhi(int iPhi)                                    const;

  bool add(int iEta,int iPhi,double energy);
  
};

inline int TowerGrid::etaBins() const { return (int)_nEta; }
inline int TowerGrid::phiBins() const { return (int)_nPhi; }

inline double TowerGrid::etaMin() const { return _etaMin; }
inline double TowerGrid::etaMax() const { return _etaMax; }

inline double TowerGrid::phiMin() const { return _phiMin; }
inline double TowerGrid::phiMax() const { return _phiMax; }

inline bool TowerGrid::checkEta(int iEta) const
{ return iEta >= 0 && iEta < static_cast<int>(_towerGrid.size()); }

inline bool TowerGrid::checkPhi(int iPhi) const
{ return iPhi >= 0 && iPhi < static_cast<int>(_towerGrid.begin()->size()); } 

inline bool TowerGrid::checkEta(double eta,int& iEta) const
{ iEta = eta > _etaMin && eta < _etaMax ? (eta-_etaMin)/_deltaEta : -1; return checkEta(iEta); }

inline bool TowerGrid::checkPhi(double phi,int& iPhi) const
{ 
  _PHI_CONV( phi ) ;
  iPhi = phi >= _phiMin && phi <= _phiMax ? (phi-_phiMin)/_deltaPhi : -1; 
  return checkPhi(iPhi);
}

inline bool TowerGrid::checkRange(int iEta,int iPhi) const
{ return checkEta(iEta) && checkPhi(iPhi); }

inline bool TowerGrid::checkRange(double eta,double phi,int& iEta,int& iPhi) const
{ return checkEta(eta,iEta) && checkPhi(phi,iPhi); }

inline bool TowerGrid::add(int iEta,int iPhi,double energy)
{ _towerGrid[iEta][iPhi].add(energy); } 

inline int TowerGrid::etaIndex(double eta) const 
{ int iEta(-1); checkEta(eta,iEta); return iEta; }

inline int TowerGrid::phiIndex(double phi) const
{ int iPhi(-1); checkPhi(phi,iPhi); return iPhi; }

// class ITowerExtractor
// {
// public:

//   typedef TowerGrid::tower_t tower_t;

//   virtual ~ITowerExtractor() {}

//   virtual bool fillPseudoJets(const TowerGrid& towerGrid,std::vector<fastjet::PseudoJet>& pjets) const = 0;
//   virtual bool fillTowers(const TowerGrid& towerGrid,std::vector<tower_t>& towers) const = 0;
// };

// struct TowerCfg
// {
//   std::vector<double> etaBoundaries;
//   std::vector<double> deltaEta;
//   std::vector<double> deltaPhi;

//   bool isEmpty;

//   TowerCfg() : isEmpty(true) { };
//   TowerCfg(const TowerCfg& towerCfg) 
//     : isEmpty(towerCfg.isEmpty)
//     , etaBoundaries(towerCfg.etaBoundaries.begin(),towerCfg.etaBoundaries.end())
//     , deltaEta(towerCfg.deltaEta.begin(),towerCfg.deltaEta.end())
//     , deltaPhi(towerCfg.deltaPhi.begin(),towerCfg.deltaPhi.end()) 
//   { }
// };

// class TowerExtractorBase : virtual public ITowerExtractor
// {
// public: 
  
//   TowerExtractorBase(const TowerCfg& towerCfg=TowerCfg());
//   virtual ~TowerExtractorBase();

//   virtual bool fillPseudoJets(const TowerGrid& towerGrid,std::vector<fastjet::PseudoJet>& pjets) const;
//   virtual bool fillTowers(const TowerGrid& towerGrid,std::vector<tower_t>& towers) const;
 
// protected:

//   const TowerCfg& config() const;

// private:

//   TowerCfg _config;
// };

// inline const TowerCfg& TowerExtractorBase::config() const
// { return _config; }

#endif
