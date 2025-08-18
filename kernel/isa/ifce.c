/* This file is a wrapper for the ISA interface implementation for the target being built */

#include "ifce.h"

#if defined(__x86_64__)
#include "isa/x86_64/ifce.c"
#endif
