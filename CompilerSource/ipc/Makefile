
codegen/%_generated.h: %.fbs
	flatc -o codegen --cpp $<

SOURCES := $(wildcard *.fbs)
GENERATED := $(addprefix codegen/,$(SOURCES:.fbs=_generated.h))

.PHONY: regenerate-flatbuffers
regenerate-flatbuffers: $(GENERATED)
