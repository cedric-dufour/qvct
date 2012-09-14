// INDENTING (emacs/vi): -*- mode:c++; tab-width:2; c-basic-offset:2; intent-tabs-mode:nil; -*- ex: set tabstop=2 expandtab:

/*
 * Qt Virtual Chart Table (QVCT)
 * Copyright (C) 2012 Cedric Dufour <http://cedric.dufour.name>
 * Author: Cedric Dufour <http://cedric.dufour.name>
 *
 * The Qt Virtual Chart Table (QVCT) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, Version 3.
 *
 * The Qt Virtual Chart Table (QVCT) is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 */

/**
 * \brief      Runtime application handler
 * \author     Cedric Dufour <http://cedric.dufour.name>
 * \version    @version@
 */


#ifndef QVCT_QVCTRUNTIME_HPP
#define QVCT_QVCTRUNTIME_HPP

// QT
#include <QMutex>
#include <QTimer>

// QVCT
#include "main.hpp"
#include "CMainWindow.hpp"
#include "CTimeView.hpp"
#include "charts/CChartTable.hpp"
#include "charts/CChartControl.hpp"
#include "devices/CDeviceDetailView.hpp"
#include "overlays/device/CDeviceOverlay.hpp"
#include "overlays/device/CDeviceOverlayListView.hpp"
#include "overlays/dock/COverlayDetailView.hpp"
#include "overlays/dock/COverlayListView.hpp"
#include "overlays/pointer/CPointerOverlay.hpp"
#include "overlays/pointer/CPointerPointDetailView.hpp"
#include "overlays/landmark/CLandmarkOverlay.hpp"
#include "overlays/landmark/CLandmarkOverlayListView.hpp"
#include "overlays/landmark/CLandmarkContainerDetailView.hpp"
#include "overlays/landmark/CLandmarkPointDetailView.hpp"
#include "overlays/route/CRouteOverlay.hpp"
#include "overlays/route/CRouteOverlayListView.hpp"
#include "overlays/route/CRouteContainerDetailView.hpp"
#include "overlays/route/CRoutePointDetailView.hpp"
#include "overlays/track/CTrackOverlay.hpp"
#include "overlays/track/CTrackOverlayListView.hpp"
#include "overlays/track/CTrackContainerDetailView.hpp"
#include "overlays/track/CTrackSubContainerDetailView.hpp"
#include "overlays/track/CTrackPointDetailView.hpp"
#include "overlays/vessel/CVesselOverlay.hpp"
#include "overlays/vessel/CVesselOverlayListView.hpp"
#include "overlays/vessel/CVesselContainerDetailView.hpp"
#include "overlays/vessel/CVesselCourseDockView.hpp"
#include "overlays/vessel/CVesselPointDetailView.hpp"
#include "overlays/vessel/CVesselPositionDockView.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceDetailView.hpp"
#include "overlays/vessel/device/CVesselPointDeviceDetailView.hpp"
#include "settings/CSettings.hpp"


class QVCTRuntime
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

private:
  static QMutex* pqMutexDataChange;
  static QTimer* pqTimerRefresh;

private:
  static CSettings* poSettings;
  static CMainWindow* poMainWindow;
  static CTimeView* poTimeView;
  static CChartTable* poChartTable;
  static CChartControl* poChartControl;
  static CDeviceDetailView* poDeviceDetailView;
  static COverlayDetailView* poOverlayDetailView;
  static COverlayListView* poOverlayListView;
  static CDeviceOverlay* poDeviceOverlay;
  static CDeviceOverlayListView* poDeviceOverlayListView;
  static CPointerOverlay* poPointerOverlay;
  static CPointerPointDetailView* poPointerPointDetailView;
  static CLandmarkOverlay* poLandmarkOverlay;
  static CLandmarkOverlayListView* poLandmarkOverlayListView;
  static CLandmarkContainerDetailView* poLandmarkContainerDetailView;
  static CLandmarkPointDetailView* poLandmarkPointDetailView;
  static CRouteOverlay* poRouteOverlay;
  static CRouteOverlayListView* poRouteOverlayListView;
  static CRouteContainerDetailView* poRouteContainerDetailView;
  static CRoutePointDetailView* poRoutePointDetailView;
  static CTrackOverlay* poTrackOverlay;
  static CTrackOverlayListView* poTrackOverlayListView;
  static CTrackContainerDetailView* poTrackContainerDetailView;
  static CTrackSubContainerDetailView* poTrackSubContainerDetailView;
  static CTrackPointDetailView* poTrackPointDetailView;
  static CVesselOverlay* poVesselOverlay;
  static CVesselOverlayListView* poVesselOverlayListView;
  static CVesselContainerDetailView* poVesselContainerDetailView;
  static CVesselCourseDockView* poVesselCourseDockView;
  static CVesselPointDetailView* poVesselPointDetailView;
  static CVesselPositionDockView* poVesselPositionDockView;
  static CVesselContainerDeviceDetailView* poVesselContainerDeviceDetailView;
  static CVesselPointDeviceDetailView* poVesselPointDeviceDetailView;

