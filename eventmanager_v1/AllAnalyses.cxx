#include "AllAnalyses.h"

#include "TestFilterAnalysis.h"
#include "HZAnalysis.h"
#include "Configuration.h"

//------------------------------------------------------------------------
// build the analysis from what is specified on the cmd line
namespace Configuration{
  fastjet::SharedPtr<UserAnalysisBase> configAnalysisPtr;

  /*! @brief return a reference to the analysis specified on the command line */
  UserAnalysisBase & getAnalysis(DataFrame* signalServer,
                                 DataFrame* pileupServer){
    // Get the analysis name from the command line if present
    std::string name;
    if (!getConfig("analysis", name))
      name = "UserAnalysisBase";
    ConfigData::instance()->analysisName = name;
    
    Services::Print::info(ConfigData::instance()->mainName,
                          "Trying to instanciate an analysis of type %s\n", name.c_str());

    if (name == std::string("UserAnalysisBase")){
      configAnalysisPtr.reset(new UserAnalysisBase(name, signalServer, pileupServer));
    } else if (name == std::string("TestFilterAnalysis")){
      configAnalysisPtr.reset(new TestFilterAnalysis(name, signalServer, pileupServer));
    } else if (name == std::string("HZAnalysis")){
      configAnalysisPtr.reset(new HZAnalysis(name, signalServer, pileupServer));
    } else {
      Services::Print::error(ConfigData::instance()->mainName,
			     "analysis name not understood. Falling back to the base analysis\n");
      configAnalysisPtr.reset(new UserAnalysisBase(name, signalServer, pileupServer));
    }
      
    return *(configAnalysisPtr.get());
  }
}
