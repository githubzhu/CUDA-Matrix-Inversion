
CUDA_INSTALL_PATH ?= /usr/local/cuda

CXX := g++
NVCC  := /usr/local/cuda/bin/nvcc

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
PTESTS := $(wildcard benchmark/data/*.in)

.DEFAULT_GOAL := run
.PHONY: install run test clean $(TESTS) $(PTESTS)
.SECONDARY:

install:
	mkdir bin obj tmp

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.ptx: src/%.cu
	$(NVCC) $(NVCCFLAGS) $< -o $@

bin/%: test/%.cpp $(CXX_OBJECTS) $(CUDA_OBJECTS)
	$(CXX) -fPIC $(CXXFLAGS) -o $@ $< $(CXX_OBJECTS) $(LIBS)

test: $(TESTS)

$(TESTS): bin/generator bin/run_cpu bin/run_gpu bin/verify
	@echo "Running test" $@
	./bin/generator < $@ > tmp/input
	cat tmp/input > tmp/output
	./bin/run_cpu < tmp/input >> tmp/output
	./bin/run_gpu < tmp/input >> tmp/output
	./bin/verify < tmp/output

benchmark: $(PTESTS)

$(PTESTS): bin/generator bin/run_cpu bin/run_gpu
	@echo "Running test" $@
	@./bin/generator < $@ > tmp/input
	@timeout 5s time -f "CPU: Elapsed real time: %es" ./bin/run_cpu < tmp/input > /dev/null || echo "CPU: Process exceeded 5s time limit"
	@timeout 10s time -f "GPU: Elapsed real time: %es" ./bin/run_gpu < tmp/input > /dev/null || echo "GPU: Process exceeded 10s time limit"

run: bin/generator bin/run_gpu
	./bin/generator < test/data/0.2.Z2.in > tmp/input
	./bin/run_gpu < tmp/input > tmp/output
	cat tmp/input
	cat tmp/output

clean:
	rm -rf bin/* obj/*.o obj/*.ptx tmp/*
