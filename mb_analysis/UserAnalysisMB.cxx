
#include "HistGroupEvent.h"
#include "UserAnalysisMB.h"
#include "Selectors.h"

#include "TFile.h"

#include "PrintDef.h"

UserAnalysisMB::UserAnalysisMB(const std::string& name,DataFrame* pileupServer)
  : UserAnalysisBase(name,(DataFrame*)0,pileupServer)
{ }

UserAnalysisMB::~UserAnalysisMB()
{ }

// bool UserAnalysisMB::process()
// {
//   ///////////////////////
//   // Setting things up //
//   ///////////////////////

//   PRINT_INFO("UserAnalysisMB::process()","entered\n");

//   // copy some configuration
//   long long nevts(this->fConfig().nEvents);
//   int       mu(this->fConfig().mu);

//   // local variables
//   long long pupEntries(0);

//   // number of events
//   long long ntot = nevts < 0 
//     ? (this->fPileupServer().getMaxEvts())/mu : nevts;

//   // set up number event store
//   Event pEvt(new Selectors::EtaRange(-6.,6.));

//   // book histograms
//   this->book();

//   PRINT_INFO("UserAnalysisMB::process()",
// 	     "attempt to analyze about %i events\n",(int)ntot);

//   /////////////////////
//   // Pileup analysis //
//   /////////////////////

//   PRINT_INFO("UserAnalysisMB::process()","analyze pileup\n");
//   long long ievts(0);
//   while ( ievts < ntot && 
// 	  this->fPileupHandler().fillEvent(this->fPileupServer(),
// 					   pEvt,pupEntries,mu,0,
// 					   Vertex::PILEUP) )
//     { this->analyze(pEvt); pEvt.reset(); ++ievts; }
//   return this->write();
// }

bool UserAnalysisMB::analyze(Event& pEvt)
{ this->fPileupPlots().fill(pEvt); return true; }

bool UserAnalysisMB::book()
{ 
  this->fSetPileupPlots(new HistGroupEvent("PileupEvent"));
  this->fPileupPlots().book();
  return true; 
}


// bool UserAnalysisMB::write()
// {
//   std::string fileOpts("RECREATE");
//   if ( Configuration::getConfig("ofileopts",fileOpts) ) 
//     {
//       PRINT_INFO("Useranalysis_%s::write()",
// 		 "write file \042%s\042 with options \042%s\042\n",
// 		 name().c_str(),
// 		 fConfig().outputFile.c_str(),
// 		 fileOpts.c_str());
//     }
//   else
//     {
//       PRINT_INFO("Useranalysis_%s::write()",
// 		 "write file \042%s\042 with default options \042%s\042\n",
// 		 name().c_str(),
// 		 fConfig().outputFile.c_str(),
// 		 fileOpts.c_str());
//     }


//   TFile* ff = new TFile(fConfig().outputFile.c_str(),fileOpts.c_str());
//   this->fPileupPlots().write();
//   ff->Close();
//   delete ff;
// }

bool UserAnalysisMB::fTicker()
{
  ++_analEvts;
  if ( _analEvts % _writeFreq == 0 )
    {
      PRINT_INFO("UserAnalysisMB_%s","number of events analyzed: %i\n",
		 this->name().c_str(),_analEvts);
      if ( _analEvts == 15*_writeFreq ) _writeFreq *= 10; 
      return this->write();
    }
  return true;
}

