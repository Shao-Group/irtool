bin_PROGRAMS=irtool

irtool_CPPFLAGS = -pthread -I$(top_srcdir)/lib/gtf -std=c++11
irtool_LDFLAGS = -L$(top_builddir)/lib/gtf -pthread
irtool_LDADD = -lgtf

irtool_SOURCES = genome1.h genome1.cc \
				   irtool.h irtool.cc \
				   config.h config.cc \
				   main.cc
