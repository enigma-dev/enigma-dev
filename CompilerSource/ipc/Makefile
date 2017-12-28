
SOURCES := $(shell find -name '*.fbs' -printf '%P\n')
GENERATED := $(addprefix codegen/,$(SOURCES:.fbs=_generated.h))
# sort has the nice side effect of removing duplicates. this may or may not be faster.
OBJDIRS := $(sort $(dir $(GENERATED)))

.PHONY: regenerate-flatbuffers

regenerate-flatbuffers: $(GENERATED)

codegen/%_generated.h: %.fbs | $(OBJDIRS)
	flatc -I util -I resources -o codegen/$(dir $*) --reflect-names --reflect-attrs --cpp $<
	flatc -I util -I resources -M --cpp $< > codegen/$*.d

$(OBJDIRS):
	mkdir -p $@

-include $(addprefix codegen/,$(SOURCES:.fbs=.d))
