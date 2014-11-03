// -*- c++ -*-
#ifndef OBJECTSELECTORS_H
#define OBJECTSELECTORS_H

#include "ITowerSelector.h"
#include "IPJSelector.h"

#include <limits>

namespace Selectors
{
  namespace Tower
  {
    class PtThreshold : virtual public ITowerSelector
    {
    public:
      
      PtThreshold(double ptMin=0.5);
      PtThreshold(const PtThreshold& ptThresh);
      virtual ~PtThreshold();

      virtual bool accept(const tower_t& tower) const;

      double ptMin() const;

    protected:

      double _ptMin;
    };

    class PtRange : virtual public ITowerSelector
    {
    public:
      
      PtRange(double ptMin=0.5,double ptMax=std::numeric_limits<double>::max());
      PtRange(const PtRange& ptRange);
      virtual ~PtRange();

      virtual bool accept(const tower_t& tower) const;

      double ptMin() const;
      double ptMax() const;

    protected:

      double _ptMin;
      double _ptMax;
    };

    class EtaRange : virtual public ITowerSelector
    {
    public:

      EtaRange(double etaAbs=5.);
      EtaRange(double etaMin,double etaMax);
      EtaRange(const EtaRange& etaRange);

      virtual ~EtaRange();

      virtual bool accept(const tower_t& tower) const;

      bool   isAbsMode() const;
      double etaMin()    const;
      double etaMax()    const;

    protected:

      bool   _isAbsMode;
      double _etaMin;
      double _etaMax;
    };

    class PhaseSpaceSelector : virtual public ITowerSelector
    {
    public:
      
      PhaseSpaceSelector();
      PhaseSpaceSelector(const PtThreshold& ptThresh,const EtaRange& etaRange);
      PhaseSpaceSelector(const PtRange& ptRange,const EtaRange& etaRamge);
      virtual ~PhaseSpaceSelector();

      virtual bool accept(const tower_t& tower) const; 

    protected:

      const PtRange*     _ptRange;
      const PtThreshold* _ptThreshold;
      const EtaRange*    _etaRange;

      bool               _hasPtRange;
    };
  }

  namespace PseudoJet
  {
    class PtThreshold : virtual public IPJSelector
    {
    public:
      
      PtThreshold(double ptMin=0.5);
      PtThreshold(const PtThreshold& ptThresh);
      virtual ~PtThreshold();

      virtual bool accept(const fastjet::PseudoJet& pseudoJet) const;

      double ptMin() const;

    protected:

      double _ptMin;
    };

    class PtRange : virtual public IPJSelector
    {
    public:
      
      PtRange(double ptMin=0.5,double ptMax=std::numeric_limits<double>::max());
      PtRange(const PtRange& ptRange);
      virtual ~PtRange();

      virtual bool accept(const fastjet::PseudoJet& pseudoJet) const;

      double ptMin() const;
      double ptMax() const;

    protected:

      double _ptMin;
      double _ptMax;
    };

    class EtaRange : virtual public IPJSelector
    {
    public:

      EtaRange(double etaAbs=5.);
      EtaRange(double etaMin,double etaMax);
      EtaRange(const EtaRange& etaRange);

      virtual ~EtaRange();

      virtual bool accept(const fastjet::PseudoJet& pseudoJet) const;

      bool   isAbsMode() const;
      double etaMin()    const;
      double etaMax()    const;

    protected:

      bool   _isAbsMode;
      double _etaMin;
      double _etaMax;
    };

    class PhaseSpaceSelector : virtual public IPJSelector
    {
    public:
      
      PhaseSpaceSelector();
      PhaseSpaceSelector(const PtThreshold& ptThresh,const EtaRange& etaRange);
      PhaseSpaceSelector(const PtRange& ptRange,const EtaRange& etaRamge);
      virtual ~PhaseSpaceSelector();

      virtual bool accept(const fastjet::PseudoJet& pseudoJet) const; 

    protected:

      const PtRange*     _ptRange;
      const PtThreshold* _ptThreshold;
      const EtaRange*    _etaRange;

      bool               _hasPtRange;
    };
  }
}

inline double Selectors::Tower::PtThreshold::ptMin() const { return _ptMin; }

inline double Selectors::Tower::PtRange::ptMin() const { return _ptMin; }
inline double Selectors::Tower::PtRange::ptMax() const { return _ptMax; }

inline bool   Selectors::Tower::EtaRange::isAbsMode() const { return _isAbsMode; }
inline double Selectors::Tower::EtaRange::etaMin()    const { return _etaMin; }
inline double Selectors::Tower::EtaRange::etaMax()    const { return _etaMax; }

inline double Selectors::PseudoJet::PtThreshold::ptMin() const { return _ptMin; }

inline double Selectors::PseudoJet::PtRange::ptMin() const { return _ptMin; }
inline double Selectors::PseudoJet::PtRange::ptMax() const { return _ptMax; }

inline bool   Selectors::PseudoJet::EtaRange::isAbsMode() const { return _isAbsMode; }
inline double Selectors::PseudoJet::EtaRange::etaMin()    const { return _etaMin; }
inline double Selectors::PseudoJet::EtaRange::etaMax()    const { return _etaMax; }
#endif
