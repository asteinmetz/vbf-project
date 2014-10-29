#ifndef __ALL_ANALYSES_H__
#define __ALL_ANALYSES_H__

#include "UserAnalysisBase.h"
#include <fastjet/SharedPtr.hh>

namespace Configuration{
  /*! @brief return a reference to the analysis specified on the command line */
  UserAnalysisBase & getAnalysis(DataFrame* signalServer,
                                 DataFrame* pileupServer);
}

#endif  // __ALL_ANALYSES_H__
