
#include "PrintDef.h"

#include "TFile.h"

#include "HistGroupEvent.h"
#include "Services.h"
#include "Selectors.h"
#include "UserAnalysisBase.h"

#include <sstream>

UserAnalysisBase::UserAnalysisBase(const std::string& name,
				   DataFrame* dataServer)
  : _name(name)
  , _signalServer(dataServer)
  , _pileupServer((DataFrame*)0)
  , _signalHandler()
  , _pileupHandler()
  , _config(Configuration::ConfigData::instance())
  , _eventPlots((HistGroupBase*)0)
  , _signalPlots((HistGroupBase*)0)
  , _pileupPlots((HistGroupBase*)0)
  , _analEvts(0)
  , _writeFreq(100)
  , _firstWrite(true)
{ }

UserAnalysisBase::UserAnalysisBase(const std::string& name,
				   DataFrame* signalServer,
				   DataFrame* pileupServer)
  : _name(name)
  , _signalServer(signalServer)
  , _pileupServer(pileupServer)
  , _signalHandler()
  , _pileupHandler()
  , _config(Configuration::ConfigData::instance())
  , _eventPlots((HistGroupBase*)0)
  , _signalPlots((HistGroupBase*)0)
  , _pileupPlots((HistGroupBase*)0)
  , _analEvts(0)
  , _writeFreq(100)
  , _firstWrite(true)
{ }

UserAnalysisBase::~UserAnalysisBase()
{ }

bool UserAnalysisBase::process()
{
  ///////////////////////
  // Setting things up //
  ///////////////////////

  PRINT_INFO("UserAnalysisBase_%s::process()","entered\n",
	     this->name().c_str());

  // copy some configuration
  long long nevts(_config->nEvents);
  int       mu(_config->mu);

  // local variables
  long long sigEntries(0);
  long long pupEntries(0);

  // number of events
  long long ntot(0);
  long long nmsg(0);
  if ( nevts < 0 )
    {
      if ( this->fHaveSignal() ) 
	{ ntot = this->fSignalServer().getMaxEvts(); nmsg = ntot; }
      else if ( this->fHavePileup() ) 
	{ ntot = this->fPileupServer().getMaxEvts(); nmsg = ntot/mu; }
      else
	{ PRINT_ERROR("UserAnalysisBase_%s::process()",
		      "no active data server!",
		      this->name().c_str());
	  Services::Process::terminate("No input data!\n");
	}
    }
  else { ntot = nevts; nmsg == ntot; }

  PRINT_INFO("UserAnalysisBase_%s::process()",
	     "analyze %i events (max)\n",
	     this->name().c_str(),(int)nmsg);

  // set up number event store
  Event pEvt(new Selectors::EtaRange(-6.,6.));

  // book histograms
  this->book();

  if ( this->fHaveMergedEvent() )
    { PRINT_INFO("UserAnalysisBase_%s::process()","analyse signal+pile-up\n",
		 this->name().c_str()); }
  else if ( this->fHaveSignal() )
    { PRINT_INFO("UserAnalysisBase_%s::process()","analyse signal only\n",
		 this->name().c_str()); }
  else if ( this->fHavePileup() )
    { PRINT_INFO("UserAnalysisBase_%s::process()","analyse pile-up only\n",
		 this->name().c_str()); }

  //////////////////////////
  // Signal only analysis //
  //////////////////////////

  if ( this->fHaveSignal() && !this->fHavePileup() )
    {
      PRINT_INFO("UserAnalysisBase_%s::process()",
		 "signal with %i interactions\n",
		 this->name().c_str(),abs(mu));
      long long ievts(0);
      while ( ievts < ntot                                            && 
	      this->fSignalHandler().fillEvent(this->fSignalServer(),
					       pEvt,sigEntries,mu,0,
					       Vertex::SIGNAL)        && 
	      this->fTicker() )
	{ this->analyze(pEvt); pEvt.reset(); ++ievts; }
    }

  //////////////////////////
  // Pileup only analysis //
  //////////////////////////

  else if ( this->fHavePileup() && !this->fHaveSignal() )
    {
      PRINT_INFO("UserAnalysisBase_%s::process()",
		 "pileup with %i interactions\n",
		 this->name().c_str(),abs(mu));
      long long ievts(0);
      while ( ievts < ntot                                             && 
	      this->fSignalHandler().fillEvent(this->fPileupServer(),
					       pEvt,pupEntries,mu,0,
					       Vertex::PILEUP)         &&
	      this->fTicker() )
	{ this->analyze(pEvt); pEvt.reset(); ++ievts;}
    }


  /////////////////////////////
  // Signal+pile-up analysis //
  /////////////////////////////
  
  else if ( this->fHaveMergedEvent() )
    {
      PRINT_INFO("UserAnalysisBase_%s::process()",
		 "single signal with %i pileup interactions\n",
		 this->name().c_str(),abs(mu));
      long long ievts(0);
      while ( ievts < ntot                                            &&
	      this->fSignalHandler().fillEvent(this->fSignalServer(),
					       pEvt,sigEntries,-1,0,
					       Vertex::SIGNAL)        &&
	      this->fPileupHandler().fillEvent(this->fPileupServer(),
					       pEvt,pupEntries,mu,1,
				       Vertex::PILEUP)                &&
	      this->fTicker() )
	{ this->analyze(pEvt); pEvt.reset(); ++ievts; }
    }
  
  return this->write();
}

