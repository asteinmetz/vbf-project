// -*- c++ -*-
#ifndef USERANALYSISBASE_H
#define USERANALYSISBASE_H

#include "Configuration.h"
#include "HistGroup.h"
#include "Event.h"
#include "DataFrame.h"

// -- commonly used histogram types for derived classes -- 
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

class UserAnalysisBase 
{
public:

  /*! @brief Single server constructor
   *
   *  This constructor is useful if only one input stream is used. In this 
   *  case the default event type is @c Vertex::SIGNAL. Collisions are merged
   *  according to the configured @c mu. 
   *
   *  @param[in] name       name of analysis 
   *  @param[in] dataServer pointer to data server 
   */
  UserAnalysisBase(const std::string& name,DataFrame* dataServer);
  /*! @brief Dual server constructor
   *
   *  This constructor can be used if two input streams are used. In this 
   *  case the first data stream argument specifies the @c Vertex::SIGNAL
   *  events, while the second argument specifies the @c Vertex::PILEUP
   *  events. Note that the configured @c mu only acts on the pile-up 
   *  stream. The number of signal events per merged event is fixed to 1. 
   */
  UserAnalysisBase(const std::string& name,DataFrame* signalServer,
		   DataFrame* pileupServer);
  /* @brief Base class destructor */
  virtual ~UserAnalysisBase();

  /*! @brief Process loop
   *
   *  This method executes the principal process loop. While it can be
   *  overwritten by clients, it is highly recommended to only have this done
   *  by experts.  
   *
   *  This method loops on the input stream(s) according to the configuration
   *  specified at construction. For each completely converted input event,
   *  which features
   *
   *  - @i single @i stream @i input : @c mu collisions per event;
   *  - @i dual @i stream @i input :   1 signal collision + @c mu pile-up 
   *                                   collisions per event,
   * 
   *  the @c UserAnalysisBase::analysis method is invoked with a reference
   *  to the actual @c Event. The @c Event cache is cleared after that. In
   *  addition, the @c fTicker method is invoked for regular logging and/or
   *  output file updates.
   */
  virtual bool process();

  /*! @brief Booking
   *
   *  This method should be implemented by the client. By default, a set 
   *  of histograms defined in @c HistGroupEvent is booked for each available
   *  input stream and the merged event (if available) separately. 
   */
  virtual bool book();
  /*! @brief Analysis
   *
   *  This method should be implemented by the client. The default 
   *  implementation fills the @c HistGroupEvent groups of histograms are
   *  filled.
   */
  virtual bool analyze(Event& pEvt);
  /* @brief Write output
   *
   * Writes to the output file.
   */
  virtual bool write();

  /*! @brief Returns analysis name */
  const std::string& name() const;

private:

  std::string _name;
  
  DataFrame*  _signalServer;
  DataFrame*  _pileupServer;

  DataHandler<DataFrame> _signalHandler;
  DataHandler<DataFrame> _pileupHandler;

  Configuration::ConfigData* _config;

  HistGroupBase* _eventPlots;
  HistGroupBase* _signalPlots;
  HistGroupBase* _pileupPlots;
  
protected:

  DataFrame& fSignalServer() { return *_signalServer; }
  DataFrame& fPileupServer() { return *_pileupServer; }

  DataHandler<DataFrame>& fSignalHandler() { return _signalHandler; }
  DataHandler<DataFrame>& fPileupHandler() { return _pileupHandler; }

  Configuration::ConfigData& fConfig() { return *_config; }

  virtual HistGroupBase& fEventPlots()  { return *_eventPlots; }
  virtual HistGroupBase& fPileupPlots() { return *_pileupPlots; }
  virtual HistGroupBase& fSignalPlots() { return *_signalPlots; }

  virtual void fSetEventPlots(HistGroupBase* hgevt)  { _eventPlots  = hgevt; }
  virtual void fSetPileupPlots(HistGroupBase* hgevt) { _pileupPlots = hgevt; }
  virtual void fSetSignalPlots(HistGroupBase* hgevt) { _signalPlots = hgevt; }

  virtual bool fHavePileup()      { return _pileupServer != 0; }
  virtual bool fHaveSignal()      { return _signalServer != 0; }
  virtual bool fHaveMergedEvent() { return fHavePileup() && fHaveSignal(); }

  virtual bool fTicker();


  int _analEvts;
  int _writeFreq;
  bool _firstWrite;

};
/*! @class UserAnalysisBase
 *
 *  @brief User analysis template
 *
 *  This object merges signal and pile-up events and provide a user hooks for
 *  analysis
 *  @code{.cpp}
 *    UserAnalysisBase::book();                 // book histograms etc.
 *    UserAnalysisBase::analyze(Event& pEvt);   // analysis
 *    UserAnalysisBase::write()                 // write out histograms
 *  @endcode
 * 
 */
inline const std::string& UserAnalysisBase::name() const { return _name; }
#endif
