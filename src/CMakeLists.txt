# REFERENCE:
# http://qt-quarterly.developpez.com/qq-34/cmake/

# External resources
# ... Qt4
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# ... GDAL
find_package( GDAL REQUIRED )
# ... GPSD
find_package( GPSD REQUIRED )

# Include files
include_directories(
  ${QT_QTXML_INCLUDE_DIR}
  ${GDAL_INCLUDE_DIRS}
  ${GPSD_INCLUDE_DIRS}
  .
)

# Source files (*.cpp)
set( MY_CPPS
  main.cpp
  QVCTRuntime.cpp
  CMainWindow.cpp
  CTimeView.cpp
  charts/CChart.cpp
  charts/CChartGDAL.cpp
  charts/CChartControl.cpp
  charts/CChartTable.cpp
  data/CDataCourse.cpp
  data/CDataCourseValidity.cpp
  data/CDataPosition.cpp
  data/CDataPositionValidity.cpp
  data/CDataTime.cpp
  data/CDataTimeValidity.cpp
  devices/CDevice.cpp
  devices/CDeviceCreateView.cpp
  devices/CDeviceDetailView.cpp
  devices/data/CDeviceDataDop.cpp
  devices/data/CDeviceDataFix.cpp
  devices/data/CDeviceDataSatellite.cpp
  devices/data/CDeviceDataSkyView.cpp
  devices/data/CDeviceDataSource.cpp
  devices/drivers/CDeviceDriver.cpp
  devices/drivers/GpsdGps/CDeviceGpsdGps.cpp
  devices/drivers/GpsdGps/CDeviceGpsdGpsEditView.cpp
  devices/drivers/GpsdAis/CDeviceGpsdAis.cpp
  devices/drivers/GpsdAis/CDeviceGpsdAisEditView.cpp
  # devices/drivers/Sample/CDeviceSample.cpp # SAMPLE_DRIVER
  # devices/drivers/Sample/CDeviceSampleEditView.cpp # SAMPLE_DRIVER
  overlays/COverlay.cpp
  overlays/COverlayBase.cpp
  overlays/COverlayBaseTree.cpp
  overlays/COverlayContainer.cpp
  overlays/COverlayCourse.cpp
  overlays/COverlayItem.cpp
  overlays/COverlayObject.cpp
  overlays/COverlayObjectDetailView.cpp
  overlays/COverlayObjectEditView.cpp
  overlays/COverlayObjectPickView.cpp
  overlays/COverlayPoint.cpp
  overlays/COverlayText.cpp
  overlays/COverlayUrl.cpp
  overlays/COverlayVisibility.cpp
  overlays/device/CDeviceOverlay.cpp
  overlays/device/CDeviceOverlayListView.cpp
  overlays/dock/COverlayDetailView.cpp
  overlays/dock/COverlayListView.cpp
  overlays/landmark/CLandmarkContainer.cpp
  overlays/landmark/CLandmarkContainerDetailView.cpp
  overlays/landmark/CLandmarkContainerEditView.cpp
  overlays/landmark/CLandmarkContainerPickView.cpp
  overlays/landmark/CLandmarkOverlay.cpp
  overlays/landmark/CLandmarkOverlayListView.cpp
  overlays/landmark/CLandmarkPoint.cpp
  overlays/landmark/CLandmarkPointDetailView.cpp
  overlays/landmark/CLandmarkPointEditView.cpp
  overlays/pointer/CPointerOverlay.cpp
  overlays/pointer/CPointerPoint.cpp
  overlays/pointer/CPointerPointDetailView.cpp
  overlays/route/CRouteContainer.cpp
  overlays/route/CRouteContainerDetailView.cpp
  overlays/route/CRouteContainerEditView.cpp
  overlays/route/CRouteContainerPickView.cpp
  overlays/route/CRouteOverlay.cpp
  overlays/route/CRouteOverlayListView.cpp
  overlays/route/CRoutePoint.cpp
  overlays/route/CRoutePointDetailView.cpp
  overlays/route/CRoutePointEditView.cpp
  overlays/track/CTrackContainer.cpp
  overlays/track/CTrackContainerDetailView.cpp
  overlays/track/CTrackContainerEditView.cpp
  overlays/track/CTrackOverlay.cpp
  overlays/track/CTrackOverlayListView.cpp
  overlays/track/CTrackPoint.cpp
  overlays/track/CTrackPointDetailView.cpp
  overlays/track/CTrackSubContainer.cpp
  overlays/track/CTrackSubContainerDetailView.cpp
  overlays/vessel/CVesselContainer.cpp
  overlays/vessel/CVesselContainerDetailView.cpp
  overlays/vessel/CVesselContainerEditView.cpp
  overlays/vessel/CVesselCourseDockView.cpp
  overlays/vessel/CVesselOverlay.cpp
  overlays/vessel/CVesselOverlayListView.cpp
  overlays/vessel/CVesselPoint.cpp
  overlays/vessel/CVesselPointDetailView.cpp
  overlays/vessel/CVesselPointEditView.cpp
  overlays/vessel/CVesselPositionDockView.cpp
  overlays/vessel/CVesselTargetDockView.cpp
  overlays/vessel/device/CVesselContainerDevice.cpp
  overlays/vessel/device/CVesselContainerDeviceCreateView.cpp
  overlays/vessel/device/CVesselContainerDeviceDetailView.cpp
  overlays/vessel/device/CVesselPointDevice.cpp
  overlays/vessel/device/CVesselPointDeviceCreateView.cpp
  overlays/vessel/device/CVesselPointDeviceDetailView.cpp
  overlays/vessel/device/CVesselPointDeviceEditView.cpp
  settings/CSettings.cpp
  settings/CSettingsEditView.cpp
  settings/CSettingsMiscEditView.cpp
  settings/CSettingsUnitsEditView.cpp
  settings/CSettingsValidityEditView.cpp
  units/CUnitBearing.cpp
  units/CUnitDate.cpp
  units/CUnitDistance.cpp
  units/CUnitElevation.cpp
  units/CUnitPosition.cpp
  units/CUnitSpeed.cpp
  units/CUnitSpeedVertical.cpp
  units/CUnitTime.cpp
  units/CUnitTimeDelta.cpp
  units/CUnitTimeZone.cpp
)

