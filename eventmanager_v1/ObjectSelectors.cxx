
#include "ObjectSelectors.h"

#include <limits>
#include <cmath>

////////////////////////
// Tower pT Threshold //
////////////////////////

Selectors::Tower::PtThreshold::PtThreshold(double ptMin)
  : _ptMin(ptMin)
{ }

Selectors::Tower::PtThreshold::PtThreshold(const PtThreshold& ptThresh)
  : _ptMin(ptThresh._ptMin)
{ }

Selectors::Tower::PtThreshold::~PtThreshold()
{ }

bool Selectors::Tower::PtThreshold::accept(const tower_t& tower) const
{ return tower.pt() > _ptMin; }

////////////////////
// Tower pT Range //
////////////////////

Selectors::Tower::PtRange::PtRange(double ptMin,double ptMax)
  : _ptMin(ptMin)
  , _ptMax(ptMax)
{ }

Selectors::Tower::PtRange::PtRange(const PtRange& ptRange)
#ifndef _NO_CPP11
  : PtRange(ptRange._ptMin,ptRange._ptMax)
#else
  : _ptMin(ptRange._ptMin)
  , _ptMax(ptRange._ptMax)
#endif
{ }

Selectors::Tower::PtRange::~PtRange()
{ }

bool Selectors::Tower::PtRange::accept(const tower_t& tower) const
{ return tower.pt() > _ptMin && tower.pt() < _ptMax; }

/////////////////////
// Tower Eta Range //
/////////////////////

Selectors::Tower::EtaRange::EtaRange(double etaAbs)
  : _isAbsMode(true)
  , _etaMin(etaAbs)
  , _etaMax(std::numeric_limits<double>::max())
{ }

Selectors::Tower::EtaRange::EtaRange(double etaMin,double etaMax)
  : _isAbsMode(false)
  , _etaMin(etaMin)
  , _etaMax(etaMax)
{ }

Selectors::Tower::EtaRange::EtaRange(const EtaRange& etaRange)
  : _isAbsMode(etaRange._isAbsMode)
  , _etaMin(etaRange._etaMin)
  , _etaMax(etaRange._etaMax)
{ }

Selectors::Tower::EtaRange::~EtaRange()
{ }

bool Selectors::Tower::EtaRange::accept(const tower_t& tower) const
{ return _isAbsMode ? std::abs(tower.eta()) > _etaMin : tower.eta() > _etaMin && tower.eta() < _etaMax; }

///////////////////////////////
// Tower Phasespace selector //
///////////////////////////////

Selectors::Tower::PhaseSpaceSelector::PhaseSpaceSelector()
  : _ptRange((const PtRange*)0)
  , _ptThreshold((const PtThreshold*)0)
  , _etaRange((const EtaRange*)0)
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::Tower::PhaseSpaceSelector::PhaseSpaceSelector(const PtRange& ptRange,const EtaRange& etaRange)
  : _ptRange(new PtRange(ptRange))
  , _ptThreshold((const PtThreshold*)0)
  , _etaRange(new EtaRange(etaRange))
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::Tower::PhaseSpaceSelector::PhaseSpaceSelector(const PtThreshold& ptThresh,const EtaRange& etaRange)
  : _ptRange((const PtRange*)0)
  , _ptThreshold(new PtThreshold(ptThresh))
  , _etaRange(new EtaRange(etaRange))
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::Tower::PhaseSpaceSelector::~PhaseSpaceSelector()
{
  if ( _ptRange != 0 ) { delete _ptRange; } else if ( _ptThreshold != 0 ) { delete _ptThreshold; }
  if ( _etaRange != 0 ) { delete _etaRange; } 
}

bool Selectors::Tower::PhaseSpaceSelector::accept(const tower_t& tower) const
{ return _hasPtRange 
    ? _ptRange->accept(tower) && _etaRange->accept(tower) 
    : _ptThreshold->accept(tower) && _etaRange->accept(tower); }

////////////////////////////
// PseudoJet pT Threshold //
////////////////////////////