bool UserAnalysisBase::analyze(Event& pEvt)
{
  // allocate all internal lists
  std::vector<fastjet::PseudoJet> pjSignal;
  std::vector<fastjet::PseudoJet> pjPileup;
  std::vector<fastjet::PseudoJet> pjEvent;

  // process flags

  ////////////
  // Signal //
  ////////////

  if ( this->fHaveSignal() )
    {
      // get signal particles 
      pjSignal = pEvt.pseudoJets(Vertex::SIGNAL);
      // fill signal plots
      this->fSignalPlots().fill(pjSignal);
    }

  ////////////
  // Pileup //
  ////////////

  if ( this->fHavePileup() )
    {
      // get pileup particles
      pjPileup = pEvt.pseudoJets(Vertex::PILEUP);
      // fill pileup plots
      this->fPileupPlots().fill(pjPileup);
    }

  //////////////////
  // Merged Event //
  //////////////////

  if ( this->fHaveMergedEvent() )
    {
      // get all particles
      pjEvent = pEvt.pseudoJets();
      // fill full event plots
      this->fEventPlots().fill(pjEvent);
    }

  return true;
}

bool UserAnalysisBase::book()
{
  PRINT_INFO("UserAnalysisBase_%s::book()","book histograms\n",
	     this->name().c_str());
  bool bookedAny(false);
  // have pile-up event
  if ( this->fHavePileup() )
    { 
      this->fSetPileupPlots(new HistGroupEvent("PileupEvent"));
      this->fPileupPlots().book();
      bookedAny = true;
    }
  // have signal event
  if ( this->fHaveSignal() )
    {
      this->fSetSignalPlots(new HistGroupEvent("SignalEvent"));
      this->fSignalPlots().book();
      bookedAny = true;
    }
  // merged event
  if ( this->fHaveMergedEvent() )
    {
      this->fSetEventPlots(new HistGroupEvent("MergedEvent"));
      this->fEventPlots().book();
      bookedAny = true;
    }
  if ( !bookedAny ) 
    { Services::Print::warning("UserAnalysisBase_%s::book()",
			       "no histograms to book!",
			       this->name().c_str()); }
  return bookedAny;
}

bool UserAnalysisBase::write()
{
  Services::Print::info("UserAnalysisBase_%s::write()", 
			"Result output after %d events analysed\n",
			this->name().c_str(),
			_analEvts);

  Services::Histogramming::write(Configuration::ConfigData::instance()->outputFile);

  _firstWrite=false;

  return true;
}

bool UserAnalysisBase::fTicker()
{
  // event counter
  ++_analEvts;

  // periodic output
  if ( _analEvts % _writeFreq == 0)
    {
      if ( !this->write() )
	{
          PRINT_ERROR("UserAnalysisBase_%s::process()", 
		      "failed to write the output. Aborting run...\n",
		      this->name().c_str());
	  Services::Process::terminate("Write to output failed!\n");
	}
      // increase the frequency as we move on
      if ( _analEvts == 15*_writeFreq ) _writeFreq *= 10;
    }

  return true;
}
