## -*- Makefile -*-
##
## User: blixit
## Time: 4 août 2016 00:30:27
## Makefile created by Oracle Developer Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g -Wall
CCFLAGS = $(BASICOPTS)  -std=c++11
CXXFLAGS = $(BASICOPTS) -std=c++11
CCADMIN = 


# Define the target directories.
TARGETDIR_all=.

all : clean ph pc
	
## Target: all
CPPFLAGS_all = \
	-Iinclude 
OBJS_ph =  \
	$(TARGETDIR_all)/penduhost.o  
OBJS_pc =  \
	$(TARGETDIR_all)/penduclient.o
SYSLIBS_all = -pthread 
USERLIBS_all = $(SYSLIBS_all) 
DEPLIBS_all = libsgmtp.a    
LDLIBS_all = $(USERLIBS_all)


# Link or archive 	
$(TARGETDIR_all)/ph: $(TARGETDIR_all) $(OBJS_ph) $(DEPLIBS_all) 
	$(LINK.cc) $(CCFLAGS_all) $(CPPFLAGS_all) -o $@ $(OBJS_ph) $(LDLIBS_all) $(DEPLIBS_all)
	
$(TARGETDIR_all)/pc: $(TARGETDIR_all) $(OBJS_pc) $(DEPLIBS_all) 
	$(LINK.cc) $(CCFLAGS_all) $(CPPFLAGS_all) -o $@ $(OBJS_pc) $(LDLIBS_all) $(DEPLIBS_all) 

# Compile source files into .o files 
$(TARGETDIR_all)/penduhost.o: $(TARGETDIR_all) penduhost.cc
	$(COMPILE.cc)  $(CCFLAGS_all) $(CPPFLAGS_all)   penduhost.cc -o $@
$(TARGETDIR_all)/penduclient.o: $(TARGETDIR_all) penduclient.cc
	$(COMPILE.cc)  $(CCFLAGS_all) $(CPPFLAGS_all)   penduclient.cc -o $@



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_all)/ph \
		$(TARGETDIR_all)/pc \
		$(TARGETDIR_all)/penduhost.o \
		$(TARGETDIR_all)/penduclient.o 
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

