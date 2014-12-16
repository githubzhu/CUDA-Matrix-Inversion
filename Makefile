
CUDA_INSTALL_PATH ?= /usr/local/cuda

CXX := g++
NVCC  := nvcc

INCLUDES := -I./src -I$(CUDA_INSTALL_PATH)/include
LIBS := -lcuda

NVCCOPTIONS := -arch sm_20 -ptx

COMMONFLAGS += $(INCLUDES)
CXXFLAGS += $(COMMONFLAGS)
NVCCFLAGS += $(COMMONFLAGS) $(NVCCOPTIONS)

CXX_SOURCES := $(wildcard src/*.cpp)
CUDA_SOURCES := $(wildcard src/*.cu)

CXX_OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(CXX_SOURCES))
CUDA_OBJECTS := $(patsubst src/%.cu,obj/%.ptx,$(CUDA_SOURCES))

TARGET := bin/test.x

.SUFFIXES: .cpp .cu .o .ptx

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.ptx: src/%.cu
	$(NVCC) $(NVCCFLAGS) $< -o $@

$(TARGET): $(CXX_OBJECTS) $(CUDA_OBJECTS)
	mkdir -p bin obj
	$(CXX) -fPIC -o $(TARGET) $(CXX_OBJECTS) $(LIBS)

clean:
	rm -rf $(TARGET) obj/*.o obj/*.ptx
