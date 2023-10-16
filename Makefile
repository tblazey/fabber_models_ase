include ${FSLCONFDIR}/default.mk

PROJNAME = fabber_ase

USRINCFLAGS = -I${INC_NEWMAT} -I${INC_PROB} -I${INC_BOOST} -I..
USRLDFLAGS = -L${LIB_NEWMAT} -L${LIB_PROB}

FSLVERSION= $(shell cat ${FSLDIR}/etc/fslversion | head -c 1)
ifeq ($(FSLVERSION), 5) 
  NIFTILIB = -lfslio -lniftiio 
  MATLIB = -lnewmat
else 
  UNAME := $(shell uname -s)
  ifeq ($(UNAME), Linux)
    MATLIB = -lopenblas
  endif
  NIFTILIB = -lfsl-NewNifti
endif

LIBS = -lfsl-utils -lfsl-newimage -lfsl-miscmaths -lfsl-cprob ${MATLIB} ${NIFTILIB} -lfsl-znz -lz -ldl

XFILES = fabber_ase

# Forward models
OBJS =  fwdmodel_ase.o

# For debugging:
#OPTFLAGS = -ggdb

# Build

all:	${XFILES} libfabber_models_ase.a

# Static library
libfabber_models_ase.a : ${OBJS}
	${AR} -r $@ ${OBJS}

# fabber built from the FSL fabbercore library including the models specified in this project
fabber_ase: fabber_main.o ${OBJS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $< ${OBJS} -lfsl-fabbercore -lfsl-fabberexec ${LIBS}

# DO NOT DELETE

