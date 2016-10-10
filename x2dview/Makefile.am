INCLUDES = -I..
CFLAGS = @CFLAGS@ 

noinst_LIBRARIES = libx2dview.a

libx2dview_a_SOURCES =							\
	X2Dview.c  							\
	color.c  							\
	converters.c  							\
	str2double.c  							\
	trans2d.c  							\
	widget_tools.c

EXTRA_DIST =								\
	X2Dview.h  							\
	X2DviewP.h  							\
	color.h    							\
	converters.h    						\
	str2double.h    						\
	trans2d.h    							\
	widget_tools.h

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
