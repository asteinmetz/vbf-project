// -*- c++ -*-
#ifndef PRINTDEF_H
#define PRINTDEF_H

#include "Services.h"

#ifndef PRINT_INFO 
#define PRINT_INFO Services::Print::info
#endif

#ifndef PRINT_WARN 
#define PRINT_WARN Services::Print::warning
#endif

#ifndef PRINT_ERROR 
#define PRINT_ERROR Services::Print::error
#endif

#endif