# MOC-related header files (*.hpp)
set( MY_MOCS
  CMainWindow.hpp
  CTimeView.hpp
  charts/CChart.hpp
  charts/CChartControl.hpp
  charts/CChartTable.hpp
  devices/CDevice.hpp
  devices/CDeviceCreateView.hpp
  devices/CDeviceDetailView.hpp
  devices/drivers/GpsdGps/CDeviceGpsdGps.hpp
  devices/drivers/GpsdGps/CDeviceGpsdGpsEditView.hpp
  devices/drivers/GpsdAis/CDeviceGpsdAis.hpp
  devices/drivers/GpsdAis/CDeviceGpsdAisEditView.hpp
  # devices/drivers/Sample/CDeviceSample.hpp # SAMPLE_DRIVER
  # devices/drivers/Sample/CDeviceSampleEditView.hpp # SAMPLE_DRIVER
  overlays/COverlayBase.hpp
  overlays/COverlayBaseTree.hpp
  overlays/COverlayObject.hpp
  overlays/COverlayObjectDetailView.hpp
  overlays/COverlayObjectEditView.hpp
  overlays/COverlayObjectPickView.hpp
  overlays/device/CDeviceOverlayListView.hpp
  overlays/dock/COverlayDetailView.hpp
  overlays/dock/COverlayListView.hpp
  overlays/landmark/CLandmarkContainerDetailView.hpp
  overlays/landmark/CLandmarkOverlayListView.hpp
  overlays/landmark/CLandmarkPointDetailView.hpp
  overlays/pointer/CPointerPointDetailView.hpp
  overlays/route/CRouteContainerDetailView.hpp
  overlays/route/CRouteOverlayListView.hpp
  overlays/route/CRoutePointDetailView.hpp
  overlays/track/CTrackContainerDetailView.hpp
  overlays/track/CTrackOverlayListView.hpp
  overlays/track/CTrackPointDetailView.hpp
  overlays/track/CTrackSubContainerDetailView.hpp
  overlays/vessel/CVesselContainerDetailView.hpp
  overlays/vessel/CVesselCourseDockView.hpp
  overlays/vessel/CVesselOverlayListView.hpp
  overlays/vessel/CVesselPointDetailView.hpp
  overlays/vessel/CVesselPositionDockView.hpp
  overlays/vessel/CVesselTargetDockView.hpp
  overlays/vessel/device/CVesselContainerDevice.hpp
  overlays/vessel/device/CVesselContainerDeviceCreateView.hpp
  overlays/vessel/device/CVesselContainerDeviceDetailView.hpp
  overlays/vessel/device/CVesselPointDevice.hpp
  overlays/vessel/device/CVesselPointDeviceCreateView.hpp
  overlays/vessel/device/CVesselPointDeviceDetailView.hpp
  overlays/vessel/device/CVesselPointDeviceEditView.hpp
  settings/CSettings.hpp
  settings/CSettingsEditView.hpp
  settings/CSettingsMiscEditView.hpp
  settings/CSettingsUnitsEditView.hpp
  settings/CSettingsValidityEditView.hpp
)

