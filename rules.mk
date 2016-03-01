LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

# When this module is included, find its public headers here
GLOBAL_INCLUDES += $(LOCAL_DIR)/include/core

GBSKIA_CPPFLAGS := -std=c++11 -DSK_BUILD_FOR_LK

##################################################################

MODULE_DEPS += \
	lib/libm

MODULE_INCLUDES += \
	$(LOCAL_DIR)/include/core \
	$(LOCAL_DIR)/include/images \
	$(LOCAL_DIR)/src/core 	\

MODULE_DEFINES += \
	SK_BUILD_FOR_LK			\
	SK_FEATURE_CONFIG_BW	\

MODULE_CPPFLAGS += -std=c++11

MODULE_SRCS += \
	$(LOCAL_DIR)/src/core/Sk64.cpp	\
	$(LOCAL_DIR)/src/core/SkAlphaRuns.cpp	\
	$(LOCAL_DIR)/src/core/SkBitmap.cpp	\
	$(LOCAL_DIR)/src/core/SkBitmapProcShader.cpp	\
	$(LOCAL_DIR)/src/core/SkBitmapProcState.cpp	\
	$(LOCAL_DIR)/src/core/SkBitmapProcState_matrixProcs.cpp	\
	$(LOCAL_DIR)/src/core/SkBitmapSampler.cpp	\
	$(LOCAL_DIR)/src/core/SkBlitter.cpp	\
	$(LOCAL_DIR)/src/core/SkBlitter_A1.cpp	\
	$(LOCAL_DIR)/src/core/SkBlitter_BW.cpp	\
	$(LOCAL_DIR)/src/core/SkBlitter_Sprite.cpp	\
	$(LOCAL_DIR)/src/core/SkBuffer.cpp	\
	$(LOCAL_DIR)/src/core/SkCanvas.cpp	\
	$(LOCAL_DIR)/src/core/SkChunkAlloc.cpp	\
	$(LOCAL_DIR)/src/core/SkColor.cpp	\
	$(LOCAL_DIR)/src/core/SkColorFilter.cpp	\
	$(LOCAL_DIR)/src/core/SkColorTable.cpp	\
	$(LOCAL_DIR)/src/core/SkComposeShader.cpp	\
	$(LOCAL_DIR)/src/core/SkCordic.cpp	\
	$(LOCAL_DIR)/src/core/SkDebug.cpp	\
	$(LOCAL_DIR)/src/core/SkDeque.cpp	\
	$(LOCAL_DIR)/src/core/SkDevice.cpp	\
	$(LOCAL_DIR)/src/core/SkDither.cpp	\
	$(LOCAL_DIR)/src/core/SkDraw.cpp	\
	$(LOCAL_DIR)/src/core/SkEdge.cpp	\
	$(LOCAL_DIR)/src/core/SkEdgeBuilder.cpp	\
	$(LOCAL_DIR)/src/core/SkEdgeClipper.cpp	\
	$(LOCAL_DIR)/src/core/SkFilterProc.cpp	\
	$(LOCAL_DIR)/src/core/SkFlattenable.cpp	\
	$(LOCAL_DIR)/src/core/SkFloatBits.cpp	\
	$(LOCAL_DIR)/src/core/SkGeometry.cpp	\
	$(LOCAL_DIR)/src/core/SkGlobals.cpp	\
	$(LOCAL_DIR)/src/core/SkGraphics.cpp	\
	$(LOCAL_DIR)/src/core/SkLineClipper.cpp	\
	$(LOCAL_DIR)/src/core/SkMask.cpp	\
	$(LOCAL_DIR)/src/core/SkMaskFilter.cpp	\
	$(LOCAL_DIR)/src/core/SkMath.cpp	\
	$(LOCAL_DIR)/src/core/SkMatrix.cpp	\
	$(LOCAL_DIR)/src/core/SkMemory_stdlib.cpp	\
	$(LOCAL_DIR)/src/core/SkPaint.cpp	\
	$(LOCAL_DIR)/src/core/SkPath.cpp	\
	$(LOCAL_DIR)/src/core/SkPathEffect.cpp	\
	$(LOCAL_DIR)/src/core/SkPathHeap.cpp	\
	$(LOCAL_DIR)/src/core/SkPathMeasure.cpp	\
	$(LOCAL_DIR)/src/core/SkPixelRef.cpp	\
	$(LOCAL_DIR)/src/core/SkPoint.cpp	\
	$(LOCAL_DIR)/src/core/SkProcSpriteBlitter.cpp	\
	$(LOCAL_DIR)/src/core/SkQuadClipper.cpp	\
	$(LOCAL_DIR)/src/core/SkRasterizer.cpp	\
	$(LOCAL_DIR)/src/core/SkRect.cpp	\
	$(LOCAL_DIR)/src/core/SkRefCnt.cpp	\
	$(LOCAL_DIR)/src/core/SkRegion.cpp	\
	$(LOCAL_DIR)/src/core/SkScan.cpp	\
	$(LOCAL_DIR)/src/core/SkScan_Antihair.cpp	\
	$(LOCAL_DIR)/src/core/SkScan_AntiPath.cpp	\
	$(LOCAL_DIR)/src/core/SkScan_Hairline.cpp	\
	$(LOCAL_DIR)/src/core/SkScan_Path.cpp	\
	$(LOCAL_DIR)/src/core/SkShader.cpp	\
	$(LOCAL_DIR)/src/core/SkStroke.cpp	\
	$(LOCAL_DIR)/src/core/SkStrokerPriv.cpp	\
	$(LOCAL_DIR)/src/core/SkTSearch.cpp	\
	$(LOCAL_DIR)/src/core/SkUnPreMultiply.cpp	\
	$(LOCAL_DIR)/src/core/SkUtils.cpp	\
	$(LOCAL_DIR)/src/core/SkWriter32.cpp	\
	$(LOCAL_DIR)/src/core/SkXfermode.cpp	\
	$(LOCAL_DIR)/src/images/SkImageDecoder_Factory.cpp	\
	$(LOCAL_DIR)/src/images/SkImageEncoder_Factory.cpp	\
	$(LOCAL_DIR)/src/opts/SkBitmapProcState_opts_none.cpp	\
	$(LOCAL_DIR)/src/opts/SkBlitRow_opts_none.cpp	\
	$(LOCAL_DIR)/src/ports/SkDebug_stdio.cpp	\
	$(LOCAL_DIR)/src/ports/SkGlobals_global.cpp	\
	$(LOCAL_DIR)/src/ports/SkThread_lk.cpp	\

include make/module.mk
