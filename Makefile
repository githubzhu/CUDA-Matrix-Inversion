
CUDA_INSTALL_PATH ?= /usr/local/cuda

CXX := g++
NVCC  := nvcc

INCLUDES := -I./src -I$(CUDA_INSTALL_PATH)/include
LIBS := -lcuda

NVCCOPTIONS := -arch sm_20 -ptx

COMMONFLAGS += $(INCLUDES)
CXXFLAGS += -std=c++11 -Wall -Wshadow $(COMMONFLAGS)
NVCCFLAGS += $(COMMONFLAGS) $(NVCCOPTIONS)

CXX_SOURCES := $(wildcard src/*.cpp)
CUDA_SOURCES := $(wildcard src/*.cu)

CXX_OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(CXX_SOURCES))
CUDA_OBJECTS := $(patsubst src/%.cu,obj/%.ptx,$(CUDA_SOURCES))

TESTS := $(wildcard test/data/*.in)

.PHONY: clean test $(TESTS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.ptx: src/%.cu
	$(NVCC) $(NVCCFLAGS) $< -o $@

bin/%: test/%.cpp $(CXX_OBJECTS) $(CUDA_OBJECTS)
	$(CXX) -fPIC $(CXXFLAGS) -o $@ $< $(CXX_OBJECTS) $(LIBS)

test: bin/generator bin/run_cpu bin/run_gpu bin/verify $(TESTS)

$(TESTS):
	./bin/generator < $@ > tmp/input
	cat tmp/input > tmp/output
	./bin/run_cpu < tmp/input >> tmp/output
	./bin/run_gpu < tmp/input >> tmp/output
	./bin/verify < tmp/output

clean:
	rm -rf bin/* obj/*.o obj/*.ptx tmp/*
