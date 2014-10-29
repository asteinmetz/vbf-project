
#include "Services.h"
#include "Configuration.h"

#include "DataFrame.h"

//#include "UserAnalysisBase.h"
//#include "TestFilterAnalysis.h"
#include "AllAnalyses.h"

#include "TChain.h"

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

/*! @brief Helper function loading file names into a @c TChain
 *
 *  @return Pointer to new @c TChain object - can be @C NULL if something
 *          goes wrong.
 *
 *  @param[in] chainName name of chain/tree
 *  @param[in] fList     name of file with file list  
 *
 *  @todo Needs a new home!
 */
TChain* fillChain(const std::string& chainName,const std::string& fList)
{
  TChain* chain = new TChain(chainName.c_str());

  // read file list into local storage
  std::string fname;
  std::ifstream inStream;
  std::vector<std::string> lofFiles;
  inStream.open(fList.c_str());
  do
    {
      inStream >> fname;
      if ( std::find(lofFiles.begin(),lofFiles.end(),fname) ==
	   lofFiles.end() )
	{
	  if ( !fname.empty() ) chain->Add(fname.c_str(),-1);
	  lofFiles.push_back(fname);
	}
    }
  while ( !inStream.eof() );

  // 
  Services::Print::info("fillchain(%s)",
			"found %i files with %i events total\n",
			chainName.c_str(),(int)lofFiles.size(),
			chain->GetEntries());
  return chain; 
}

/*! @brief Example main program for analysis */
int main(int argc,char** argv)
{

  // check on arguments
  Configuration::ConfigData* fConfig = Configuration::ConfigData::instance();
  Configuration::interpretConfig(argc,argv,*fConfig);
  Configuration::printConfig(*fConfig);

  // something went wrong
  if ( !fConfig->isActive ) return 0; 

  // process control
  bool haveSignal(!fConfig->isEmpty(fConfig->signalFileList));
  bool havePileup(!fConfig->isEmpty(fConfig->pileupFileList));

  // check process configuration
  if ( !haveSignal ) 
    { Services::Print::warning("%s","no signal events given\n",
			       fConfig->mainName.c_str()); }
  if ( !havePileup ) 
    { Services::Print::warning("%s","no pile-up events given\n",
			       fConfig->mainName.c_str()); }

  // invalid configuration
  if ( !haveSignal && !havePileup ) 
    { Services::Print::error("%s","missing input file lists, terminate\n",
			     fConfig->mainName.c_str()); 
      return 0; }

  // set up chain: pile-up
  TChain*    pileupChain  = 0;
  DataFrame* pileupServer = 0;
  if ( havePileup )
    {
      pileupChain = fillChain(fConfig->pileupTreeName,fConfig->pileupFileList);
      havePileup  = pileupChain != 0;
      if ( havePileup ) pileupServer = new DataFrame(pileupChain);
    }

  // set up chain: signal
  TChain*    signalChain  = 0;
  DataFrame* signalServer = 0;
  if ( haveSignal )
    {
      signalChain = fillChain(fConfig->signalTreeName,fConfig->signalFileList);
      haveSignal  = signalChain != 0;
      if ( haveSignal ) signalServer = new DataFrame(signalChain);
    }

  // check on configurations again
  if ( !haveSignal )
    { Services::Print::error(fConfig->mainName,
			     "no working setup for signal or pile-up\n");
      return 0; }

  Services::Print::info(fConfig->mainName,"initialize user analysis\n");


  // set up user analysis
  //UserAnalysisBase usrAna("TestAnalysis",signalServer,pileupServer);
  
  //TestFilterAnalysis usrAna("TestAnalysis",signalServer,pileupServer);
  //// you may set some parameters here
  //usrAna.set_jet_ptmin(500.0);
  //usrAna.set_jet_R(1.0);

  UserAnalysisBase &usrAna = Configuration::getAnalysis(signalServer,pileupServer);
  usrAna.process();


  return 0;
};
