CXX := g++

PROGSRCS = mb_analysis.C
COMMONSRCS = UserAnalysisMB.cxx UserAnalysisMBJets.cxx HistGroupMBJets.cxx
COMMONOBJS = $(patsubst %.C,%.o,$(patsubst %.cxx,%.o,$(COMMONSRCS)))

ROOTCXXFLAGS = $(shell root-config --cflags)
ROOTLDFLAGS = $(shell root-config --libs)
FASTJETCXXFLAGS = $(shell fastjet-config --cxxflags)
FASTJETLDFLAGS = $(shell fastjet-config --libs)

EVENTMGR_DIR  = ../eventmanager_v1
HELPERS_DIR   = ../helper_tools
LOCALLDFLAGS  = -L. -lmb_analysis -L$(EVENTMGR_DIR) -lframe -L$(HELPERS_DIR) -lhelpers
LOCALCXXFLAGS = -I. -I$(EVENTMGR_DIR) -I$(HELPERS_DIR) -D_HAVE_NO_BOOST_



CXXFLAGS = -g $(LOCALCXXFLAGS) $(ROOTCXXFLAGS) $(FASTJETCXXFLAGS)
LIBS     =  $(LOCALLDFLAGS) $(ROOTLDFLAGS) $(FASTJETLDFLAGS) -Wl,-rpath,$(shell root-config --prefix)/lib

.PHONY = all lib

all: lib mb_analysis doxygen

lib: libmb_analysis.a

mb_analysis: libmb_analysis.a mb_analysis.o
	$(CXX) $(CXXFLAGS) mb_analysis.o $(LIBS) -o mb_analysis.exe

clean:
	@rm -f *.o

doxygen:
	$(shell which doxygen) doxygen.conf

########################
%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

libmb_analysis.a: $(COMMONOBJS)
	ar cruv libmb_analysis.a $(COMMONOBJS)
	ranlib libmb_analysis.a

depend:
	makedepend  -Y --   -- $(COMMONSRCS) $(PROGSRCS)
# DO NOT DELETE

Event.o: Event.h IFinalStateSelector.h RandomEngine.h Event.icc
RandomEngine.o: RandomEngine.h
Selectors.o: Selectors.h IFinalStateSelector.h Event.h RandomEngine.h
Selectors.o: Event.icc
Services.o: Services.h PrintDef.h Services.icc
Configuration.o: PrintDef.h Services.h Services.icc Configuration.h
Configuration.o: Configuration.icc
HistGroup.o: HistGroup.h Services.h PrintDef.h Services.icc HistGroupCfg.h
HistGroupEvent.o: Services.h PrintDef.h Services.icc Event.h
HistGroupEvent.o: IFinalStateSelector.h RandomEngine.h Event.icc
HistGroupEvent.o: HistGroupCfg.h HistGroupEvent.h HistGroup.h
ExtraSelectors.o: ExtraSelectors.hh Event.h IFinalStateSelector.h
ExtraSelectors.o: RandomEngine.h Event.icc
DataFrame.o: Services.h PrintDef.h Services.icc Selectors.h
DataFrame.o: IFinalStateSelector.h DataFrame.h Configuration.h
DataFrame.o: Configuration.icc Event.h RandomEngine.h Event.icc
UserAnalysisBase.o: Services.h PrintDef.h Services.icc Selectors.h
UserAnalysisBase.o: IFinalStateSelector.h UserAnalysisBase.h Configuration.h
UserAnalysisBase.o: Configuration.icc HistGroupEvent.h HistGroup.h
UserAnalysisBase.o: HistGroupCfg.h Event.h RandomEngine.h Event.icc
UserAnalysisBase.o: DataFrame.h
TestFilterAnalysis.o: TestFilterAnalysis.h UserAnalysisBase.h Configuration.h
TestFilterAnalysis.o: PrintDef.h Services.h Services.icc Configuration.icc
TestFilterAnalysis.o: HistGroupEvent.h HistGroup.h HistGroupCfg.h Event.h
TestFilterAnalysis.o: IFinalStateSelector.h RandomEngine.h Event.icc
TestFilterAnalysis.o: DataFrame.h
HZAnalysis.o: HZAnalysis.h UserAnalysisBase.h Configuration.h PrintDef.h
HZAnalysis.o: Services.h Services.icc Configuration.icc HistGroupEvent.h
HZAnalysis.o: HistGroup.h HistGroupCfg.h Event.h IFinalStateSelector.h
HZAnalysis.o: RandomEngine.h Event.icc DataFrame.h ExtraSelectors.hh
AllAnalyses.o: AllAnalyses.h UserAnalysisBase.h Configuration.h PrintDef.h
AllAnalyses.o: Services.h Services.icc Configuration.icc HistGroupEvent.h
AllAnalyses.o: HistGroup.h HistGroupCfg.h Event.h IFinalStateSelector.h
AllAnalyses.o: RandomEngine.h Event.icc DataFrame.h TestFilterAnalysis.h
AllAnalyses.o: HZAnalysis.h
analysis.o: Services.h PrintDef.h Services.icc Configuration.h
analysis.o: Configuration.icc DataFrame.h Event.h IFinalStateSelector.h
analysis.o: RandomEngine.h Event.icc AllAnalyses.h UserAnalysisBase.h
analysis.o: HistGroupEvent.h HistGroup.h HistGroupCfg.h
