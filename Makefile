RM    := rm -f
CP    := cp
MKDIR := mkdir -p
AR    := ar -rv
SED   := sed -e
MONGO := mongo

VPATH = src include test

INCLUDE_DIRS := src include /usr/local/include /opt/local/include
CXXFLAGS      = -std=c++11 -Wall -g $(MACROS) $(addprefix -I,$(INCLUDE_DIRS))
SRC           = $(wildcard src/*.cpp)
OBJ           = $(subst .cpp,.o,$(SRC))
TSRC          = $(wildcard test/*.cpp) $(wildcard test/fhq/*.cpp) $(wildcard test/sanity/*.cpp) $(wildcard test/fhq/processors/*.cpp)
TOBJ          = $(subst .cpp,.o,$(TSRC))
EXES         := bin/gogomongo test/test_runner
CLEANLIST     = $(OBJ) $(TOBJ) $(EXES) $(wildcard log/*.log) $(subst .o,.dpp,$(OBJ)) $(subst .o,.dpp,$(TOBJ))
LDFLAGS       = -Wl,-rpath,/usr/local/lib /usr/local/lib/libmongoclient.a -lboost_thread -lboost_filesystem -lboost_system -lboost_program_options
LD_LIBRARY_PATH += /usr/local/lib
CXXMAKEDEPS   = $(CXX) -M $(CXXFLAGS) $(CPPFLAGS)

all : bin/gogomongo

.PHONY : all clean cuke wip test install

clean :
	$(RM) $(CLEANLIST)

cuke : bin/gogomongo
	bundle exec cucumber

wip : bin/gogomongo
	bundle exec cucumber -p wip

test : test/test_runner
  ifdef TF
	test/test_runner --gtest_filter=$(TF)
  else
	test/test_runner
  endif

install :
	$(CP) bin/gogomongo /usr/local/bin/gogomongo      && \
	$(MKDIR) /etc/gogomongo                           && \
	$(CP) config/conf.json /etc/gogomongo/conf.json   && \
	$(CP) config/builddb.js /etc/gogomongo/builddb.js && \
	$(MONGO) /etc/gogomongo/builddb.js

bin/gogomongo : MACROS = -DPRIVATE=private -DPROTECTED=protected -DVIRTUAL="" -D"TEST_ONLY(n)=/* n */"
bin/gogomongo : src/gogomongo.o
	$(CXX) -o $@ $+ $(LDFLAGS)

test/test_runner.o test/test_runner.dpp %_test.o %_test.dpp : INCLUDE_DIRS += gtest/include test
test/test_runner %_test.o : MACROS = -DPRIVATE=public -DPROTECTED=public -DVIRTUAL=virtual -D"TEST_ONLY(n)=n"

test/test_runner : $(TOBJ) lib/libgtest.a
	$(CXX) -o $@ $+ $(LDFLAGS) -pthread
lib/libgtest.a : gtest/src/gtest-all.o
	$(AR) $@ $^
gtest/src/gtest-all.o : gtest/src/gtest-all.cc
	$(CXX) -std=c++11 -Wall -g -Igtest/include -Igtest -pthread -c $< -o $@
gtest/src/gtest-all.cc :
	svn checkout http://googletest.googlecode.com/svn/trunk/ gtest

ifneq "$(MAKECMDGOALS)" "clean"
  ifneq "$(MAKECMDGOALS)" "install"
    -include $(subst .cpp,.dpp,$(SRC)) -include $(subst .cpp,.dpp,$(TSRC))
  endif
endif

%.dpp : %.cpp
	$(CXXMAKEDEPS) $< > $*._dpp && \
	$(SED) 's,$(*F)\.o[ :]*,$*.o $@ : ,g' < $*._dpp > $@ && \
	$(RM) $*._dpp
