
# project name and executable name
project = app
# source suffix
source = .cpp 
# object files to make
objects = Engine.o MuDst.o VpdOfflineSlewPicoDstMaker.o

sources = $(wildcard *.cpp)
headers = $(wildcard ../include/*.h)

# ROOT libs and includes
ROOTCFLAGS    	= $(shell root-config --cflags)
ROOTLDFLAGS    	= $(shell root-config --ldflags)
ROOTLIBS      	= $(shell root-config --libs)
ROOTGLIBS     	= $(shell root-config --glibs)

# Custom Project includes and libs
# -lGui -lHtml -lGuiHtml
includes		= -I../include -I$(JDB_LIB)/include/jdb -I$(JDB_LIB)/include/ -I. -I$(ROOTSYS)/include
libraries 		= $(ROOTLIBS) $(SYSLIBS)
staticLibs 		= $(JDB_LIB)/lib/libJDB.a

cxx 		= g++ -std=c++11 -O3 -DJDB_LOG_LEVEL=30
flags 		= -Wall -g $(ROOTCFLAGS) $(includes) -Wno-write-strings -fno-inline 
compile 	= $(cxx) $(flags) -c 
ldFlags  	= $(ROOTLDFLAGS) 

all: $(project)

$(project): $(objects)
		$(cxx) $(ldFlags) -o  $(project) $(objects) $(staticLibs) $(ROOTGLIBS) $(libraries)
		@cp $(project) ../bin

%.o: %$(source)
		$(compile)  $<

clean:
		@rm -f $(objects) $(project)
		@rm -f ../bin/$(project)
		@echo "clean done"

full:
	clean
	$(project)
