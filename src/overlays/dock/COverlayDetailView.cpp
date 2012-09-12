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
#include <QStackedWidget>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/dock/COverlayDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayDetailView::COverlayDetailView( QWidget* _pqParent )
  : QDockWidget( tr("Overlay Detail"), _pqParent )
  , pqStackedWidget( 0 )
{
  QDockWidget::setObjectName( "OverlayDetail" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  constructLayout();
}

void COverlayDetailView::constructLayout()
{
  // Create stacked widget
  pqStackedWidget = new QStackedWidget( this );

  // Add overlay

  // ... pointer (COverlayDetailView::POINTER_POINT)
  CPointerPointDetailView* __poPointerPointDetailView = new CPointerPointDetailView( pqStackedWidget );
  QVCTRuntime::registerPointerPointDetailView( __poPointerPointDetailView );
  pqStackedWidget->addWidget( __poPointerPointDetailView );

  // ... landmark container (COverlayDetailView::LANDMARK_CONTAINER)
  CLandmarkContainerDetailView* __poLandmarkContainerDetailView = new CLandmarkContainerDetailView( pqStackedWidget );
  QVCTRuntime::registerLandmarkContainerDetailView( __poLandmarkContainerDetailView );
  pqStackedWidget->addWidget( __poLandmarkContainerDetailView );

  // ... landmark point (COverlayDetailView::LANDMARK_POINT)
  CLandmarkPointDetailView* __poLandmarkPointDetailView = new CLandmarkPointDetailView( pqStackedWidget );
  QVCTRuntime::registerLandmarkPointDetailView( __poLandmarkPointDetailView );
  pqStackedWidget->addWidget( __poLandmarkPointDetailView );

  // ... route container (COverlayDetailView::ROUTE_CONTAINER)
  CRouteContainerDetailView* __poRouteContainerDetailView = new CRouteContainerDetailView( pqStackedWidget );
  QVCTRuntime::registerRouteContainerDetailView( __poRouteContainerDetailView );
  pqStackedWidget->addWidget( __poRouteContainerDetailView );

  // ... route point (COverlayDetailView::ROUTE_POINT)
  CRoutePointDetailView* __poRoutePointDetailView = new CRoutePointDetailView( pqStackedWidget );
  QVCTRuntime::registerRoutePointDetailView( __poRoutePointDetailView );
  pqStackedWidget->addWidget( __poRoutePointDetailView );

  // ... track container (COverlayDetailView::TRACK_CONTAINER)
  CTrackContainerDetailView* __poTrackContainerDetailView = new CTrackContainerDetailView( pqStackedWidget );
  QVCTRuntime::registerTrackContainerDetailView( __poTrackContainerDetailView );
  pqStackedWidget->addWidget( __poTrackContainerDetailView );

  // ... track sub-container (COverlayDetailView::TRACK_SUBCONTAINER)
  CTrackSubContainerDetailView* __poTrackSubContainerDetailView = new CTrackSubContainerDetailView( pqStackedWidget );
  QVCTRuntime::registerTrackSubContainerDetailView( __poTrackSubContainerDetailView );
  pqStackedWidget->addWidget( __poTrackSubContainerDetailView );

  // ... track point (COverlayDetailView::TRACK_POINT)
  CTrackPointDetailView* __poTrackPointDetailView = new CTrackPointDetailView( pqStackedWidget );
  QVCTRuntime::registerTrackPointDetailView( __poTrackPointDetailView );
  pqStackedWidget->addWidget( __poTrackPointDetailView );

  // ... vessel container (COverlayDetailView::VESSEL_CONTAINER)
  CVesselContainerDetailView* __poVesselContainerDetailView = new CVesselContainerDetailView( pqStackedWidget );
  QVCTRuntime::registerVesselContainerDetailView( __poVesselContainerDetailView );
  pqStackedWidget->addWidget( __poVesselContainerDetailView );

  // ... vessel container's device (COverlayDetailView::VESSEL_CONTAINER_DEVICE)
  CVesselContainerDeviceDetailView* __poVesselContainerDeviceDetailView = new CVesselContainerDeviceDetailView( pqStackedWidget );
  QVCTRuntime::registerVesselContainerDeviceDetailView( __poVesselContainerDeviceDetailView );
  pqStackedWidget->addWidget( __poVesselContainerDeviceDetailView );

  // ... vessel point (COverlayDetailView::VESSEL_POINT)
  CVesselPointDetailView* __poVesselPointDetailView = new CVesselPointDetailView( pqStackedWidget );
  QVCTRuntime::registerVesselPointDetailView( __poVesselPointDetailView );
  pqStackedWidget->addWidget( __poVesselPointDetailView );

  // ... vessel point's device (COverlayDetailView::VESSEL_POINT_DEVICE)
  CVesselPointDeviceDetailView* __poVesselPointDeviceDetailView = new CVesselPointDeviceDetailView( pqStackedWidget );
  QVCTRuntime::registerVesselPointDeviceDetailView( __poVesselPointDeviceDetailView );
  pqStackedWidget->addWidget( __poVesselPointDeviceDetailView );

  // ... device (COverlayDetailView::DEVICE)
  CDeviceDetailView* __poDeviceDetailView = new CDeviceDetailView( pqStackedWidget );
  QVCTRuntime::registerDeviceDetailView( __poDeviceDetailView );
  pqStackedWidget->addWidget( __poDeviceDetailView );

  // Finalize
  pqStackedWidget->setMaximumHeight( pqStackedWidget->minimumSizeHint().height() );
  QDockWidget::setWidget( pqStackedWidget );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void COverlayDetailView::switchView( COverlayDetailView::EView eView )
{
  if( QVCTRuntime::useChartTable()->getOverlayPointMove() ) return; // NOTE: Prevent view switching when moving a point
  pqStackedWidget->setCurrentIndex( eView );
}
