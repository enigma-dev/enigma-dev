ifeq ($(TARGET-PLATFORM), Android)
  override CXXFLAGS += -IPlatforms/SDL/fileio
  SOURCES += Platforms/SDL/fileio/fileio.cpp
else
  override CXXFLAGS += -IPlatforms/General/fileio
  SOURCES += Platforms/General/fileio/fileio.cpp
endif 
