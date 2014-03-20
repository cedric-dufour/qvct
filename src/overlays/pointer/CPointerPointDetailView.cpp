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
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/pointer/CPointerPointDetailView.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRoutePoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CPointerPointDetailView::CPointerPointDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CPointerPointDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "", this );
  pqPushButtonVisible->setToolTip( tr("Toggle the pointer visibility") );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "", this );
  pqPushButtonCenter->setToolTip( tr("Center chart on the pointer position") );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete the last pointer path's position") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... Add landmark
  pqPushButtonAddLandmark = new QPushButton( QIcon( ":icons/32x32/landmark_add.png" ), "", this );
  pqPushButtonAddLandmark->setToolTip( tr("Create new landmark(s) at the pointer (path) positions") );
  pqPushButtonAddLandmark->setMaximumSize( 36, 34 );
  pqPushButtonAddLandmark->setEnabled( false );
  QWidget::connect( pqPushButtonAddLandmark, SIGNAL( clicked() ), this, SLOT( slotAddLandmark() ) );
  // ... Add route
  pqPushButtonAddRoute = new QPushButton( QIcon( ":icons/32x32/route_add.png" ), "", this );
  pqPushButtonAddRoute->setToolTip( tr("Create new (route) waypoint(s) at the pointer (path) position(s)") );
  pqPushButtonAddRoute->setMaximumSize( 36, 34 );
  pqPushButtonAddRoute->setEnabled( false );
  QWidget::connect( pqPushButtonAddRoute, SIGNAL( clicked() ), this, SLOT( slotAddRoute() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/pointer.png" ) );
  __pqLabelIcon->setToolTip( tr("Pointer") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  COverlayText* __poTextName = new COverlayText( tr("Pointer"), this );
  __poTextName->setToolTip( tr("Name") );
  __poTextName->setFont( __qFontHeader );
  __poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( __poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget( this );
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... position
  __qFontData.setPixelSize( 20 );
  QWidget* __poWidgetPosition = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutPosition = new QVBoxLayout( __poWidgetPosition );
  poTextLongitude = new COverlayText( this );
  poTextLongitude->setToolTip( tr("Longitude") );
  poTextLongitude->setFont( __qFontData );
  poTextLongitude->setIndent( 10 );
  poTextLongitude->setAlignment( Qt::AlignHCenter );
  poTextLongitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLongitude );
  poTextLatitude = new COverlayText( this );
  poTextLatitude->setToolTip( tr("Latitude") );
  poTextLatitude->setFont( __qFontData );
  poTextLatitude->setIndent( 10 );
  poTextLatitude->setAlignment( Qt::AlignHCenter );
  poTextLatitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLatitude );
  poTextElevation = new COverlayText( this );
  poTextElevation->setToolTip( tr("Elevation") );
  poTextElevation->setFont( __qFontData );
  poTextElevation->setIndent( 10 );
  poTextElevation->setAlignment( Qt::AlignHCenter );
  poTextElevation->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextElevation, 1 );
  __poWidgetPosition->setLayout( __pqVBoxLayoutPosition );
  __poTabWidget->addTab( __poWidgetPosition, tr("Position") );

  // ... path
  QWidget* __poWidgetPath = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutPath = new QVBoxLayout( __poWidgetPath );
  poTextPathSegments = new COverlayText( this );
  poTextPathSegments->setToolTip( tr("Segments") );
  poTextPathSegments->setFont( __qFontData );
  poTextPathSegments->setIndent( 10 );
  poTextPathSegments->setAlignment( Qt::AlignHCenter );
  poTextPathSegments->resetText();
  __pqVBoxLayoutPath->addWidget( poTextPathSegments );
  poTextPathLengthRL = new COverlayText( this );
  poTextPathLengthRL->setToolTip( tr("Length (rhumb-line along segments)") );
  poTextPathLengthRL->setFont( __qFontData );
  poTextPathLengthRL->setIndent( 10 );
  poTextPathLengthRL->setAlignment( Qt::AlignHCenter );
  poTextPathLengthRL->resetText();
  __pqVBoxLayoutPath->addWidget( poTextPathLengthRL );
  poTextPathLengthGC = new COverlayText( this );
  poTextPathLengthGC->setToolTip( tr("Length (great-circle equivalent)") );
  poTextPathLengthGC->setFont( __qFontData );
  poTextPathLengthGC->setIndent( 10 );
  poTextPathLengthGC->setAlignment( Qt::AlignHCenter );
  poTextPathLengthGC->resetText();
  __pqVBoxLayoutPath->addWidget( poTextPathLengthGC, 1 );
  __poWidgetPath->setLayout( __pqVBoxLayoutPath );
  __poTabWidget->addTab( __poWidgetPath, tr("Path") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonVisible, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenter, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddLandmark, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddRoute, 0, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  QWidget::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CPointerPointDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = (CPointerPoint*)poOverlayObject;

  // ... position
  poTextLongitude->setText( CUnitPosition::toString( __poPointerPoint->getLongitude(), CUnitPosition::LONGITUDE ) );
  poTextLatitude->setText( CUnitPosition::toString( __poPointerPoint->getLatitude(), CUnitPosition::LATITUDE ) );

  // ... elevation
  if( __poPointerPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    poTextElevation->setText( CUnitElevation::toString( __poPointerPoint->getElevation() ) );
  else
    poTextElevation->resetText();

  // ... path
  if( __poPointerOverlay->hasPath() )
  {
    poTextPathSegments->setText( QString::number( __poPointerOverlay->getPathSegments() )+" "+tr("segment(s)") );
    poTextPathLengthRL->setText( CUnitDistance::toString( __poPointerOverlay->getPathLengthRL() ) );
    poTextPathLengthGC->setText( "("+CUnitDistance::toString( __poPointerOverlay->getPathLengthGC() )+")", true );
  }
  else
  {
    poTextPathSegments->resetText();
    poTextPathLengthRL->resetText();
    poTextPathLengthGC->resetText();
  }

  // ... buttons
  pqPushButtonDelete->setEnabled( QVCTRuntime::usePointerOverlay()->hasPath() );
}

void CPointerPointDetailView::resetContent()
{
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
  poTextPathSegments->resetText();
  poTextPathLengthRL->resetText();
  poTextPathLengthGC->resetText();
}

void CPointerPointDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonAddLandmark->setEnabled( true );
    pqPushButtonAddRoute->setEnabled( true );
  }
}

void CPointerPointDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonAddLandmark->setEnabled( false );
  pqPushButtonAddRoute->setEnabled( false );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CPointerPointDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CPointerPoint* __poPointerPoint = (CPointerPoint*)poOverlayObject;
  __poPointerPoint->toggleVisibility();
  __poPointerPoint->useOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CPointerPointDetailView::slotPositionCenter()
{
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  if( __poPointerOverlay->hasPath() )
  {
    CDataPosition _oDataPositionLower, _oDataPositionUpper;
    int __iCount = COverlayPoint::getPositionBox( __poPointerOverlay, &_oDataPositionLower, &_oDataPositionUpper, 1 );
    if( __iCount > 1 ) QVCTRuntime::useChartTable()->setScaleArea( _oDataPositionLower, _oDataPositionUpper, 0.9 );
    else if( __iCount == 1 ) QVCTRuntime::useChartTable()->setGeoPosition( _oDataPositionLower );
    if( __iCount ) return;
  }
  if( !poOverlayObject ) return;
  CPointerPoint* __poPointerPoint = (CPointerPoint*)poOverlayObject;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poPointerPoint );
  QVCTRuntime::useChartTable()->updateChart();
}

void CPointerPointDetailView::slotDelete()
{
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  if( !__poPointerOverlay->hasPath() ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CPointerPoint* __poPointerPoint = (CPointerPoint*)__poPointerOverlay->takeChild( __poPointerOverlay->childCount()-1 );
  delete __poPointerPoint;
  __poPointerPoint = __poPointerOverlay->usePointerPoint();
  __poPointerPoint->setPosition( *(CPointerPoint*)__poPointerOverlay->child( __poPointerOverlay->childCount()-1 ) );
  __poPointerPoint->showDetail();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}

void CPointerPointDetailView::slotAddLandmark()
{
  if( !poOverlayObject ) return;
  CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
  CLandmarkContainer* __poLandmarkContainer = __poLandmarkOverlay->pickContainer();
  if( !__poLandmarkContainer ) return;
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  CLandmarkPoint* __poLandmarkPoint = 0;
  if( __poPointerOverlay->hasPath() )
  {
    int __iCount = __poPointerOverlay->childCount();
    for( int __i = 2; __i < __iCount; __i++ )
    {
      QString __qsName = __poLandmarkContainer->newChildName( tr("Landmark"), 1, true );
      __poLandmarkContainer->addPoint( __qsName, *(CPointerPoint*)__poPointerOverlay->child( __i ) );
    }
    __poLandmarkOverlay->setCurrentItem( __poLandmarkContainer );
  }
  else
  {
    QString __qsName = __poLandmarkContainer->newChildName( tr("Landmark"), 1, true );
    __poLandmarkPoint = __poLandmarkContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject );
    if( __poLandmarkPoint ) __poLandmarkOverlay->setCurrentItem( __poLandmarkPoint );
  }
  __poLandmarkOverlay->forceRedraw();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->clearPath();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  if( __poLandmarkPoint ) __poLandmarkPoint->showEdit();
}

void CPointerPointDetailView::slotAddRoute()
{
  if( !poOverlayObject ) return;
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  CRouteContainer* __poRouteContainer = __poRouteOverlay->pickContainer();
  if( !__poRouteContainer ) return;
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  CRoutePoint* __poRoutePoint = 0;
  if( __poPointerOverlay->hasPath() )
  {
    int __iCount = __poPointerOverlay->childCount();
    for( int __i = 2; __i < __iCount; __i++ )
    {
      QString __qsName = __poRouteContainer->newChildName( tr("Waypoint"), 1, true );
      __poRouteContainer->addPoint( __qsName, *(CPointerPoint*)__poPointerOverlay->child( __i ) );
    }
    __poRouteOverlay->setCurrentItem( __poRouteContainer );
  }
  else
  {
    QString __qsName = __poRouteContainer->newChildName( tr("Waypoint"), 1, true );
    __poRoutePoint = __poRouteContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject, true );
    if( __poRoutePoint ) __poRouteOverlay->setCurrentItem( __poRoutePoint );
  }
  __poRouteOverlay->forceRedraw();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->clearPath();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  if( __poRoutePoint ) __poRoutePoint->showEdit();
}
