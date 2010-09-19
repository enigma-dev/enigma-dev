LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ndkEnigmaGame

CG_SUBDIRS := \
../../../../../SHELL \
../../../../../SHELL/Preprocessor_Environment_Editable \
../../../../../SHELL/Universal_System \
../../../../../SHELL/Universal_System/instance \
../../../../../SHELL/Graphics_Systems/OPENGLES \
../../../../../SHELL/Platforms/Android \
../../../../../SHELL/Audio_Systems/androidAudio \
../../../../../SHELL/Collision_Systems/none \


LOCAL_SRC_FILES := $(foreach F, $(CG_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.cpp))))

LOCAL_LDLIBS    := -lz -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)