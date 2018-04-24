# Ben Smedley                                                                                                 
# Net ID: bss170430                                                                                          
# CS 3377.501                                                                                                
# Makefile for program 6                                                                                      

PROJECTNAME = CS3377.501.Program_6
CXX = g++
CPPFLAGS = -Wall -g $(INCPATH) -std=c++11
SRCS = cdk.cc
EXEC = Program6
OBJS = $(SRCS:cc=o)
LIBS = -l cdk -lcurses
LIBPATH = -L /scratch/perkins/lib
INCPATH = -I /scratch/perkins/include


all: $(EXEC)

clean:
	rm -f $(OBJS)*.o~ $(EXEC)


$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(LIBPATH) $(LIBS)



# Backup Target                                                                                            

backup: clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!
