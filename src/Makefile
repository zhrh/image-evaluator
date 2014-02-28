INCDIR = ../include
LIBDIR = ../lib

ODIR = ../build

CXX = g++
CXXFLAGS = -fPIC -Wall -I$(INCDIR)
LDFLAGS = -L$(LIBDIR)
LDLIBS =  -lmysqlclient

PROG = evaluator 
LIB =

MAINOBJ = $(ODIR)/main.o
LIBOBJS = $(ODIR)/MySQL_API.o

all: $(PROG) $(LIB)

$(PROG):$(MAINOBJ) $(LIBOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(LIB):$(LIBOBJS)
	$(CXX) $^ -o $@ -shared

$(ODIR)/%.o:$(addprefix ./, %.cpp)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY:clean

clean:
	rm $(ODIR)/*.o evaluator
