CXX := g++

COMMONSRCS = matching.cc SubtractorWithMass.cc SelectorArea.cc MDTaggerWithSub.cc
COMMONOBJS = $(patsubst %.cc,%.o,$(COMMONSRCS))

FASTJETCXXFLAGS = $(shell fastjet-config --cxxflags)

CXXFLAGS = -g -I. $(FASTJETCXXFLAGS)

.PHONY = all lib

all: lib

lib: libhelpers.a

clean:
	@rm -f *.o

########################
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

libhelpers.a: $(COMMONOBJS)
	ar cru libhelpers.a $(COMMONOBJS)
	ranlib libhelpers.a

depend:
	makedepend  -Y --   -- $(COMMONSRCS) $(PROGSRCS)
# DO NOT DELETE

matching.o: matching.hh
SubtractorWithMass.o: SubtractorWithMass.hh
