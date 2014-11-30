LOCAL_PATH			:= $(call my-dir)
SRCS				:= $(wildcard $(LOCAL_PATH)/src/*.c)

include $(CLEAR_VARS)

LOCAL_MODULE		:= HealthCare
LOCAL_SRC_FILES		:= $(SRCS:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/inc
LOCAL_LDLIBS		:= -llog

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)