SOURCES += $(wildcard Universal_System/*.cpp)\
           $(wildcard Universal_System/Object_Tiers/*.cpp)\
           $(wildcard Universal_System/Instances/*.cpp)\
           $(wildcard Universal_System/Resources/*.cpp)
override LDLIBS += -lz
CLEANDEPS += $(OBJDIR)/Universal_System/loading.o

Universal_System/Object_Tiers/planar_object.cpp: $(CODEGEN)/API_Switchboard.h
Universal_System/loading.cpp: $(CODEGEN)/API_Switchboard.h
