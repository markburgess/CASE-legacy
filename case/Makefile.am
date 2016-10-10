INCLUDES = -I.. -I../x2dview
CFLAGS = @CFLAGS@

noinst_LIBRARIES = libCA.a

libCA_a_SOURCES =							\
	CAApp.C   							\
	CAEnvRect.C  \
	CAEnviron.C  \
	CAProperty.C  \
	CASymbol.C  \
	filereq.C  \
	toolkit.C \
	CACell.C  \
	CAEnvTri.C \
	CAModel.C   \
	CAReq.C      \
	CAVisual.C  \
	menu.C     \
	taglist.C

EXTRA_DIST =								\
	CA.h     \
	CACell.h  \
	CAModel.h  \
	CAReq.h     \
	CAVisual.h  \
	menu.h     \
	toolkit.h \
	CAApp.h  \
	CAEnviron.h   \
	CAProperty.h  \
	CASymbol.h  \
	filereq.h   \
	taglist.h  \
	types.h

#
# Some basic clean ups
#
MOSTLYCLEANFILES = *~

#
# Get everything removed down to where rebuilding requires:
# "make; make install"
# 
CLEANFILES =

#
# Get everything removed down to where rebuilding requires:
# "configure; make; make install"
#
DISTCLEANFILES = 

#
# Get everything removed down to where rebuilding requires:
# "aclocal; autoconf; autoheader; automake --add-missing"
# "configure; make; make install"
#
MAINTAINERCLEANFILES = Makefile.in