public:
  static void registerSettings( CSettings* _poSettings ) { poSettings = _poSettings; };
  static void registerMainWindow( CMainWindow* _poMainWindow ) { poMainWindow = _poMainWindow; };
  static void registerTimeView( CTimeView* _poTimeView ) { poTimeView = _poTimeView; };
  static void registerChartTable( CChartTable* _poChartTable ) { poChartTable = _poChartTable; };
  static void registerChartControl( CChartControl* _poChartControl ) { poChartControl = _poChartControl; };
  static void registerDeviceDetailView( CDeviceDetailView* _poDeviceDetailView ) { poDeviceDetailView = _poDeviceDetailView; };
  static void registerOverlayDetailView( COverlayDetailView* _poOverlayDetailView ) { poOverlayDetailView = _poOverlayDetailView; };
  static void registerOverlayListView( COverlayListView* _poOverlayListView ) { poOverlayListView = _poOverlayListView; };
  static void registerDeviceOverlay( CDeviceOverlay* _poDeviceOverlay ) { poDeviceOverlay = _poDeviceOverlay; };
  static void registerDeviceOverlayListView( CDeviceOverlayListView* _poDeviceOverlayListView ) { poDeviceOverlayListView = _poDeviceOverlayListView; };
  static void registerPointerOverlay( CPointerOverlay* _poPointerOverlay ) { poPointerOverlay = _poPointerOverlay; };
  static void registerPointerPointDetailView( CPointerPointDetailView* _poPointerPointDetailView ) { poPointerPointDetailView = _poPointerPointDetailView; };
  static void registerLandmarkOverlay( CLandmarkOverlay* _poLandmarkOverlay ) { poLandmarkOverlay = _poLandmarkOverlay; };
  static void registerLandmarkOverlayListView( CLandmarkOverlayListView* _poLandmarkOverlayListView ) { poLandmarkOverlayListView = _poLandmarkOverlayListView; };
  static void registerLandmarkContainerDetailView( CLandmarkContainerDetailView* _poLandmarkContainerDetailView ) { poLandmarkContainerDetailView = _poLandmarkContainerDetailView; };
  static void registerLandmarkPointDetailView( CLandmarkPointDetailView* _poLandmarkPointDetailView ) { poLandmarkPointDetailView = _poLandmarkPointDetailView; };
  static void registerRouteOverlay( CRouteOverlay* _poRouteOverlay ) { poRouteOverlay = _poRouteOverlay; };
  static void registerRouteOverlayListView( CRouteOverlayListView* _poRouteOverlayListView ) { poRouteOverlayListView = _poRouteOverlayListView; };
  static void registerRouteContainerDetailView( CRouteContainerDetailView* _poRouteContainerDetailView ) { poRouteContainerDetailView = _poRouteContainerDetailView; };
  static void registerRoutePointDetailView( CRoutePointDetailView* _poRoutePointDetailView ) { poRoutePointDetailView = _poRoutePointDetailView; };
  static void registerTrackOverlay( CTrackOverlay* _poTrackOverlay ) { poTrackOverlay = _poTrackOverlay; };
  static void registerTrackOverlayListView( CTrackOverlayListView* _poTrackOverlayListView ) { poTrackOverlayListView = _poTrackOverlayListView; };
  static void registerTrackContainerDetailView( CTrackContainerDetailView* _poTrackContainerDetailView ) { poTrackContainerDetailView = _poTrackContainerDetailView; };
  static void registerTrackSubContainerDetailView( CTrackSubContainerDetailView* _poTrackSubContainerDetailView ) { poTrackSubContainerDetailView = _poTrackSubContainerDetailView; };
  static void registerTrackPointDetailView( CTrackPointDetailView* _poTrackPointDetailView ) { poTrackPointDetailView = _poTrackPointDetailView; };
  static void registerVesselOverlay( CVesselOverlay* _poVesselOverlay ) { poVesselOverlay = _poVesselOverlay; };
  static void registerVesselOverlayListView( CVesselOverlayListView* _poVesselOverlayListView ) { poVesselOverlayListView = _poVesselOverlayListView; };
  static void registerVesselContainerDetailView( CVesselContainerDetailView* _poVesselContainerDetailView ) { poVesselContainerDetailView = _poVesselContainerDetailView; };
  static void registerVesselCourseDockView( CVesselCourseDockView* _poVesselCourseDockView ) { poVesselCourseDockView = _poVesselCourseDockView; };
  static void registerVesselPointDetailView( CVesselPointDetailView* _poVesselPointDetailView ) { poVesselPointDetailView = _poVesselPointDetailView; };
  static void registerVesselPositionDockView( CVesselPositionDockView* _poVesselPositionDockView ) { poVesselPositionDockView = _poVesselPositionDockView; };
  static void registerVesselContainerDeviceDetailView( CVesselContainerDeviceDetailView* _poVesselContainerDeviceDetailView ) { poVesselContainerDeviceDetailView = _poVesselContainerDeviceDetailView; };
  static void registerVesselPointDeviceDetailView( CVesselPointDeviceDetailView* _poVesselPointDeviceDetailView ) { poVesselPointDeviceDetailView = _poVesselPointDeviceDetailView; };

