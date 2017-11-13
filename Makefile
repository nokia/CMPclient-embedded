# vim: set noexpandtab:

CMPCLDIR=CMPclient-embedded-lib
LDIR=$(CMPCLDIR)/mbedtls/library
IDIR=$(CMPCLDIR)/mbedtls/include

gcc_warn=-DDEBUG_UNUSED -Wswitch -DPEDANTIC -pedantic -Wno-long-long -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wsign-compare -Wmissing-prototypes -Wshadow -Wformat -Wtype-limits -Wundef
#-Werror

CC=gcc
CFLAGS=-g3 -O0 $(gcc_warn) -I$(IDIR) -I$(CMPCLDIR) -L$(CMPCLDIR) -L$(LDIR)
CPPFLAGS=-g3 -O0 $(gcc_warn) -I$(IDIR) -I$(CMPCLDIR) -L$(CMPCLDIR) -L$(LDIR)
ODIR=obj

LIBS=-lmbedcrypto -lmbedtls -lmbedx509 -lcmpcl

CMPLIB = $(CMPCLDIR)/libcmpcl.a

_DEPS = cmpcl_certs.h
DEPS = $(patsubst %, ./%, $(_DEPS))

_OBJ = cmpclient.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: ./%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cmpclient: $(OBJ) $(CMPLIB)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: .FORCE
$(CMPLIB): .FORCE
	make -C $(CMPCLDIR)

.PHONY: tags
tags:
	-ctags -R .

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o cmpclient

.PHONY: distclean
distclean: clean
	make -C $(CMPCLDIR) distclean
