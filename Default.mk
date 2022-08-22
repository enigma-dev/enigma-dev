.PHONY: clean obj_dirs .FORCE

.DEFAULT_GOAL := $(TARGET)

.FORCE:

# libProtocols uses non-standard file extensions so it handles it's own objects list
ifndef OBJECTS
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))
endif

OBJ_DIRS = $(sort $(OBJ_DIR) $(dir $(OBJECTS)))
DEPENDS = $(OBJECTS:.o=.d)

ifdef SHARED_SRC_DIR
CXXFLAGS += -I$(SHARED_SRC_DIR) $(addprefix -I$(SHARED_SRC_DIR)/, $(SHARED_INCLUDES))
SOURCES += $(addprefix $(SHARED_SRC_DIR),$(SHARED_SOURCES))
OBJECTS += $(addprefix $(OBJ_DIR)/shared/,$(SHARED_SOURCES:.cpp=.o))
DEPENDS += $(addprefix $(OBJ_DIR)/shared/,$(SHARED_SOURCES:.cpp=.d))
endif

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	$(LINKING_TARGETS)

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

# Create the object directories
$(OBJ_DIRS):
	mkdir -p $@

# Old make has a bug which requires this nonsense...
obj_dirs: $(OBJ_DIRS)

# Default g++ rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Shared srcs g++ rules
$(OBJ_DIR)/shared/%.o: $(SHARED_SRC_DIR)/%.cpp | obj_dirs
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Proto rules
# The order-only dependency on $(GENERATED) is an over-approximation to force all protos to be built
# before any object files. This matters on the first run when Make can't yet know about #includes.
$(OBJ_DIR)/%.pb.o $(OBJ_DIR)/%.pb.d: $(OBJ_DIR)/%.pb.cc | $(GENERATED)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $(OBJ_DIR)/$*.pb.o $(OBJ_DIR)/$*.pb.cc

$(OBJ_DIR)/%.pb.cc $(OBJ_DIR)/%.pb.h: %.proto | obj_dirs 
	protoc -I. --cpp_out=$(OBJ_DIR) $<

$(OBJ_DIR)/%.grpc.pb.cc $(OBJ_DIR)/%.grpc.pb.h: %.proto | obj_dirs 
	protoc -I. --grpc_out=$(OBJ_DIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: $(GENERATED)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif
