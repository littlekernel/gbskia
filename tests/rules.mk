LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

# When this module is included, find its public headers here
GLOBAL_INCLUDES += $(LOCAL_DIR)

MODULE_DEPS += \
	external/gbskia

MODULE_DEFINES +=  \
    SK_BUILD_FOR_LK \

LIBC_REDEFINES += \
	-Datof=sk_test_atof	\
	-Dsscanf=sk_test_sscanf \
	-Dvsscanf=sk_test_vsscanf	\

MODULE_CPPFLAGS += -std=c++11 $(LIBC_REDEFINES)
MODULE_CFLAGS += $(LIBC_REDEFINES)

MODULE_SRCS += \
	$(LOCAL_DIR)/svg_test.cpp \
	$(LOCAL_DIR)/utils/atof.c	\
	$(LOCAL_DIR)/utils/scanf.c 	\

include make/module.mk
