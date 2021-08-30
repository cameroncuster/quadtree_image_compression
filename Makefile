SOURCE = src/application.cpp \
         src/main.cpp \
         src/readPNG.cpp \
         src/lodepng.cpp \
         src/quadTree.cpp \
         src/quadTreeNode.cpp \
         src/alloc2d.cpp

SOURCE_TEST = test/testMain.cpp \
              test/testCases.cpp \
              src/quadTree.cpp \
              src/quadTreeNode.cpp \
              src/alloc2d.cpp

OBJS = $(SOURCE:.cpp=.o)

TEST_OBJS = $(SOURCE_TEST:.cpp=.o)

#GNU C/C++ Compiler
GCC = g++

# GNU C/C++ Linker
LINK = g++

# Compiler flags
CFLAGS = -Wall -O3 -I inc
CXXFLAGS = $(CFLAGS)

# Fill in special libraries needed here
LIBS = -lX11 -lGL -lpng -lpthread -lstdc++fs

.PHONY: clean

# Targets include all, tests, clean, debug, tar

all : quadTree

quadTree: $(OBJS)
	$(LINK) -o $@ $^ $(LIBS)

tests: $(TEST_OBJS)
	$(LINK) -o $@ $^

clean:
	rm -rf src/*.o src/*.d core quadTree tests

debug: CXXFLAGS = -Wall -I inc -DDEBUG -g -lX11 -lGL -lpng -lpthread -lstdc++fs
debug: quadTree

tar: clean
	tar zcvf quadTree.tgz $(SOURCE) inc/*.h Makefile

help:
	@echo " make quadTree  - same as make all"
	@echo " make all   - builds the main target"
	@echo " make tests - builds the test suite"
	@echo " make       - same as make all"
	@echo " make clean - remove .o .d core main"
	@echo " make debug - make all with -g and -DDEBUG"
	@echo " make tar   - make a tarball of .cpp and .h files"
	@echo " make help  - this message"

-include $(SOURCE:.cpp=.d)

%.d: %.cpp
	@set -e; rm -rf $@;$(GCC) -MM $< $(CXXFLAGS) > $@
