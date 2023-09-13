SOURCES += $(wildcard Universal_System/*.cpp)\
           $(wildcard Universal_System/Object_Tiers/*.cpp)\
           $(wildcard Universal_System/Instances/*.cpp)\
           $(wildcard Universal_System/Resources/*.cpp)
override LDLIBS += -lz

##############################################################################
#
# Link Steamworks mock API if Steamworks is not enabled.
#
##############################################################################
ifdef ENIGMA_STEAMWORKS_API_MOCK
SOURCES += $(wildcard Universal_System/Extensions/Steamworks/gameclient/SteamMock/sdk/public/steam/*.cpp)
override CXXFLAGS += -I$(ENIGMA_ROOT)/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/SteamMock/sdk/public/
endif

$(OBJDIR)/Universal_System/Object_Tiers/planar_object.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/Resources/loading.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/Resources/*init.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/image_formats.o: $(CODEGEN)/API_Switchboard.h
$(OBJDIR)/Universal_System/fileio.o: $(CODEGEN)/API_Switchboard.h
