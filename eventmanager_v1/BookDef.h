// -*- c++ -*-
#ifndef BOOKDEF_H
#define BOOKDEF_H

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TProfile2D.h"

#define BOOK_( _x_ ) Services::Histogramming::book< _x_ >

#define BOOK_TH1 BOOK_( TH1D )
#define BOOK_TH2 BOOK_( TH2D )
#define BOOK_TH3 BOOK_( TH3D )
#define BOOK_TP1 BOOK_( TProfile )
#define BOOK_TP2 BOOK_( TProfile2D )

#endif
