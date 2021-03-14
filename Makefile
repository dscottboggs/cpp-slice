CXX_COMPILER := clang++
LFLAGS := -lfmt
CXX_STANDARD := c++2a

all:

tests: bin/test-runner
	bin/test-runner

bin/test-runner:
	-mkdir bin
	$(CXX_COMPILER) $(LFLAGS) -g -std=$(CXX_STANDARD) -obin/test-runner test.cpp

clean:
	-rm bin/*