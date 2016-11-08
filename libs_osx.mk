#FIX: GCC cant evaluate ~
HMD := $(shell echo ~)

#LIBS
LIBFILES := pq
LPATHS := $(HMD)/PostgreSQL/pg96/lib
IPATHS := $(HMD)/PostgreSQL/pg96/include
