# Justin B. McBride
# WSU ID: 99921839
# $Source$
# $Revision$
# $Date$
# $Log$

PROG = $(shell basename $(PWD))

OSNAME = $(shell ./systype.sh)

LIBS = 

HDRS_IMPORTED =

HDRS_HERE =

SRCS = \
	aalex.c

OBJS = $(SRCS:.c=.o)

MAKETYPE = DEBUG

CC = cc

SYSENV_DEFS = -DOSNAME_$(OSNAME)

CFLAGS = $(CFLAGS_$(MAKETYPE)) $(INCLIST) $(SYSENV_DEFS)
CFLAGS_DEBUG = -g $(CFLAGS_DEBUG_$(OSNAME))
CFLAGS_DEBUG_IRIX = -O -fullwarn
CFLAGS_DEBUG_SOLARIS = -O
CFLAGS_DEBUG_LINUX = -Wall -Wstrict-prototypes
CFLAGS_OPTIMIZED = -O -DNDEBUG
CFLAGS_PROFILE = -pg -O -DNDEBUG

LD = $(CC)

LDFLAGS=$(LDFLAGS_$(MAKETYPE)) $(LDFLAGS_$(OSNAME))
LDFLAGS_DEBUG = -g
LDFLAGS_OPTIMIZED =
LDFLAGS_PROFILE = -pg

PR = a2ps

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

default: $(PROG)

all:	default

checkin:
	ci -l Makefile $(HDRS_HERE) $(SRCS)

clean:
	rm -f a.out core* /cores/core* *% $(OBJS) $(GENFILES) Makefile.BAK *.bak *~

clobber:	clean
	rm -f $(PROG) 


install:	$(BINDEST)/$(PROG)

$(BINDEST)/$(PROG):	$(PROG)
	cp $(PROG) $(BINDEST)/$(PROG)

list:	$(HDRS_HERE) $(SRCS)
	@$(PR) $(HDRS_HERE) $(SRCS)

rcsupdate:
	ci -l Makefile $(SRCS) $(HDRS_HERE)

tags:	$(SRCS) $(HDRS_HERE)
	ctags -t $(SRCS) $(HDRS_HERE) >tags

$(OBJS): $(HDRS_HERE) $(HDRS_IMPORTED)

$(PROG): $(OBJS) $(LIBS)
	@echo $(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(EXTOBJS) $(LIBS) $(LIBFLAGS) $(LIBFLAGS_PAUL)
	@if $(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(EXTOBJS) $(LIBS) $(LIBFLAGS) $(LIBFLAGS_PAUL) ;\
	then \
		echo $(PROG) linked ;\
	else \
		echo errors in link, $(PROG) executable removed ;\
		/bin/rm $(PROG) ;\
	fi

test: $(TESTOBJS)
	$(CC) $(CFLAGS) $(INCLIST) -DTEST $(SRC) -o `basename $(SRC) .c`_t $(TESTOBJS) $(LIBS) $(LIBFLAGS) $(LIBFLAGS_PAUL)

profile:
	$(MAKE) MAKETYPE=PROFILE PROG=$(PROG)_p
