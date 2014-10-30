// Copyright (c) 2005-2013, Matteo Cacciari, Gavin P. Salam and Gregory Soyez

#ifndef __IDENTITY_TRANSFORMER_HH__
#define __IDENTITY_TRANSFORMER_HH__

/// \class IdentityTransformer
/// simple transformer that does not modify anything
class IdentityTransformer : public Transformer{
public:
  IdentityTransformer(){};
  string description() const{ return "Identity";}
  PseudoJet result(const PseudoJet &jet) const{
    return jet;
  }
};

#endif  // __IDENTITY_TRANSFORMER_HH__
