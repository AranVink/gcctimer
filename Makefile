INSTALL = install

DIST = gcctimer.c gcctimer.h Makefile

VERSION = 1.0
NAME = gcctimer-$(VERSION)

prefix = /usr/local
includedir = $(prefix)/include
libdir = $(prefix)/lib

libgcctimer.a: gcctimer.o
	$(AR) cru $@ $?
	$(RANLIB) $@

gcctimer.o: gcctimer.c gcctimer.h

clean:
	rm -f *.o *.a

dist:
	mkdir $(NAME)
	cp $(DIST) $(NAME)
	tar cfj $(NAME).tar.bz2 $(NAME)
	rm -r $(NAME)

install: libgcctimer.a
	$(INSTALL) -d $(includedir)
	$(INSTALL) -m 644 gcctimer.h $(includedir)
	$(INSTALL) -m 644 libgcctimer.a $(libdir)
