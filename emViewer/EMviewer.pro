#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T22:18:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EMviewer
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        emviewer.cpp \
    emfile.cpp \
    tools.cpp \
    object.cpp

HEADERS  += emviewer.h \
    emfile.h \
    tools.h \
    object.h

FORMS    += emviewer.ui \
    tools.ui

RESOURCES += \
    resource.qrc

INCLUDEPATH += C:/VTK/include/vtk-6.3
LIBS += -LC:/VTK/lib/ \
-llibvtkalglib-6.3d \
-llibvtkChartsCore-6.3d \
-llibvtkCommonColor-6.3d \
-llibvtkCommonComputationalGeometry-6.3d \
-llibvtkCommonCore-6.3d \
-llibvtkCommonDataModel-6.3d \
-llibvtkCommonExecutionModel-6.3d \
-llibvtkCommonMath-6.3d \
-llibvtkCommonMisc-6.3d \
-llibvtkCommonSystem-6.3d \
-llibvtkCommonTransforms-6.3d \
-llibvtkDICOMParser-6.3d \
-llibvtkDomainsChemistry-6.3d \
-llibvtkexoIIc-6.3d \
-llibvtkexpat-6.3d \
-llibvtkFiltersAMR-6.3d \
-llibvtkFiltersCore-6.3d \
-llibvtkFiltersExtraction-6.3d \
-llibvtkFiltersFlowPaths-6.3d \
-llibvtkFiltersGeneral-6.3d \
-llibvtkFiltersGeneric-6.3d \
-llibvtkFiltersGeometry-6.3d \
-llibvtkFiltersHybrid-6.3d \
-llibvtkFiltersHyperTree-6.3d \
-llibvtkFiltersImaging-6.3d \
-llibvtkFiltersModeling-6.3d \
-llibvtkFiltersParallel-6.3d \
-llibvtkFiltersParallelImaging-6.3d \
-llibvtkFiltersProgrammable-6.3d \
-llibvtkFiltersSelection-6.3d \
-llibvtkFiltersSMP-6.3d \
-llibvtkFiltersSources-6.3d \
-llibvtkFiltersStatistics-6.3d \
-llibvtkFiltersTexture-6.3d \
-llibvtkFiltersVerdict-6.3d \
-llibvtkfreetype-6.3d \
-llibvtkftgl-6.3d \
-llibvtkGeovisCore-6.3d \
-llibvtkgl2ps-6.3d \
-llibvtkGUISupportQt-6.3d \
-llibvtkGUISupportQtOpenGL-6.3d \
-llibvtkGUISupportQtSQL-6.3d \
-llibvtkGUISupportQtWebkit-6.3d \
-llibvtkImagingColor-6.3d \
-llibvtkImagingCore-6.3d \
-llibvtkImagingFourier-6.3d \
-llibvtkImagingGeneral-6.3d \
-llibvtkImagingHybrid-6.3d \
-llibvtkImagingMath-6.3d \
-llibvtkImagingMorphological-6.3d \
-llibvtkImagingSources-6.3d \
-llibvtkImagingStatistics-6.3d \
-llibvtkImagingStencil-6.3d \
-llibvtkInfovisCore-6.3d \
-llibvtkInfovisLayout-6.3d \
-llibvtkInteractionImage-6.3d \
-llibvtkInteractionStyle-6.3d \
-llibvtkInteractionWidgets-6.3d \
-llibvtkIOAMR-6.3d \
-llibvtkIOCore-6.3d \
-llibvtkIOEnSight-6.3d \
-llibvtkIOExodus-6.3d \
-llibvtkIOExport-6.3d \
-llibvtkIOGeometry-6.3d \
-llibvtkIOImage-6.3d \
-llibvtkIOImport-6.3d \
-llibvtkIOInfovis-6.3d \
-llibvtkIOLegacy-6.3d \
-llibvtkIOLSDyna-6.3d \
-llibvtkIOMINC-6.3d \
-llibvtkIOMovie-6.3d \
-llibvtkIONetCDF-6.3d \
-llibvtkIOParallel-6.3d \
-llibvtkIOParallelXML-6.3d \
-llibvtkIOPLY-6.3d \
-llibvtkIOSQL-6.3d \
-llibvtkIOVideo-6.3d \
-llibvtkIOXML-6.3d \
-llibvtkIOXMLParser-6.3d \
-llibvtkjpeg-6.3d \
-llibvtkjsoncpp-6.3d \
-llibvtklibxml2-6.3d \
-llibvtkmetaio-6.3d \
-llibvtkNetCDF_cxx-6.3d \
-llibvtkNetCDF-6.3d \
-llibvtkoggtheora-6.3d \
-llibvtkParallelCore-6.3d \
-llibvtkpng-6.3d \
-llibvtkproj4-6.3d \
-llibvtkRenderingAnnotation-6.3d \
-llibvtkRenderingContext2D-6.3d \
-llibvtkRenderingContextOpenGL-6.3d \
-llibvtkRenderingCore-6.3d \
-llibvtkRenderingFreeType-6.3d \
-llibvtkRenderingGL2PS-6.3d \
-llibvtkRenderingImage-6.3d \
-llibvtkRenderingLabel-6.3d \
-llibvtkRenderingLIC-6.3d \
-llibvtkRenderingLOD-6.3d \
-llibvtkRenderingOpenGL-6.3d \
-llibvtkRenderingQt-6.3d \
-llibvtkRenderingVolume-6.3d \
-llibvtkRenderingVolumeOpenGL-6.3d \
-llibvtksys-6.3d \
-llibvtktiff-6.3d \
-llibvtkverdict-6.3d \
-llibvtkViewsContext2D-6.3d \
-llibvtkViewsCore-6.3d \
-llibvtkViewsInfovis-6.3d \
-llibvtkViewsQt-6.3d \
-llibvtkzlib-6.3d \

