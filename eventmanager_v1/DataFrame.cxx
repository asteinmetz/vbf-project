
#include "Services.h"
#include "Selectors.h"

#include "TFile.h"

#include "DataFrame.h"

#ifndef _HAVE_NO_BOOST_
#include "boost/lexical_cast.hpp"
#else
#include <sstream>
#endif

DataFrame::DataFrame(TChain* pChain)
  : fChain(pChain)
  , fCurrent(-1)
  , fEvtsProc(0)
  , fEvtsFreq(100)
  , fDataHandler(new DataHandler<DataFrame>())
  , fConfig(Configuration::ConfigData::instance())
{
  if ( fChain == 0 ) 
    { Services::Print::error("DataFrame::DataFrame(%p)",
			     "Not a valid pointer argument (TChain*)%p - will self destruct!\n",
			     (void*)pChain,(void*)fChain); 
      delete this;
    }
  this->init(pChain);
}

DataFrame::~DataFrame()
{ if ( fChain == 0 ) return; delete fChain->GetCurrentFile(); }

Int_t DataFrame::getEntry(Long64_t entry)
{ return fChain->GetEntry(entry); }

Long64_t DataFrame::loadTree(Long64_t entry)
{ 
  Long64_t jentry(fChain->LoadTree(entry));
  if ( jentry < 0 ) return jentry;
  //
  Int_t itree(fChain->GetTreeNumber());
  if ( itree != fCurrent ) { fCurrent = itree; this->notify(); }
  //
  return jentry;
}		  

void DataFrame::init(TChain* pChain)
{
  Services::Print::info("DataFrame::init(%p)","Entered for chain \042%s\042\n",
			(void*)pChain,pChain->GetName());
  // ???
  fChain->SetMakeClass(1);
  // set branch addresses
  fChain->SetBranchAddress("Nentry", &Nentry, &b_Nentry);
  fChain->SetBranchAddress("Npartons", &Npartons, &b_Npartons);
  fChain->SetBranchAddress("Nparticles", &Nparticles, &b_Nparticles);
  fChain->SetBranchAddress("ID", ID, &b_ID);
  fChain->SetBranchAddress("Stat", Stat, &b_Stat);
  fChain->SetBranchAddress("Charge", Charge, &b_Charge);
//   fChain->SetBranchAddress("Px", Px, &b_Px);
//   fChain->SetBranchAddress("Py", Py, &b_Py);
//   fChain->SetBranchAddress("Pz", Pz, &b_Pz);
//   fChain->SetBranchAddress("P0", P0, &b_P0);
  fChain->SetBranchAddress("Pm", Pm, &b_Pm);
  fChain->SetBranchAddress("Pt", Pt, &b_Pt);
  fChain->SetBranchAddress("Rap", Rap, &b_Rap);
  fChain->SetBranchAddress("Phi", Phi, &b_Phi);
//   fChain->SetBranchAddress("Eta", Eta, &b_Eta);
  this->notify();
}

Bool_t DataFrame::notify()
{ 
  if ( fCurrent < 0 )
    {
      Services::Print::info("DataFrame::init(%p)",
			    "Initialization: set branch addresses for tree \042%s\042\n",
			    (void*)fChain,fChain->GetName());
    }
  else
    {
      Services::Print::info("DataFrame::notify()",
			    "Tree \042%s\042 number %4i allocated\n",
			    fChain->GetName(),fCurrent);
    } 
  return true;
}

void DataFrame::show(Long64_t entry)
{ if ( entry < 0 ) return; }

Bool_t DataFrame::getEvent(Long64_t& ientry,Event& rEvent,Int_t mu,
			   Vertex::InteractionType collType,Int_t vtxOffset)
{
  // reset event store
  //--> don't do this here!  rEvent.reset();
  // fill event
  return fDataHandler->fillEvent(*this,rEvent,ientry,mu,vtxOffset,collType);
}

void DataFrame::loop(Vertex::InteractionType collType,Int_t vtxOffset)
{
  // figure out how many events to run
  Long64_t nevts((Long64_t)fConfig->nEvents);
  Long64_t ntot(0);
   if ( nevts > 0 ) 
     { ntot = nevts; }
   else
     { Long64_t nall((Long64_t)fChain->GetEntries());
       Long64_t nmu((Long64_t)fConfig->mu);
       ntot = nall/nmu; }
   Services::Print::info("DataFrame::Loop()",
			 "approximately form and analyze %i events\n",
			 (int)ntot);
   if ( nevts == 0 )
     { Services::Print::warning("DataFrame::Loop()",
				"no events left to analyze (nevts = %i)\n",
				(int)nevts);
       return; }

   // book histograms
   this->book();

   // prepare event loop
   Long64_t ientries(0);

   // instaniate Event with Selector   
   Event* pEvt = new Event(new Selectors::EtaRange(-6.,6.));

   ////////////////
   // Event loop //
   ////////////////

   Long64_t ievts(0);

   // requested number of events
   if ( nevts > 0 )
     {
       while ( ievts < ntot && 
	       fDataHandler->fillEvent(*this,*pEvt,ientries,
				       fConfig->mu,vtxOffset,collType))
	 { _incEvts(); this->ticker(); this->analyze(*pEvt); pEvt->reset(); }
     }
   // all events on input stream
   else
     { 
       while ( fDataHandler->fillEvent(*this,*pEvt,ientries,fConfig->mu) ) 
	 { _incEvts(); ticker(); analyze(*pEvt); pEvt->reset(); }
     }

   this->write();
}

bool DataFrame::book()
{ return true; }
 
bool DataFrame::write()
{ return true; }

bool DataFrame::analyze(Event& pEvt)
{ return true; }

Long64_t DataFrame::getMaxEvts()
{ return (Long64_t)fChain->GetEntries(); }

bool DataFrame::ticker()
{ 
  if ( fEvtsFreq < 0 ) return false;
  if ( fEvtsProc != 0 && fEvtsProc % fEvtsFreq )
    {
      Long64_t nevts((Long64_t)fChain->GetEntries());
      if ( fConfig->nEvents < nevts ) nevts = fConfig->nEvents;
      Long64_t ievts(1);
      unsigned int ictr(0);
      while ( nevts > ievts ) { ievts *= 10; ++ictr; }
#ifndef _HAVE_NO_BOOST_
      std::string fstr = "processed %"+boost::lexical_cast<std::string>(ievts)+
	"s/%"+boost::lexical_cast<std::string>(ievts)+"s events\n";
#else
      std::stringstream ostr;
      ostr << "processed %" << ievts << "s/%" << ievts << "s events\n";
      std::string fstr(ostr.str());
#endif
      Services::Print::info("%s::ticker()",fstr,fChain->GetName(),
			     fEvtsProc,(int)nevts);
    }
  return false;
}

Int_t DataFrame::numberOfEvents()
{ return fEvtsProc; }

Int_t DataFrame::cut(Long64_t /*entry*/)
{ return 0; }

void DataFrame::setPrintFrequency(Int_t nEventsFreq)
{ fEvtsFreq = nEventsFreq; }
