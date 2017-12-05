LOCAL_PATH				:= $(call my-dir)
MY_PATH					:= $(LOCAL_PATH)/../../../src/eepp
INC_PATH				:= $(LOCAL_PATH)/../../../include
BASE_PATH				:= $(LOCAL_PATH)/../../../src

MY_SDL_PATH				:= $(MY_PATH)/helper/SDL2
MY_SDL_MAIN_PATH		:= helper/SDL2/src/main/android/*.c

MY_C_INCLUDES			:= \
	$(MY_PATH)/helper/openal-soft/include/ \
	$(MY_PATH)/helper/freetype2/include \
	$(MY_SDL_PATH)/include \
	$(MY_PATH)/helper/chipmunk \
	$(INC_PATH)/eepp/helper/chipmunk \
	$(MY_PATH)/helper/SOIL2/src/SOIL2 \
	$(MY_PATH)/helper/stb_vorbis \
	$(INC_PATH)/eepp/helper/chipmunk

MY_C_FLAGS				:= \
	-Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit \
	-DANDROID \
	-DANDROID_NDK \
	-DDISABLE_IMPORTGL \
	-w \
	-Wno-unknown-pragmas \
	$(EE_GLES_VERSION) \
	-DEE_NO_SNDFILE \
	-D$(EE_SDL_VERSION) \
	-I$(INC_PATH) \
	-I$(BASE_PATH)

MY_LDLIBS				:= $(APP_LDLIBS)

include $(call all-subdir-makefiles) 

#*************** EEPP ***************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)

LOCAL_MODULE			:= eepp

LOCAL_CFLAGS			:= $(MY_C_FLAGS)

CODE_SRCS				:=  \
	helper/SOIL2/src/SOIL2/*.c \
	helper/stb_vorbis/*.c \
	helper/zlib/*.c \
	helper/libzip/*.c \
	helper/jpeg-compressor/*.cpp \
	helper/imageresampler/*.cpp \
	helper/haikuttf/*.cpp \
	helper/pugixml/*.cpp \
	system/*.cpp \
	system/platform/posix/*.cpp \
	network/*.cpp \
	network/ssl/*.cpp \
	network/ssl/backend/openssl/*.cpp \
	network/platform/unix/*.cpp \
	core/*.cpp \
	math/*.cpp \
	audio/*.cpp \
	window/*.cpp \
	window/backend/SDL2/*.cpp \
	window/platform/null/*.cpp \
	graphics/*.cpp \
	graphics/renderer/*.cpp \
	physics/*.cpp \
	physics/constraints/*.cpp \
	ui/*.cpp \
	ui/tools/*.cpp \
	maps/*.cpp \
	maps/mapeditor/*.cpp

LOCAL_C_INCLUDES		:= $(MY_C_INCLUDES)

LOCAL_SRC_FILES			:= $(foreach F, $(CODE_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_STATIC_LIBRARIES	:= openal SDL2 chipmunk freetype

include $(BUILD_STATIC_LIBRARY) 
#*************** EEPP ***************

#*************** CHIPMUNK ***************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)

LOCAL_MODULE			:= chipmunk

LOCAL_CFLAGS			:= \
	-DANDROID_NDK \
	-DDISABLE_IMPORTGL \
	-std=gnu99 \
	-Wall \
	-Wno-unknown-pragmas

CHIPMUNK_SRCS			:=  \
	helper/chipmunk/*.c \
	helper/chipmunk/constraints/*.c

LOCAL_C_INCLUDES		:= $(MY_C_INCLUDES)

LOCAL_SRC_FILES			:= $(foreach F, $(CHIPMUNK_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

include $(BUILD_STATIC_LIBRARY)
#*************** CHIPMUNK ***************

#*************** FREETYPE ***************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)/helper/freetype2

LOCAL_MODULE			:= freetype

# APP_SUBDIRS := $(patsubst $(LOCAL_PATH)/%, %, $(shell find $(LOCAL_PATH)/src -type d))

# LOCAL_C_INCLUDES := $(foreach D, $(APP_SUBDIRS), $(LOCAL_PATH)/$(D)) $(LOCAL_PATH)/include
# LOCAL_CFLAGS := -Os -DFT2_BUILD_LIBRARY

# LOCAL_SRC_FILES += $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.c))))

# walk through dir and subdir
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALLFILES = $(call walk, $(LOCAL_PATH)/src)
FILE_LIST += $(filter %.c, $(ALLFILES))

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Os -DFT2_BUILD_LIBRARY

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)
#*************** FREETYPE ***************

#*************** OPENAL *****************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)/helper/openal-soft

LOCAL_MODULE			:= openal

LOCAL_CFLAGS			:= -O3 -DHAVE_CONFIG_H -DAL_ALEXT_PROTOTYPES -DHAVE_OPENSL

LOCAL_C_INCLUDES		:= $(LOCAL_PATH)/ $(LOCAL_PATH)/include $(LOCAL_PATH)/OpenAL32/Include

LOCAL_SRC_FILES			:= \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/OpenAL32/*.c) \
	$(wildcard $(LOCAL_PATH)/Alc/AL*.c) \
	$(wildcard $(LOCAL_PATH)/Alc/alc*.c) \
	$(LOCAL_PATH)/Alc/bs2b.c \
	$(LOCAL_PATH)/Alc/helpers.c \
	$(LOCAL_PATH)/Alc/hrtf.c \
	$(LOCAL_PATH)/Alc/mixer.c \
	$(LOCAL_PATH)/Alc/mixer_c.c \
	$(LOCAL_PATH)/Alc/panning.c \
	$(LOCAL_PATH)/Alc/backends/opensl.c \
	$(LOCAL_PATH)/Alc/backends/loopback.c \
	$(LOCAL_PATH)/Alc/backends/wave.c \
	$(LOCAL_PATH)/Alc/backends/null.c \
	$(wildcard $(LOCAL_PATH)/src/video/android/*.c))

LOCAL_LDLIBS			:= -llog -lOpenSLES

include $(BUILD_SHARED_LIBRARY)
#*************** OPENAL *****************

#**************** SDL 2 ***************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_SDL_PATH)

LOCAL_MODULE			:= SDL2

LOCAL_C_INCLUDES		:= $(LOCAL_PATH)/include

LOCAL_CFLAGS			:= -D__ANDROID__ -DANDROID -DGL_GLEXT_PROTOTYPES $(EE_GLES_VERSION)

LOCAL_SRC_FILES			:= \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/audio/dummy/*.c) \
	$(LOCAL_PATH)/src/atomic/SDL_atomic.c \
	$(LOCAL_PATH)/src/atomic/SDL_spinlock.c.arm \
	$(wildcard $(LOCAL_PATH)/src/core/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/cpuinfo/*.c) \
	$(wildcard $(LOCAL_PATH)/src/dynapi/*.c) \
	$(wildcard $(LOCAL_PATH)/src/events/*.c) \
	$(wildcard $(LOCAL_PATH)/src/file/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/*.c) \
	$(wildcard $(LOCAL_PATH)/src/haptic/dummy/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/*.c) \
	$(wildcard $(LOCAL_PATH)/src/joystick/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/loadso/dlopen/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/*.c) \
	$(wildcard $(LOCAL_PATH)/src/power/android/*.c) \
	$(wildcard $(LOCAL_PATH)/src/filesystem/dummy/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*.c) \
	$(wildcard $(LOCAL_PATH)/src/render/*/*.c) \
	$(wildcard $(LOCAL_PATH)/src/stdlib/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/thread/pthread/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/*.c) \
	$(wildcard $(LOCAL_PATH)/src/timer/unix/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/*.c) \
	$(wildcard $(LOCAL_PATH)/src/video/android/*.c) \
    $(wildcard $(LOCAL_PATH)/src/test/*.c))

include $(BUILD_STATIC_LIBRARY)
#**************** SDL 2 ***************

#************* empty_window *************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)

LOCAL_MODULE			:= empty_window

LOCAL_LDLIBS			:= $(MY_LDLIBS)

LOCAL_CFLAGS			:= $(MY_C_FLAGS)

LOCAL_C_INCLUDES		:= $(MY_C_INCLUDES)

CORE_SRCS				:=  \
	$(MY_SDL_MAIN_PATH) \
	../examples/empty_window/*.cpp

LOCAL_SRC_FILES			:= $(foreach F, $(CORE_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_STATIC_LIBRARIES	:= eepp

include $(BUILD_SHARED_LIBRARY)
#************ empty_window ************

#************* external_shader *************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)

LOCAL_MODULE			:= external_shader

LOCAL_LDLIBS			:= $(MY_LDLIBS)

LOCAL_CFLAGS			:= $(MY_C_FLAGS)

LOCAL_C_INCLUDES		:= $(MY_C_INCLUDES)

CORE_SRCS				:=  \
	$(MY_SDL_MAIN_PATH) \
	../examples/external_shader/*.cpp

LOCAL_SRC_FILES			:= $(foreach F, $(CORE_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_STATIC_LIBRARIES	:= eepp

include $(BUILD_SHARED_LIBRARY)
#************ external_shader ************

#************* full_test *************
include $(CLEAR_VARS)

LOCAL_PATH				:= $(MY_PATH)

LOCAL_MODULE			:= main

LOCAL_LDLIBS			:= $(MY_LDLIBS)

LOCAL_CFLAGS			:= $(MY_C_FLAGS)

LOCAL_C_INCLUDES		:= $(MY_C_INCLUDES)

CORE_SRCS				:=  \
	$(MY_SDL_MAIN_PATH) \
	../test/*.cpp

LOCAL_SRC_FILES			:= $(foreach F, $(CORE_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_STATIC_LIBRARIES	:= eepp

include $(BUILD_SHARED_LIBRARY)
#************ full_test ************
