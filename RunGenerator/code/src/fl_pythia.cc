// Learning pythia 11/07/2014
#include "Pythia.h"
using namespace Pythia8;
int main()
{
  Pythia pythia;
  pythia.readString("HardQCD:all = on");
  pythia.readString("PhaseSpace:pTHatMin = 20.");
  pythia.init(2212,2212,14000.);
  Hist eta("eta",100,-5.,5.);
  for (int iEvent=0;iEvent<10;++iEvent)
    {
      pythia.next();
      for (int i=0;i<pythia.event.size();++i)
	{
	  eta.fill(pythia.event[i].eta());
	}
      cout << eta;
    }
}
