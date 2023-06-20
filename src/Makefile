BUILD_DIR=build
SOURCE_DIR=.
include $(N64_INST)/include/n64.mk

src = ndot.cpp node.cpp
N64_CXXFLAGS += -std=c++14

all: ndot.z64

OBJS = $(BUILD_DIR)/ndot.o $(BUILD_DIR)/node.o 

$(BUILD_DIR)/ndot.elf: $(OBJS)

ndot.z64: N64_ROM_TITLE="NDot test"

clean:
	rm -rf $(BUILD_DIR) ndot.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
