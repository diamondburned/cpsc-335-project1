CXX = g++
CXXFLAGS = $(shell cat compile_flags.txt)

CXX_FILES = $(wildcard *.cpp)
HXX_FILES = $(wildcard *.hpp)

a.out: $(CXX_FILES) $(HXX_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(CXX_FILES)
