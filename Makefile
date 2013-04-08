RM  := rm -f

INCLUDE_DIRS := include /usr/local/include /opt/local/include /home/ubuntu/work/src/mongo-cxx-driver-nightly/src
CXXFLAGS      = -std=c++11 -Wall -g $(addprefix -I,$(INCLUDE_DIRS))
SRC           = $(wildcard src/*.cpp)
OBJ           = $(subst .cpp,.o,$(SRC))
EXES         := bin/logserver
CLEANLIST     = $(OBJ) $(EXES)
LDFLAGS       = -Wl,-rpath,/usr/local/lib /home/ubuntu/work/src/mongo-cxx-driver-nightly/build/libmongoclient.a -lboost_thread -lboost_filesystem -lboost_system -lboost_program_options
LD_LIBRARY_PATH += /usr/local/lib

all : bin/logserver

.PHONY : all clean cuke

clean :
	$(RM) $(CLEANLIST)

cuke : bin/logserver
	bundle exec cucumber

bin/logserver : src/logserver.o
	$(CXX) -o $@ $+ $(LDFLAGS)