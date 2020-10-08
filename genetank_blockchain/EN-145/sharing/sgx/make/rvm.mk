R_CC := cc
R_CXX := g++

R_DEFS := -DHAVE_CONFIG_H
R_CFLAGS := -g -O0
R_OPENMP_CFLAGS := -fopenmp
R_CPICFLAGS := -fpic

R_XTRA_CPPFLAGS :=  -I. -I$(RVM)/include -I$(RVM)/clapack/INCLUDE
ALL_CPPFLAGS := $(R_XTRA_CPPFLAGS) $(R_DEFS)

