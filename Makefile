CXX=g++
CXXFLAGS=-Wall -Werror -std=c++14 -O2 -DNDEBUG

all:
	${CXX} ${CXXFLAGS} segment_tree.cpp -o segment_tree

