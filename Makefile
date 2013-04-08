RM  := rm -f
AR  := ar -rv

INCLUDE_DIRS := src include /usr/local/include /opt/local/include /home/ubuntu/work/src/mongo-cxx-driver-nightly/src
CXXFLAGS      = -std=c++11 -Wall -g $(addprefix -I,$(INCLUDE_DIRS))
SRC           = $(wildcard src/*.cpp)
OBJ           = $(subst .cpp,.o,$(SRC))
EXES         := bin/logserver
CLEANLIST     = $(OBJ) $(EXES) $(wildcard log/*.log)
LDFLAGS       = -Wl,-rpath,/usr/local/lib /home/ubuntu/work/src/mongo-cxx-driver-nightly/build/libmongoclient.a -lboost_thread -lboost_filesystem -lboost_system -lboost_program_options
LD_LIBRARY_PATH += /usr/local/lib

all : bin/logserver

.PHONY : all clean cuke wip

clean :
	$(RM) $(CLEANLIST)

cuke : bin/logserver
	bundle exec cucumber

wip : bin/logserver
	bundle exec cucumber -p wip

src/logserver.o : src/b23/file.h src/fhq/log_server.h

bin/logserver : src/logserver.o
	$(CXX) -o $@ $+ $(LDFLAGS)

lib/libgtest.a : gtest/src/gtest-all.o
	$(AR) $@ $^
gtest/src/gtest-all.o : gtest/src/gtest-all.cc
	$(CXX) -std=c++11 -Wall -g -Igtest/include -Igtest -pthread -c $< -o $@
gtest/src/gtest-all.cc :
	svn checkout http://googletest.googlecode.com/svn/trunk/ gtest
