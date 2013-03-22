QT       -= core gui
TEMPLATE = app

CONFIG += console warn_on

# Use common project definitions.
include(../../../settings.pro)
include(../../../common.pro)

debug {
    DEFINES += DRWG_DBG
    QT       += core
}

# the library to test
LIBS += -ldxfrw

# google test library
LIBS += -lgtest_main -lgtest

SOURCES += \
    tests/dwgBuffer_test.cpp \
    tests/DRW_Xline_test.cpp \
    tests/DRW_Viewport_test.cpp \
    tests/DRW_Vertex_test.cpp \
    tests/DRW_Unknown_test.cpp \
    tests/DRW_Trace_test.cpp \
    tests/DRW_Text_test.cpp \
    tests/DRW_Spline_test.cpp \
    tests/DRW_Solid_test.cpp \
    tests/DRW_Ray_test.cpp \
    tests/DRW_Polyline_test.cpp \
    tests/DRW_Point_test.cpp \
    tests/DRW_Overlaybox_test.cpp \
    tests/DRW_Mtext_test.cpp \
    tests/DRW_LwPolyline_test.cpp \
    tests/DRW_Line_test.cpp \
    tests/DRW_Leader_test.cpp \
    tests/DRW_Insert_test.cpp \
    tests/DRW_Image_test.cpp \
    tests/DRW_Hatch_test.cpp \
    tests/DRW_Ellipse_test.cpp \
    tests/DRW_E3DFace_test.cpp \
    tests/DRW_DimRadial_test.cpp \
    tests/DRW_DimOrdinate_test.cpp \
    tests/DRW_DimLinear_test.cpp \
    tests/DRW_Dimension_test.cpp \
    tests/DRW_DimDiametric_test.cpp \
    tests/DRW_DimAngular_test.cpp \
    tests/DRW_DimAngular3P_test.cpp \
    tests/DRW_DimAligned_test.cpp \
    tests/DRW_ConstructionLine_test.cpp \
    tests/DRW_Circle_test.cpp \
    tests/DRW_Block_test.cpp \
    tests/DRW_Arc_test.cpp \
    tests/bitsbuild_test.cpp

HEADERS += \
    tests/bitsbuild.h
