
MCH=x86_64-slc5-gcc43-opt

export ROOTPATH=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.26.00/${MCH}/root/
export ROOTLIB=${ROOTPATH}/lib

export FASTJETPATH=/raid03/francav/LesHouches2011/code/install-3.0alpha3/
export FASTJETLIB=${FASTJETPATH}/lib
export FASTJETINC=${FASTJETPATH}/include

export PATH=${PATH}:${FASTJETPATH}/bin:${ROOTPATH}/bin
export LD_LIBRARY_PATH=${ROOTLIB}:${FASTJETLIB}:${LD_LIBRARY_PATH}

export PYTHIA8DATA=/afs/cern.ch/sw/lcg/external/MCGenerators/pythia8/150/share/xmldoc
export PYTHIADIR=/afs/cern.ch/sw/lcg/external/MCGenerators/pythia8/150/${MCH}

#/afs/cern.ch/sw/lcg/external/MCGenerators/herwig++/2.3.2/${MCH}
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/herwig++/2.3.2/${MCH}/lib:$LD_LIBRARY_PATH
export PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/herwig++/2.3.2/${MCH}/bin:$PATH


export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/thepeg/1.4.2/${MCH}/lib:$LD_LIBRARY_PATH
export PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/thepeg/1.4.2/${MCH}/bin:$PATH

export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/GSL/1.10/${MCH}/lib:$LD_LIBRARY_PATH
export PATH=/afs/cern.ch/sw/lcg/external/GSL/1.10/${MCH}/bin:$PATH

export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/HepMC/2.03.09/${MCH}/lib:$LD_LIBRARY_PATH
export PATH=/afs/cern.ch/sw/lcg/external/HepMC/2.03.09/${MCH}/bin:$PATH

