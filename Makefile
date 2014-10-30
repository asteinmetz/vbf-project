## Makefile for installing dependency packages into PREFIX and building components

PREFIX := $(PWD)/local
BUILDDIR := $(PREFIX)/build

FJNAME := fastjet-3.0.4
HEPMCNAME := HepMC-2.06.09
PY8NAME := pythia8186  ##76

.PHONY = help eventmgr helpers rungen fastjet hepmc pythia8 herwigpp root $(PREFIX) $(BUILDDIR)

help:
	@echo "No default target. Available targets are: eventmgr helpers rungen fastjet hepmc pythia8 (herwigpp) (root)"

fastjet: $(PREFIX)/lib/libfastjet.so
$(PREFIX)/lib/libfastjet.so: $(BUILDDIR)/$(FJNAME)
	cd $(BUILDDIR)/$(FJNAME) && \
	./configure --prefix=$(PREFIX) --enable-shared && \
	make -j2 && make install

pythia8: $(PREFIX)/lib/libpythia8.so
$(PREFIX)/lib/libpythia8.so: $(BUILDDIR)/$(PY8NAME)
	cd $(BUILDDIR)/$(PY8NAME) && \
	./configure --prefix=$(PREFIX) --enable-shared && \
	make -j2 && make install

hepmc: $(PREFIX)/lib/libHepMC.so
$(PREFIX)/lib/libHepMC.so: $(BUILDDIR)/$(HEPMCNAME)
	cd $(BUILDDIR) && mkdir -p build && cd build && \
	cmake -DCMAKE_INSTALL_PREFIX=$(PREFIX) -Dmomentum=GEV -Dlength=MM $(BUILDDIR)/$(HEPMCNAME) && \
	make -j2 && make install

eventmgr: helpers
	cd eventmanager_v1 && make

helpers:
	cd helper_tools && make

rungen: $(PREFIX)/lib/libpythia8.so
	cd RunGenerator/code/src && make PY8DIR=$(PREFIX)


$(PREFIX):
	@mkdir -p $(PREFIX)
$(BUILDDIR): $(PREFIX)
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(FJNAME): $(BUILDDIR)
	cd $(BUILDDIR) && wget http://www.fastjet.fr/repo/$(FJNAME).tar.gz -O- | tar xz

$(BUILDDIR)/$(PY8NAME): $(BUILDDIR)
	cd $(BUILDDIR) && wget http://home.thep.lu.se/~torbjorn/pythia8/$(PY8NAME).tgz -O- | tar xz

$(BUILDDIR)/$(HEPMCNAME): $(BUILDDIR)
	cd $(BUILDDIR) && wget http://lcgapp.cern.ch/project/simu/HepMC/download/$(HEPMCNAME).tar.gz -O- | tar xz
