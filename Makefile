SOURCE_DIR = src

INCLUDE_DIR = inc

TEST_DIR = test

SOURCE = $(SOURCE_DIR)/application.cpp \
         $(SOURCE_DIR)/main.cpp \
         $(SOURCE_DIR)/readPNG.cpp \
         $(SOURCE_DIR)/lodepng.cpp \
         $(SOURCE_DIR)/quadTree.cpp \
         $(SOURCE_DIR)/quadTreeNode.cpp \
         $(SOURCE_DIR)/alloc2d.cpp

SOURCE_TEST = $(TEST_DIR)/testMain.cpp \
              $(TEST_DIR)/testCases.cpp \
              $(SOURCE_DIR)/quadTree.cpp \
              $(SOURCE_DIR)/quadTreeNode.cpp \
              $(SOURCE_DIR)/alloc2d.cpp

OBJS = $(SOURCE:.cpp=.o)

TEST_OBJS = $(SOURCE_TEST:.cpp=.o)

#GNU C/C++ Compiler
GCC = g++

# GNU C/C++ Linker
LINK = g++

# Compiler flags
CFLAGS = -Wall -O3 -I $(INCLUDE_DIR)
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
	rm -rf $(SOURCE_DIR)/*.o $(SOURCE_DIR)/*.d core quadTree tests

debug: CXXFLAGS = -Wall -I $(INCLUDE_DIR) -DDEBUG -g -lX11 -lGL -lpng -lpthread -lstdc++fs
debug: quadTree

tar: clean
	tar zcvf quadTree.tgz $(SOURCE) $(INCLUDE_DIR)/*.h Makefile

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
