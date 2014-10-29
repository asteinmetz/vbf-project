// Copyright (c) 2013-, Gavin P. Salam. Gregory Soyez

#include "SimpleBTagger.h"

#include<valarray>
#include<iostream>
#include<sstream>

#include "../eventmanager_v1/Event.h"

using namespace fastjet;
using namespace std;

// description of the tagger
string SimpleBTagger::description() const{
  ostringstream oss;
  oss << "Simple B tagger with a tagging probability of " << _btag_proba
      << " and a fake rate of " << _fake_proba;
  return oss.str();
}

// get the b-tag probability for a given jet
//
// Note that if the jet has no particle, we assume we have a single
// particle and aplpy the b-tagging to it.
double SimpleBTagger::result(const PseudoJet &jet) const{
  vector<PseudoJet> constituents;
    
  if (jet.has_constituents())
    constituents = jet.constituents();
  else 
    constituents.push_back(jet);

  // count the number of b's
  int bcount=0;
  for (unsigned int i=0;i<constituents.size(); i++){
    const PseudoJet &particle = constituents[i];
    if (particle.has_user_info<ParticleInfo>())
      bcount += _b_count_from_particle_id(particle.user_info<ParticleInfo>().id());
  }
    
  // decide the weight
  if (bcount == 0)
    return _fake_proba;

  
  return _btag_proba;
}    

// given a PDG id, return the net b count in the particle
// (i.e. number of b's - number of bbar's)
int SimpleBTagger::_b_count_from_particle_id(int id) const{
  // for particles with illicit (zero) idhep, no work to be done
  if (id == 0) return 0;

  int netsign = (id >= 0 ? +1 : -1);
  id = abs(id);

  // extract digits of the idhep, since these contain information 
  // on flavour of component quarks
  valarray<int> digit(4);
  int           ndigits = 0;
  for (int i = 0; i < 4; i++) {
    digit[i] = id % 10;
    if (digit[i] != 0) ndigits = i+1;
    id /= 10; // "shift" things along
  }

  int net_b_count=0;
  
  // start this part with _flav_content already initialised to zero
  // in constructor
  if (ndigits == 1) { // a lone quark
    if (digit[0] > 6 || digit[0] == 0) {
      cerr << "Net b counting: failed to understand id = " << id << endl;
      exit(-1);
    }
    if (digit[0]==5) return netsign;
  } else if (ndigits == 2) { // a lepton, photon or cluster [flav lost...]
    return 0;
  } else { // must be a meson, cluster or baryon
    // check sanity of codes
    for (int i=1; i < ndigits; i++) {
      if (digit[i] > 6){
        cerr << "Net b counting: failed to understand id = " << id << endl;
        exit(-1);
      }
    }
    
    // now deal with different cases
    if (ndigits == 4) { // diquark [nm0x] or baryon [nmpx]
      for (int i=1; i < ndigits; i++) {
	if (digit[i] == 5) net_b_count += netsign;
      }
    } else if (ndigits == 3) { // meson [nmx]
      // Beware of PDG convention that says that a K+ or B+ are a
      // particle and so have positive id (i.e. flavcodes > 1). So
      if (digit[2] == 3 || digit[2] == 5) netsign = -netsign;
      if (digit[2]==5) net_b_count += netsign;
      if (digit[1]==5) net_b_count -= netsign;
    } else {
      cerr << "Net b counting: failed to understand id = " << id << endl;
      exit(-1);
    }
  }

  return net_b_count;
  
}
