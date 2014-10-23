LOCAL_PATH			:= $(call my-dir)
JSON_DIR			:= $(LOCAL_PATH)/json_parser
JSON_SRCS			:= $(wildcard $(LOCAL_PATH)/json_parser/src/*.c)
ABSTRACTION_DIR		:= $(LOCAL_PATH)/abstraction_layer
ABSTRACTION_SRCS	:= $(wildcard $(LOCAL_PATH)/abstraction_layer/src/*.c)

include $(CLEAR_VARS)

LOCAL_MODULE		:= JSON_Parser
LOCAL_SRC_FILES		:= $(JSON_SRCS:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES	:= $(JSON_DIR)/inc
LOCAL_LDLIBS		:= -llog

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE		:= HealthCare
LOCAL_SRC_FILES		:= $(ABSTRACTION_SRCS:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES	:= $(ABSTRACTION_DIR)/inc
LOCAL_LDLIBS		:= -llog

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)