SOURCES += $(wildcard Universal_System/*.cpp)\
           $(wildcard Universal_System/Object_Tiers/*.cpp)\
           $(wildcard Universal_System/Instances/*.cpp)\
           $(wildcard Universal_System/Resources/*.cpp)
ifeq ($(OS), Darwin)
else ifeq ($(OS), FreeBSD)
else ifeq ($(OS), DragonFly)
else ifeq ($(OS), OpenBSD)
else
	override LDFLAGS += -lz
	override LDLIBS += -lz
endif

$(OBJDIR)/Universal_System/Object_Tiers/planar_object.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/Resources/loading.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/Resources/*init.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/image_formats.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/fileio.o: $(CODEGEN)/API_Switchboard.h
