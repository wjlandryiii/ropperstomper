# Wedit Makefile for project ropperstomper
SRCDIR=c:\lcc\projects\ropperstomper
CFLAGS=-I"c:\lcc\include" -I"C:\lcc\include"  -g2 
CC=$(LCCROOT)\bin\lcc.exe
LINKER=$(LCCROOT)\bin\lcclnk.exe
OBJS=\
	ropperstomper.obj \
	llist.obj \
	instructions_x86.obj \
	image.obj \
	gadgets.obj \
	gadget_search.obj \
	bintree.obj

LIBS=
EXE=ropperstomper.exe

$(EXE):	$(OBJS) Makefile
	$(LINKER)  -subsystem console -o $(SRCDIR)\lcc\ropperstomper.exe $(OBJS) $(LIBS)

# Build ropperstomper.c
ROPPERSTOMPER_C=\
	$(SRCDIR)\instructions_x86.h\
	$(SRCDIR)\gadgets.h\
	$(SRCDIR)\bintree.h\
	$(SRCDIR)\bintree.h\
	$(SRCDIR)\image.h\
	$(SRCDIR)\llist.h\
	$(SRCDIR)\llist.h\

ropperstomper.obj: $(ROPPERSTOMPER_C) $(SRCDIR)\ropperstomper.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\ropperstomper.c

# Build llist.c
LLIST_C=\
	$(SRCDIR)\llist.h\

llist.obj: $(LLIST_C) $(SRCDIR)\llist.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\llist.c

# Build instructions_x86.c
INSTRUCTIONS_X86_C=\
	$(SRCDIR)\instructions_x86.h\

instructions_x86.obj: $(INSTRUCTIONS_X86_C) $(SRCDIR)\instructions_x86.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\instructions_x86.c

# Build image.c
IMAGE_C=\
	$(SRCDIR)\image.h\
	$(SRCDIR)\llist.h\

image.obj: $(IMAGE_C) $(SRCDIR)\image.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\image.c

# Build gadgets.c
GADGETS_C=\
	c:\lcc\include\_syslist.h\
	c:\lcc\include\safelib.h\
	c:\lcc\include\safelib.h\
	$(SRCDIR)\gadgets.h\
	$(SRCDIR)\bintree.h\
	$(SRCDIR)\bintree.h\

gadgets.obj: $(GADGETS_C) $(SRCDIR)\gadgets.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\gadgets.c

# Build gadget_search.c
GADGET_SEARCH_C=\
	$(SRCDIR)\gadget_search.h\

gadget_search.obj: $(GADGET_SEARCH_C) $(SRCDIR)\gadget_search.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\gadget_search.c

# Build bintree.c
BINTREE_C=\
	$(SRCDIR)\bintree.h\

bintree.obj: $(BINTREE_C) $(SRCDIR)\bintree.c
	$(CC) -c $(CFLAGS) $(SRCDIR)\bintree.c

link:
	$(LINKER)  -subsystem console -o $(SRCDIR)\lcc\ropperstomper.exe $(OBJS) $(LIBS)

clean:
	del $(OBJS) ropperstomper.exe
