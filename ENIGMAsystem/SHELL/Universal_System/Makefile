SOURCES += $(wildcard Universal_System/*.cpp)\
           $(wildcard Universal_System/Object_Tiers/*.cpp)\
           $(wildcard Universal_System/Instances/*.cpp)\
           $(wildcard Universal_System/Resources/*.cpp)
override LDLIBS += -lz
CLEANDEPS += $(OBJDIR)/Universal_System/Resources/loading.o

$(OBJDIR)/Universal_System/Object_Tiers/planar_object.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/Resources/loading.o: $(CODEGEN)/API_Switchboard.h
