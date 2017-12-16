
codegen/%_generated.h: %.fbs
	flatc -I util -I resources -o codegen --cpp $<

SOURCES := $(shell find -name '*.fbs')
GENERATED := $(addprefix codegen/,$(SOURCES:.fbs=_generated.h))

.PHONY: regenerate-flatbuffers
regenerate-flatbuffers: $(GENERATED)
