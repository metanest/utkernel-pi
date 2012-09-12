#
# ----------------------------------------------------------------------
#     micro T-Kernel
#
#     Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
#     micro T-Kernel is distributed under the T-License 2.0.
# ----------------------------------------------------------------------
#
#     Version:   1.01.01
#     Released by T-Engine Forum(http://www.t-engine.org) at 2011/12/12.
#
# ----------------------------------------------------------------------
#

# modified for Raspberry Pi Type B by Makoto Kishimoto

#
#	Makefile for gmake
#	libtk (system-dependent description)
#

# T-Engine type
MACHINE = raspib
TETYPE = app

# default rules
include ../../../../etc/makerules

# source files (system-dependent)
SRC =	disint.S int.c

# compiler options
CFLAGS += -fomit-frame-pointer

# include main makefile (common description)
include ../../src/Makefile.common
