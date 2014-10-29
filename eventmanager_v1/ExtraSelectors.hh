#ifndef __EXTRA_SELECTORS_H__
#define __EXTRA_SELECTORS_H__

#include <fastjet/Selector.hh>

fastjet::Selector SelectorHardVertex();
fastjet::Selector SelectorAbsId(const unsigned int id);

#endif // __EXTRA_SELECTORS_H__
