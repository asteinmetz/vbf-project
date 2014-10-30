
#include "Services.h"
#include "Configuration.h"

#include "DataFrame.h"

#include "UserAnalysisMBJets.h"

#include "TChain.h"

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "PrintDef.h"

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
  PRINT_INFO("fillchain(%s)","found %i files with %i events total\n",
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

//   double rCore(0);
//   if ( !Configuration::getConfig("rcore",rCore ) )
//     { PRINT_WARN(fConfig->mainName,"cannot find parameter <rcore>\n"); }
//   else
//     { PRINT_INFO(fConfig->mainName,"found rcore = %4.2f\n",rCore); }

//   bool cFlag(false);
//   if ( !Configuration::getConfig("testflag",cFlag) )
//     { PRINT_WARN(fConfig->mainName,"cannot find parameter <testflag>\n"); }
//   else
//     { PRINT_INFO(fConfig->mainName,"found testflag = %ib\n",cFlag); }

  // something went wrong
  if ( !fConfig->isActive ) return 0; 

  // process control
  bool haveSignal(!fConfig->isEmpty(fConfig->signalFileList));
  bool havePileup(!fConfig->isEmpty(fConfig->pileupFileList));
  if ( haveSignal )
    { PRINT_WARN(fConfig->mainName,
		 "This is a pile-up/MB analysis, signal is ignored.\n"); }

  // check process configuration
  if ( !havePileup ) 
    { 
      PRINT_ERROR(fConfig->mainName,
		  "Pile-up files required for this analysis!\n"); 
      Services::Process::terminate("Invalid configuration\n");
    }

  // set up chain: pile-up
  TChain* pileupChain = 
    fillChain(fConfig->pileupTreeName,fConfig->pileupFileList);
  DataFrame* pileupServer = pileupChain != 0 
    ? new DataFrame(pileupChain) : 0;
  if ( pileupServer == 0 )
    {
      PRINT_ERROR(fConfig->mainName,
		  "Cannot set up pile-up event server!\n");
      return 0;
    }

  PRINT_INFO(fConfig->mainName,"initialize user analysis\n");

  // set up user analysis
  UserAnalysisMBJets usrAna("MBJetsAnalysis",pileupServer);

  usrAna.process();

  return 0;
};
