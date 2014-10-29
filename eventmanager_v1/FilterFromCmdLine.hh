#ifndef __FILTER_FROM_CMDLINE_HH__
#define __FILTER_FROM_CMDLINE_HH__

#include "Configuration.h"
#include <fastjet/tools/Filter.hh>
#include <fastjet/tools/BackgroundEstimatorBase.hh>
#include "SelectorArea.hh"
#include <sstream>

// this is simply a small util that allows you to specify a filter
// from the command line using
//
//    --filter=...
//
// ['filter' can be replaced by an optional string if needed]
//
// The argument is interpreted as follows:
//
//   - hardest_<xfilt>_<nfilt> (with Rfilt a double and Rfilt an
//     integer) will recluster with a radius xfilt.R (R being the
//     original jet radius) and keep the nfilt hardest subjets
//
//   - trim_<xfilt> (with xfilt a double) will recluster with a radius
//     0.2 and keep the subjets with ptsub/pt > xfilt.
//
//   - area_xsigma (with xfilt a double) will recluster with a radius
//     0.2 and keep the subjets with ptsub(subtracted) > xfilt sigma sqrt(A).
//

namespace Configuration{

  bool getFilter(fastjet::Filter &filter, 
                 double Rjet, fastjet::BackgroundEstimatorBase *bge=0,
                 const std::string name="filter"){
    std::string value;
    if (!getConfig(name, value)) return false;

    // try to parse the value
    //
    if ((value.size()>=9) && (value.compare(0,8,"hardest_")==0)){
      // hardest
      size_t sep_pos = value.rfind("_");
      if (sep_pos==7){
        Services::Print::error("Configuration::getFilter",
                               "hardest_xfilt_nfilt filter syntax not understood.\n", value.c_str());
        return false;
      }

      std::istringstream iss1(value.substr(8, (sep_pos-8)));
      std::istringstream iss2(value.substr(sep_pos+1));
      double xfilt;
      unsigned int nfilt;
      iss1 >> xfilt;
      iss2 >> nfilt;
      
      filter = fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, xfilt*Rjet),
                               fastjet::SelectorNHardest(nfilt));
    } else if ((value.size()>=5) && (value.compare(0,5,"trim_")==0)){
      // trimming
      std::istringstream iss(value.substr(5));
      double xfilt;
      iss >> xfilt;
      filter = fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.2),
                               fastjet::SelectorPtFractionMin(xfilt));      
    } else if ((value.size()>=5) && (value.compare(0,5,"area_")==0)){
      // area filter
      std::istringstream iss(value.substr(5));
      double area_n;
      iss >> area_n;
      if (! bge){
        Services::Print::error("Configuration::getFilter",
                               "requested and area filter without specifying a background estimator).\n", value.c_str());
        return false;
      }
      filter = fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.2),
                               fastjet::SelectorAreaRhoThreshold(bge, 0.0, area_n));
    } else {
      Services::Print::error("Configuration::getFilter",
                             "filter specifiaction (%s) not understood.\n", value.c_str());
      return false;
    }

    return true;
  }

}
#endif  // __FILTER_FROM_CMDLINE_HH__
