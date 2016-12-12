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
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/dock/COverlayListView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayListView::COverlayListView( QWidget* _pqParent )
  : QDockWidget( tr("Overlay List"), _pqParent )
  , poPointerOverlay( 0 )
  , pqTabWidget( 0 )
{
  QDockWidget::setObjectName( "OverlayList" ); // required to save main window's state
  QDockWidget::setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  constructLayout();

  // Additional overlay (not visible as list)
  // ... pointer
  poPointerOverlay = new CPointerOverlay();
  QVCTRuntime::registerPointerOverlay( poPointerOverlay );
}

COverlayListView::~COverlayListView()
{
  if( poPointerOverlay ) delete poPointerOverlay;
}

void COverlayListView::constructLayout()
{
  // Create layout
  // NOTE: Merely a place-holder to insure the same margin appearance as the COverlayDetailView
  QWidget* __pqWidget = new QWidget();
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Create tab widget
  pqTabWidget = new QTabWidget();
  __pqVBoxLayout->addWidget( pqTabWidget );
  // ... style
  pqTabWidget->setStyleSheet( ".QTabBar::tab { PADDING:4px 0px 0px 14px; HEIGHT:28px; }" );

  // Add overlay
  int __iIndex = -1;

  // ... landmark (COverlayListView::LANDMARK)
  __iIndex++;
  CLandmarkOverlayListView* __poLandmarkOverlayListView = new CLandmarkOverlayListView();
  QVCTRuntime::registerLandmarkOverlayListView( __poLandmarkOverlayListView );
  pqTabWidget->addTab( __poLandmarkOverlayListView, QIcon( ":icons/16x16/landmark.png" ), "" );
  pqTabWidget->setTabToolTip( __iIndex, tr("Landmarks") );

  // ... route (COverlayListView::ROUTE)
  __iIndex++;
  CRouteOverlayListView* __poRouteOverlayListView = new CRouteOverlayListView();
  QVCTRuntime::registerRouteOverlayListView( __poRouteOverlayListView );
  pqTabWidget->addTab( __poRouteOverlayListView, QIcon( ":icons/16x16/route.png" ), "" );
  pqTabWidget->setTabToolTip( __iIndex, tr("Routes") );

  // ... track (COverlayListView::TRACK)
  __iIndex++;
  CTrackOverlayListView* __poTrackOverlayListView = new CTrackOverlayListView();
  QVCTRuntime::registerTrackOverlayListView( __poTrackOverlayListView );
  pqTabWidget->addTab( __poTrackOverlayListView, QIcon( ":icons/16x16/track.png" ), "" );
  pqTabWidget->setTabToolTip( __iIndex, tr("Tracks") );

  // ... vessel (COverlayListView::VESSEL)
  __iIndex++;
  CVesselOverlayListView* __poVesselOverlayListView = new CVesselOverlayListView();
  QVCTRuntime::registerVesselOverlayListView( __poVesselOverlayListView );
  pqTabWidget->addTab( __poVesselOverlayListView, QIcon( ":icons/16x16/vessel.png" ), "" );
  pqTabWidget->setTabToolTip( __iIndex, tr("Vessels") );

  // ... device (COverlayListView::DEVICE)
  __iIndex++;
  CDeviceOverlayListView* __poDeviceOverlayListView = new CDeviceOverlayListView();
  QVCTRuntime::registerDeviceOverlayListView( __poDeviceOverlayListView );
  pqTabWidget->addTab( __poDeviceOverlayListView, QIcon( ":icons/16x16/device.png" ), "" );
  pqTabWidget->setTabToolTip( __iIndex, tr("Devices") );

  // ... [end]
  connect( pqTabWidget, SIGNAL( currentChanged(int) ), this, SLOT( slotChangeTab(int) ) );

  // Finalize
  __pqWidget->setLayout( __pqVBoxLayout );
  QDockWidget::setWidget( __pqWidget );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void COverlayListView::slotChangeTab( int _iTabIndex )
{
  COverlayBaseTree* __poOverlayBaseTree;
  switch( _iTabIndex )
  {
  case LANDMARK: __poOverlayBaseTree = QVCTRuntime::useLandmarkOverlay(); break;
  case ROUTE: __poOverlayBaseTree = QVCTRuntime::useRouteOverlay(); break;
  case TRACK: __poOverlayBaseTree = QVCTRuntime::useTrackOverlay(); break;
  case VESSEL: __poOverlayBaseTree = QVCTRuntime::useVesselOverlay(); break;
  case DEVICE: __poOverlayBaseTree = QVCTRuntime::useDeviceOverlay(); break;
  default: return; // WHAT THE F*** !?!
  }
  __poOverlayBaseTree->showDetail( __poOverlayBaseTree->currentItem() );
}

//
// OTHER
//

void COverlayListView::switchView( COverlayListView::EView eView )
{
  pqTabWidget->setCurrentIndex( eView );
}
