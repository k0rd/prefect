PROGNAME=Prefect
# Compiler and flags
CC=g++
CCFLAGS=-w -O3 -s -pthread
PATHSRC=./
PATHOUT=bin/
CRM32Pro=/usr/CRM32Pro/CRM32Pro.so -I/usr/CRM32Pro
X11=-L/usr/X11/lib -lX11 -lXext

# Construccion de ejemplos
all: $(PROGNAME)
$(PROGNAME): 
	@echo
	@echo Compiling $(PROGNAME)...
	$(CC) $(CCFLAGS) $(PATHSRC)prefect.cpp -o $(PATHOUT)prefect $(CRM32Pro) $(X11) -DLINUX

package:
	mkdir prefect
	cp bin/prefect.dpf prefect/
	cp bin/setup.dpf prefect/
	cp bin/prefect prefect/
	tar cf prefect_linux.tar prefect
	bzip2 prefect_linux.tar
	mv prefect_linux.tar.bz2 Deploys
	rm -rf prefect

