RM  := rm -f
AR  := ar -rv
SED := sed -e

VPATH = src include

INCLUDE_DIRS := src include /usr/local/include /opt/local/include /home/ubuntu/work/src/mongo-cxx-driver-nightly/src
CXXFLAGS      = -std=c++11 -Wall -g $(addprefix -I,$(INCLUDE_DIRS))
SRC           = $(wildcard src/*.cpp)
OBJ           = $(subst .cpp,.o,$(SRC))
TSRC          = $(wildcard test/*.cpp) $(wildcard test/fhq/*.cpp)
TOBJ          = $(subst .cpp,.o,$(TSRC))
EXES         := bin/logserver test/test_runner
CLEANLIST     = $(OBJ) $(TOBJ) $(EXES) $(wildcard log/*.log) $(subst .o,.dpp,$(OBJ)) $(subst .o,.dpp,$(TOBJ)) $(subst .o,._dpp,$(OBJ)) $(subst .o,._dpp,$(TOBJ))
LDFLAGS       = -Wl,-rpath,/usr/local/lib /home/ubuntu/work/src/mongo-cxx-driver-nightly/build/libmongoclient.a -lboost_thread -lboost_filesystem -lboost_system -lboost_program_options
LD_LIBRARY_PATH += /usr/local/lib
CXXMAKEDEPS   = $(CXX) -M $(CXXFLAGS) $(CPPFLAGS)

all : bin/logserver

.PHONY : all clean cuke wip

clean :
	$(RM) $(CLEANLIST)

cuke : bin/logserver
	bundle exec cucumber

wip : bin/logserver
	bundle exec cucumber -p wip

test : test/test_runner
	test/test_runner

# src/logserver.o : b23/file.h fhq/log_server.h

bin/logserver : src/logserver.o
	$(CXX) -o $@ $+ $(LDFLAGS)

%_test.o %_test.dpp : SRC += $(TSRC)
%_test.o %_test.dpp : INCLUDE_DIRS += gtest/include
# test/fhq/log_server_test.o : fhq/log_server.h

test/test_runner.o test/test_runner.dpp : SRC += $(TSRC)
test/test_runner.o test/test_runner.dpp : INCLUDE_DIRS += gtest/include

test/test_runner : $(TOBJ) lib/libgtest.a
	$(CXX) -o $@ $+ $(LDFLAGS) -pthread
lib/libgtest.a : gtest/src/gtest-all.o
	$(AR) $@ $^
gtest/src/gtest-all.o : gtest/src/gtest-all.cc
	$(CXX) -std=c++11 -Wall -g -Igtest/include -Igtest -pthread -c $< -o $@
gtest/src/gtest-all.cc :
	svn checkout http://googletest.googlecode.com/svn/trunk/ gtest

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(subst .cpp,.dpp,$(SRC))
endif

%.dpp : %.cpp
	$(CXXMAKEDEPS) $< > $*._dpp && \
	echo $* && \
	$(SED) 's,$(*F)\.o[ :]*,$*.o $@ : ,g' < $*._dpp > $@ && \
	$(RM) $*._dpp
