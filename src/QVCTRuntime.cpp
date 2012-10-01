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

// QT
#include <QDockWidget>

// QVCT
#include "QVCTRuntime.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

QMutex* QVCTRuntime::pqMutexDataChange = new QMutex();
QTimer* QVCTRuntime::pqTimerRefresh = new QTimer();

CSettings* QVCTRuntime::poSettings = 0;
CMainWindow* QVCTRuntime::poMainWindow = 0;
CTimeView* QVCTRuntime::poTimeView = 0;
CChartTable* QVCTRuntime::poChartTable = 0;
CChartControl* QVCTRuntime::poChartControl = 0;
CDeviceDetailView* QVCTRuntime::poDeviceDetailView = 0;
COverlayDetailView* QVCTRuntime::poOverlayDetailView = 0;
COverlayListView* QVCTRuntime::poOverlayListView = 0;
CDeviceOverlay* QVCTRuntime::poDeviceOverlay = 0;
CDeviceOverlayListView* QVCTRuntime::poDeviceOverlayListView = 0;
CPointerOverlay* QVCTRuntime::poPointerOverlay = 0;
CPointerPointDetailView* QVCTRuntime::poPointerPointDetailView = 0;
CLandmarkOverlay* QVCTRuntime::poLandmarkOverlay = 0;
CLandmarkOverlayListView* QVCTRuntime::poLandmarkOverlayListView = 0;
CLandmarkContainerDetailView* QVCTRuntime::poLandmarkContainerDetailView = 0;
CLandmarkPointDetailView* QVCTRuntime::poLandmarkPointDetailView = 0;
CRouteOverlay* QVCTRuntime::poRouteOverlay = 0;
CRouteOverlayListView* QVCTRuntime::poRouteOverlayListView = 0;
CRouteContainerDetailView* QVCTRuntime::poRouteContainerDetailView = 0;
CRoutePointDetailView* QVCTRuntime::poRoutePointDetailView = 0;
CTrackOverlay* QVCTRuntime::poTrackOverlay = 0;
CTrackOverlayListView* QVCTRuntime::poTrackOverlayListView = 0;
CTrackContainerDetailView* QVCTRuntime::poTrackContainerDetailView = 0;
CTrackSubContainerDetailView* QVCTRuntime::poTrackSubContainerDetailView = 0;
CTrackPointDetailView* QVCTRuntime::poTrackPointDetailView = 0;
CVesselOverlay* QVCTRuntime::poVesselOverlay = 0;
CVesselOverlayListView* QVCTRuntime::poVesselOverlayListView = 0;
CVesselContainerDetailView* QVCTRuntime::poVesselContainerDetailView = 0;
CVesselCourseDockView* QVCTRuntime::poVesselCourseDockView = 0;
CVesselPointDetailView* QVCTRuntime::poVesselPointDetailView = 0;
CVesselPositionDockView* QVCTRuntime::poVesselPositionDockView = 0;
CVesselTargetDockView* QVCTRuntime::poVesselTargetDockView = 0;
CVesselContainerDeviceDetailView* QVCTRuntime::poVesselContainerDeviceDetailView = 0;
CVesselPointDeviceDetailView* QVCTRuntime::poVesselPointDeviceDetailView = 0;
