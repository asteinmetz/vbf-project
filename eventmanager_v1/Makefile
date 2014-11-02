CXX := $(GCC)

PROGSRCS = analysis.C
COMMONSRCS = Event.cxx RandomEngine.cxx Selectors.cxx Services.cxx\
  Configuration.cxx HistGroup.cxx HistGroupEvent.cxx ExtraSelectors.cxx\
  DataFrame.cxx UserAnalysisBase.cxx TestFilterAnalysis.cxx HZAnalysis.cxx\
  AllAnalyses.cxx HistGroupContainer.cxx SimpleBTagger.cxx TowerGrid.cxx ObjectSelectors.cxx
COMMONOBJS = $(patsubst %.C,%.o,$(patsubst %.cxx,%.o,$(COMMONSRCS)))

ROOTCXXFLAGS = $(shell root-config --cflags)
ROOTLDFLAGS = $(shell root-config --libs)
FASTJETCXXFLAGS = $(shell fastjet-config --cxxflags)
FASTJETLDFLAGS = $(shell fastjet-config --libs)
HELPERS_DIR=../helper_tools

CXXFLAGS = -g -I. -I$(HELPERS_DIR) -D_HAVE_NO_BOOST_ $(ROOTCXXFLAGS) $(FASTJETCXXFLAGS)
LIBS =  -L. -L$(HELPERS_DIR) -lhelpers $(ROOTLDFLAGS) $(FASTJETLDFLAGS) -Wl,-rpath,$(shell root-config --prefix)/lib

.PHONY = all lib clean

all: lib analysis

# TODO: linking against libframe doesn't work on Andy's machine... something from root-config?
lib: libframe.a

analysis: libframe.a analysis.o
	$(CXX) $(CXXFLAGS) analysis.o $(COMMONOBJS) $(LIBS) -o analysis.exe

clean:
	@rm -f *.o

doxygen:
	$(shell which doxygen) doxygen.conf

########################

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# TODO: linking against libframe doesn't work on Andy's machine... something from root-config?
libframe.a: $(COMMONOBJS)
	ar cru libframe.a $(COMMONOBJS)
	ranlib libframe.a

depend:
	makedepend  -Y --   -- $(COMMONSRCS) $(PROGSRCS)
# DO NOT DELETE

Event.o: PrintDef.h Services.h Services.icc Event.h IFinalStateSelector.h
Event.o: RandomEngine.h Event.icc
RandomEngine.o: RandomEngine.h
Selectors.o: Selectors.h IFinalStateSelector.h Event.h RandomEngine.h
Selectors.o: Event.icc
Services.o: Services.h PrintDef.h Services.icc
Configuration.o: PrintDef.h Services.h Services.icc Configuration.h
Configuration.o: Configuration.icc
HistGroup.o: HistGroup.h Event.h IFinalStateSelector.h RandomEngine.h
HistGroup.o: Event.icc Services.h PrintDef.h Services.icc HistGroupCfg.h
HistGroupEvent.o: Services.h PrintDef.h Services.icc Event.h
HistGroupEvent.o: IFinalStateSelector.h RandomEngine.h Event.icc
HistGroupEvent.o: HistGroupCfg.h HistGroupEvent.h HistGroup.h
ExtraSelectors.o: ExtraSelectors.hh Event.h IFinalStateSelector.h
ExtraSelectors.o: RandomEngine.h Event.icc
DataFrame.o: Services.h PrintDef.h Services.icc Selectors.h
DataFrame.o: IFinalStateSelector.h DataFrame.h Configuration.h
DataFrame.o: Configuration.icc Event.h RandomEngine.h Event.icc
UserAnalysisBase.o: PrintDef.h Services.h Services.icc HistGroupEvent.h
UserAnalysisBase.o: HistGroup.h Event.h IFinalStateSelector.h RandomEngine.h
UserAnalysisBase.o: Event.icc HistGroupCfg.h Selectors.h UserAnalysisBase.h
UserAnalysisBase.o: Configuration.h Configuration.icc DataFrame.h
TestFilterAnalysis.o: TestFilterAnalysis.h UserAnalysisBase.h Configuration.h
TestFilterAnalysis.o: PrintDef.h Services.h Services.icc Configuration.icc
TestFilterAnalysis.o: HistGroup.h Event.h IFinalStateSelector.h
TestFilterAnalysis.o: RandomEngine.h Event.icc HistGroupCfg.h DataFrame.h
HZAnalysis.o: HZAnalysis.h UserAnalysisBase.h Configuration.h PrintDef.h
HZAnalysis.o: Services.h Services.icc Configuration.icc HistGroup.h Event.h
HZAnalysis.o: IFinalStateSelector.h RandomEngine.h Event.icc HistGroupCfg.h
HZAnalysis.o: DataFrame.h ExtraSelectors.hh FilterFromCmdLine.hh
AllAnalyses.o: AllAnalyses.h UserAnalysisBase.h Configuration.h PrintDef.h
AllAnalyses.o: Services.h Services.icc Configuration.icc HistGroup.h Event.h
AllAnalyses.o: IFinalStateSelector.h RandomEngine.h Event.icc HistGroupCfg.h
AllAnalyses.o: DataFrame.h TestFilterAnalysis.h HZAnalysis.h
HistGroupContainer.o: PrintDef.h Services.h Services.icc HistGroupContainer.h
HistGroupContainer.o: IFinalStateSelector.h HistGroup.h Event.h
HistGroupContainer.o: RandomEngine.h Event.icc HistGroupCfg.h
SimpleBTagger.o: ../eventmanager_v1/Event.h IFinalStateSelector.h
SimpleBTagger.o: RandomEngine.h Event.icc
analysis.o: Services.h PrintDef.h Services.icc Configuration.h
analysis.o: Configuration.icc DataFrame.h Event.h IFinalStateSelector.h
analysis.o: RandomEngine.h Event.icc AllAnalyses.h UserAnalysisBase.h
analysis.o: HistGroup.h HistGroupCfg.h
