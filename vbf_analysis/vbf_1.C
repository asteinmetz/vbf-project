
#include "Services.h"
#include "Configuration.h"

#include "DataFrame.h"

#include "vbf_analysis_1.h"

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

  // something went wrong
  if ( !fConfig->isActive ) return 0; 

  // process control
  bool haveSignal(!fConfig->isEmpty(fConfig->signalFileList));
  bool havePileup(!fConfig->isEmpty(fConfig->pileupFileList));
  if ( !haveSignal )
    { PRINT_WARN(fConfig->mainName,
		 "This is a signal analysis, signal is needed!!\n"); 
      return 0;
    }
  if ( !havePileup )
    { PRINT_WARN(fConfig->mainName,"No pileup files given %s\n",
		 fConfig->pileupFileList.c_str()); }

  // check process configuration
  //  if ( !havePileup ) 
  //   { /
  //      PRINT_ERROR(fConfig->mainName,
  // 		  "Pile-up files required for this analysis!\n"); 
//       Services::Process::terminate("Invalid configuration\n");
//     }

  // set up chain: signal
  TChain* signalChain = haveSignal 
    ? fillChain(fConfig->signalTreeName,fConfig->signalFileList) : (TChain*)0;
  TChain* pileupChain = havePileup
    ? fillChain(fConfig->pileupTreeName,fConfig->pileupFileList) : (TChain*)0;

  DataFrame* signalServer = signalChain != 0 
    ? new DataFrame(signalChain) : 0;
  if ( signalServer == 0 )
    {
      PRINT_ERROR(fConfig->mainName,
		  "Cannot set up signal event server!\n");
      return 0;
    }

  DataFrame* pileupServer = pileupChain != 0
    ? new DataFrame(pileupChain) : 0;
  if ( pileupServer == 0 )
    {
      PRINT_WARN(fConfig->mainName,"No pileup configured!\n");
    }

  PRINT_INFO(fConfig->mainName,"initialize user analysis\n");

  // set up user analysis
  Vbf_Analysis usrAna("VBF",signalServer,pileupServer);

  usrAna.process();

  return 0;
};