public:
  static QMutex* useMutexDataChange() { return pqMutexDataChange; };
  static QTimer* useTimerRefresh() { return pqTimerRefresh; };

public:
  static CSettings* useSettings() { return poSettings; };
  static CMainWindow* useMainWindow() { return poMainWindow; };
  static CTimeView* useTimeView() { return poTimeView; };
  static CChartTable* useChartTable() { return poChartTable; };
  static CChartControl* useChartControl() { return poChartControl; };
  static CDeviceDetailView* useDeviceDetailView() { return poDeviceDetailView; };
  static COverlayDetailView* useOverlayDetailView() { return poOverlayDetailView; };
  static COverlayListView* useOverlayListView() { return poOverlayListView; };
  static CDeviceOverlay* useDeviceOverlay() { return poDeviceOverlay; };
  static CDeviceOverlayListView* useDeviceOverlayListView() { return poDeviceOverlayListView; };
  static CPointerOverlay* usePointerOverlay() { return poPointerOverlay; };
  static CPointerPointDetailView* usePointerPointDetailView() { return poPointerPointDetailView; };
  static CLandmarkOverlay* useLandmarkOverlay() { return poLandmarkOverlay; };
  static CLandmarkOverlayListView* useLandmarkOverlayListView() { return poLandmarkOverlayListView; };
  static CLandmarkContainerDetailView* useLandmarkContainerDetailView() { return poLandmarkContainerDetailView; };
  static CLandmarkPointDetailView* useLandmarkPointDetailView() { return poLandmarkPointDetailView; };
  static CRouteOverlay* useRouteOverlay() { return poRouteOverlay; };
  static CRouteOverlayListView* useRouteOverlayListView() { return poRouteOverlayListView; };
  static CRouteContainerDetailView* useRouteContainerDetailView() { return poRouteContainerDetailView; };
  static CRoutePointDetailView* useRoutePointDetailView() { return poRoutePointDetailView; };
  static CTrackOverlay* useTrackOverlay() { return poTrackOverlay; };
  static CTrackOverlayListView* useTrackOverlayListView() { return poTrackOverlayListView; };
  static CTrackContainerDetailView* useTrackContainerDetailView() { return poTrackContainerDetailView; };
  static CTrackSubContainerDetailView* useTrackSubContainerDetailView() { return poTrackSubContainerDetailView; };
  static CTrackPointDetailView* useTrackPointDetailView() { return poTrackPointDetailView; };
  static CVesselOverlay* useVesselOverlay() { return poVesselOverlay; };
  static CVesselOverlayListView* useVesselOverlayListView() { return poVesselOverlayListView; };
  static CVesselContainerDetailView* useVesselContainerDetailView() { return poVesselContainerDetailView; };
  static CVesselCourseDockView* useVesselCourseDockView() { return poVesselCourseDockView; };
  static CVesselPointDetailView* useVesselPointDetailView() { return poVesselPointDetailView; };
  static CVesselPositionDockView* useVesselPositionDockView() { return poVesselPositionDockView; };
  static CVesselContainerDeviceDetailView* useVesselContainerDeviceDetailView() { return poVesselContainerDeviceDetailView; };
  static CVesselPointDeviceDetailView* useVesselPointDeviceDetailView() { return poVesselPointDeviceDetailView; };

};

#endif // QVCT_QVCTRUNTIME_HPP