# User-interface files (*.ui)
#set( MY_UIS
#)

# Resource files (*.rc)
set( MY_RCS
  resources/cursors.qrc
  resources/icons-16x16.qrc
  resources/icons-32x32.qrc
)

# Qt4 resources
qt4_wrap_cpp( MY_QT_MOCS ${MY_MOCS} )
#qt4_wrap_ui( MY_QT_UIS ${MY_UIS})
qt4_add_resources( MY_QT_RCS ${MY_RCS})

# Executable
set( MY_DEPS
  ${MY_CPPS}
  ${MY_MOCS}
  ${MY_QT_MOCS}
  ${MY_QT_RCS}
)
set( EXECUTABLE_OUTPUT_PATH
  ${CMAKE_BINARY_DIR}/bin
)
add_executable( ${PROGNAME}
  ${MY_DEPS}
)

# Libraries
target_link_libraries( ${PROGNAME}
  ${QT_LIBRARIES}
  ${QT_QTXML_LIBRARIES}
  ${GDAL_LIBRARIES}
  ${GPSD_LIBRARIES}
)

# Install
install(
  TARGETS
    ${PROGNAME}
  DESTINATION
    ${BIN_INSTALL_DIR}
)
install(
  FILES
    resources/qvct.desktop
  DESTINATION
    ${XDG_APPS_DIR}
)
install(
  FILES
    resources/icons/32x32/qvct.png
  DESTINATION
    ${DATA_INSTALL_PREFIX}/pixmaps
)

# Documentation
find_package( Doxygen )
if( DOXYGEN_FOUND )
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/doc/devel/doxygen.conf.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONLY)
  file( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/devel )
  file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/doc/images
    DESTINATION ${CMAKE_BINARY_DIR}/doc
    FILES_MATCHING PATTERN *.jpg )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc/devel
    COMMENT "Generating documentation with Doxygen" VERBATIM
    )
  install(
    DIRECTORY
      ${CMAKE_BINARY_DIR}/doc/devel
    DESTINATION
      ${DATA_INSTALL_PREFIX}/doc/qvct
  )
endif( DOXYGEN_FOUND )
install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/user
  DESTINATION
    ${DATA_INSTALL_PREFIX}/doc/qvct
)
install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/32x32/
  DESTINATION
    ${DATA_INSTALL_PREFIX}/doc/qvct/user/html/images/icons
  FILES_MATCHING PATTERN "*.png"
)
