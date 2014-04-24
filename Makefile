CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g 
LDFLAGS=-g -lboost_timer -lboost_system -lboost_thread 
LDLIBS=

SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: rb_tree

rb_tree: $(OBJS)
	g++ $(LDFLAGS) -o $@ $(OBJS) 

profile: rb_tree
	perf stat -e r1c9,r4c9 $^ 2>&1 | sed -e 's/r1c9/txstarts/' -e 's/r4c9/tx aborts/'

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
