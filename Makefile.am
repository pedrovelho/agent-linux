#Makefile: makefile for paagent.cpp

bin_PROGRAMS=paagent
paagent_SOURCES=paagent.cpp Controller.cpp Event.cpp NodeStarter.cpp Watcher.cpp 
paagent_LDADD = -llog4cxx -lccext2 -lccgnu2 -lz -ldl -lrt -ldbus-1
paagent_LDFLAGS = -pthread