Selectors::PseudoJet::PtThreshold::PtThreshold(double ptMin)
  : _ptMin(ptMin)
{ }

Selectors::PseudoJet::PtThreshold::PtThreshold(const PtThreshold& ptThresh)
  : _ptMin(ptThresh._ptMin)
{ }

Selectors::PseudoJet::PtThreshold::~PtThreshold()
{ }

bool Selectors::PseudoJet::PtThreshold::accept(const fastjet::PseudoJet& tower) const
{ return tower.pt() > _ptMin; }

////////////////////////
// PseudoJet pT Range //
////////////////////////

Selectors::PseudoJet::PtRange::PtRange(double ptMin,double ptMax)
  : _ptMin(ptMin)
  , _ptMax(ptMax)
{ }

Selectors::PseudoJet::PtRange::PtRange(const PtRange& ptRange)
#ifndef _NO_CPP11
  : PtRange(ptRange._ptMin,ptRange._ptMax)
#else
  : _ptMin(ptRange._ptMin)
  , _ptMax(ptRange._ptMax)
#endif
{ }

Selectors::PseudoJet::PtRange::~PtRange()
{ }

bool Selectors::PseudoJet::PtRange::accept(const fastjet::PseudoJet& tower) const
{ return tower.pt() > _ptMin && tower.pt() < _ptMax; }

/////////////////////////
// PseudoJet Eta Range //
/////////////////////////

Selectors::PseudoJet::EtaRange::EtaRange(double etaAbs)
  : _isAbsMode(true)
  , _etaMin(etaAbs)
  , _etaMax(std::numeric_limits<double>::max())
{ }

Selectors::PseudoJet::EtaRange::EtaRange(double etaMin,double etaMax)
  : _isAbsMode(false)
  , _etaMin(etaMin)
  , _etaMax(etaMax)
{ }

Selectors::PseudoJet::EtaRange::EtaRange(const EtaRange& etaRange)
  : _isAbsMode(etaRange._isAbsMode)
  , _etaMin(etaRange._etaMin)
  , _etaMax(etaRange._etaMax)
{ }

Selectors::PseudoJet::EtaRange::~EtaRange()
{ }

bool Selectors::PseudoJet::EtaRange::accept(const fastjet::PseudoJet& tower) const
{ return _isAbsMode ? std::abs(tower.eta()) > _etaMin : tower.eta() > _etaMin && tower.eta() < _etaMax; }

///////////////////////////////////
// PseudoJet Phasespace selector //
///////////////////////////////////

Selectors::PseudoJet::PhaseSpaceSelector::PhaseSpaceSelector()
  : _ptRange((const PtRange*)0)
  , _ptThreshold((const PtThreshold*)0)
  , _etaRange((const EtaRange*)0)
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::PseudoJet::PhaseSpaceSelector::PhaseSpaceSelector(const PtRange& ptRange,const EtaRange& etaRange)
  : _ptRange(new PtRange(ptRange))
  , _ptThreshold((const PtThreshold*)0)
  , _etaRange(new EtaRange(etaRange))
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::PseudoJet::PhaseSpaceSelector::PhaseSpaceSelector(const PtThreshold& ptThresh,const EtaRange& etaRange)
  : _ptRange((const PtRange*)0)
  , _ptThreshold(new PtThreshold(ptThresh))
  , _etaRange(new EtaRange(etaRange))
  , _hasPtRange(_ptRange != 0)
{ }

Selectors::PseudoJet::PhaseSpaceSelector::~PhaseSpaceSelector()
{
  if ( _ptRange != 0 ) { delete _ptRange; } else if ( _ptThreshold != 0 ) { delete _ptThreshold; }
  if ( _etaRange != 0 ) { delete _etaRange; } 
}

bool Selectors::PseudoJet::PhaseSpaceSelector::accept(const fastjet::PseudoJet& tower) const
{ return _hasPtRange 
    ? _ptRange->accept(tower) && _etaRange->accept(tower) 
    : _ptThreshold->accept(tower) && _etaRange->accept(tower); }

