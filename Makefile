# Some settings for the Makefile.  
# 
# In particular, you might need to adjust WT_BASE to wherever Wt was installed.  /usr/local is the default.

CXX=g++
WT_BASE=/usr/local
CXXFLAGS=--std=c++14 -I$(WT_BASE)/include -arch x86_64
LDFLAGS=-L$(WT_BASE)/lib -Wl,-rpath,$(WT_BASE)/lib -lwthttp -lwt -lboost_thread-mt -lboost_atomic-mt -lboost_filesystem-mt -arch x86_64

# Identify our code base to be building.  Headers are listed under DEPS, and objects under OBJS.

DEPS = wordle.h
OBJS = main.o wordle.o

# How do we build objects?

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# How do we build the executable?

hello: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

# How do we clean up after ourselves?

clean:
	rm -f hello $(OBJS)