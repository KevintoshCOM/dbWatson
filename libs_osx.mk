#FIX: GCC cant evaluate ~
HMD := $(shell echo ~)

#LIBS
LIBFILES := pq xml2
LPATHS := $(HMD)/PostgreSQL/pg96/lib libs/osx
IPATHS := $(HMD)/PostgreSQL/pg96/include
