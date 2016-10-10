CFLAGS = @GCC_CFLAG@ @CFLAGS@  
LDADD = -lCA -lx2dview -lm -L../case -L../x2dview @X_LIBS@ -lXmu -lXext -lXaw -lXt -lX11 
INCLUDES = -I../case -I. -I../x2dview -I..
LDFLAGS = 

bin_PROGRAMS = CASE-model

CASE_model_SOURCES =							\
	CASE-model.C	\
	GoLModel.C	\
	GameModel.C	\
	Periodic.C    	\
	TriModel.C    	\
	XYBModel.C	\
	DiffModel.C   	\
	Palladium.C     \
	Particle.C      \
	ThermoCell.C  	\
	TriPattern.C  	\
	XYCell.C	\
	GoLCell.C     	\
	GameCell.C     	\
	ParticleModel.C \
	TriCell.C     	\
	XYBCell.C     	\
	XYModel.C	

#
# Pickup header files here so Automake knows about them
#
noinst_HEADERS =	\
	CASE-model.h  	\
	GameCell.h   	\
	GoLCell.h   	\
	Network.h    	\
	Particle.h      \
	Periodic.h    	\
	TriCell.h   	\
	XYBCell.h   	\
	XYCell.h	\
	DiffModel.h   	\
	GoLModel.h  	\
	GameModel.h  	\
	Palladium.h  	\
	ParticleModel.h \
	ThermoCell.h  	\
	TriModel.h  	\
	XYBModel.h  	\
	XYModel.h

#
# Some basic clean ups
#
MOSTLYCLEANFILES = *~ *.orig *.rej

#
# Get everything removed down to where rebuilding requires:
# "make; make install"
# 
CLEANFILES = cflex.c cfparse.c cfparse.h

#
# Get everything removed down to where rebuilding requires:
# "configure; make; make install"
#
DISTCLEANFILES = cflex.c cfparse.c cfparse.h

#
# Get everything removed down to where rebuilding requires:
# "aclocal; autoconf; autoheader; automake --add-missing"
# "configure; make; make install"
#
MAINTAINERCLEANFILES = Makefile.in conf.h.in stamp-h.in
