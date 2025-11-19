CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
SRC_DIR := RGG-Backend
SRCS := $(SRC_DIR)/Graph.cpp \
        $(SRC_DIR)/read_graph.cpp \
        $(SRC_DIR)/show_graph.cpp \
        $(SRC_DIR)/RGG-Backend.cpp
TARGET := rgg-parser

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
