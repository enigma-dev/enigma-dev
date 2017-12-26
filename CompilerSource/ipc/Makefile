
SOURCES := $(shell find -name '*.fbs' -printf '%P\n')
GENERATED := $(addprefix codegen/,$(SOURCES:.fbs=_generated.h))
OBJS := $(addprefix codegen/,$(SOURCES:.fbs=.bfbs))
# sort has the nice side effect of removing duplicates. this may or may not be faster.
OBJDIRS := $(sort $(dir $(GENERATED)))

.PHONY: regenerate-flatbuffers

regenerate-flatbuffers: $(GENERATED)
regenerate-flatbuffers: $(OBJS)

codegen/%_generated.h: %.fbs | $(OBJDIRS)
	flatc -I util -I resources -o codegen/$(dir $*) --cpp $<
	flatc -I util -I resources -M --cpp $< > codegen/$*.d

codegen/%.bfbs: %.fbs | $(OBJDIRS)
	flatc -I util -I resources -o codegen/$(dir $*) --binary --schema $<

$(OBJDIRS):
	mkdir -p $@

-include $(addprefix codegen/,$(SOURCES:.fbs=.d))
