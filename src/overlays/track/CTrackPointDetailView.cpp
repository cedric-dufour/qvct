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
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/data/CDeviceDataDop.hpp"
#include "overlays/COverlayText.hpp"
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/track/CTrackPointDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackPointDetailView::CTrackPointDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
{
  constructLayout();
}

void CTrackPointDetailView::constructLayout()
{
  // Create the buttons
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "" );
  pqPushButtonCenter->setToolTip( tr("Center chart on this point") );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete this point") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... Add landmark
  pqPushButtonAddLandmark = new QPushButton( QIcon( ":icons/32x32/landmark_add.png" ), "" );
  pqPushButtonAddLandmark->setToolTip( tr("Create a new landmark at the point position") );
  pqPushButtonAddLandmark->setMaximumSize( 36, 34 );
  pqPushButtonAddLandmark->setEnabled( false );
  QWidget::connect( pqPushButtonAddLandmark, SIGNAL( clicked() ), this, SLOT( slotAddLandmark() ) );
  // ... Add (route) waypoint
  pqPushButtonAddRoute = new QPushButton( QIcon( ":icons/32x32/route_add.png" ), "" );
  pqPushButtonAddRoute->setToolTip( tr("Create a new (route) waypoint at the point position") );
  pqPushButtonAddRoute->setMaximumSize( 36, 34 );
  pqPushButtonAddRoute->setEnabled( false );
  QWidget::connect( pqPushButtonAddRoute, SIGNAL( clicked() ), this, SLOT( slotAddRoute() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/track_point.png" ) );
  __pqLabelIcon->setToolTip( tr("Point") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText();
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget();
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... position
  QWidget* __poWidgetPosition = new QWidget();
  QVBoxLayout* __pqVBoxLayoutPosition = new QVBoxLayout();
  __qFontData.setPixelSize( 20 );
  poTextLongitude = new COverlayText();
  poTextLongitude->setToolTip( tr("Longitude") );
  poTextLongitude->setFont( __qFontData );
  poTextLongitude->setIndent( 10 );
  poTextLongitude->setAlignment( Qt::AlignHCenter );
  poTextLongitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLongitude );
  poTextLatitude = new COverlayText();
  poTextLatitude->setToolTip( tr("Latitude") );
  poTextLatitude->setFont( __qFontData );
  poTextLatitude->setIndent( 10 );
  poTextLatitude->setAlignment( Qt::AlignHCenter );
  poTextLatitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLatitude );
  poTextElevation = new COverlayText();
  poTextElevation->setToolTip( tr("Elevation") );
  poTextElevation->setFont( __qFontData );
  poTextElevation->setIndent( 10 );
  poTextElevation->setAlignment( Qt::AlignHCenter );
  poTextElevation->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextElevation, 1 );
  __poWidgetPosition->setLayout( __pqVBoxLayoutPosition );
  __poTabWidget->addTab( __poWidgetPosition, tr("Position") );

  // ... date/time
  QWidget* __poWidgetTime = new QWidget();
  QVBoxLayout* __pqVBoxLayoutTime = new QVBoxLayout();
  poTextDate = new COverlayText();
  poTextDate->setToolTip( tr("Date") );
  poTextDate->setFont( __qFontData );
  poTextDate->setIndent( 10 );
  poTextDate->setAlignment( Qt::AlignHCenter );
  poTextDate->resetText();
  __pqVBoxLayoutTime->addWidget( poTextDate );
  poTextTime = new COverlayText();
  poTextTime->setToolTip( tr("Time") );
  poTextTime->setFont( __qFontData );
  poTextTime->setIndent( 10 );
  poTextTime->setAlignment( Qt::AlignHCenter );
  poTextTime->resetText();
  __pqVBoxLayoutTime->addWidget( poTextTime );
  poTextTime2 = new COverlayText();
  poTextTime2->setToolTip( tr("Time (alternate timezone)") );
  poTextTime2->setFont( __qFontData );
  poTextTime2->setIndent( 10 );
  poTextTime2->setAlignment( Qt::AlignHCenter );
  poTextTime2->resetText();
  __pqVBoxLayoutTime->addWidget( poTextTime2, 1 );
  __poWidgetTime->setLayout( __pqVBoxLayoutTime );
  __poTabWidget->addTab( __poWidgetTime, tr("Time") );

  // ... precision
  QWidget* __poWidgetPrecision = new QWidget();
  QVBoxLayout* __pqVBoxLayoutPrecision = new QVBoxLayout();
  poTextFixType = new COverlayText();
  poTextFixType->setToolTip( tr("Fix Type") );
  poTextFixType->setFont( __qFontData );
  poTextFixType->setIndent( 10 );
  poTextFixType->setAlignment( Qt::AlignHCenter );
  poTextFixType->resetText();
  __pqVBoxLayoutPrecision->addWidget( poTextFixType );
  poTextSatellite = new COverlayText();
  poTextSatellite->setToolTip( tr("Satellites Used") );
  poTextSatellite->setFont( __qFontData );
  poTextSatellite->setIndent( 10 );
  poTextSatellite->setAlignment( Qt::AlignHCenter );
  poTextSatellite->resetText();
  __pqVBoxLayoutPrecision->addWidget( poTextSatellite );
  poTextDop = new COverlayText();
  poTextDop->setToolTip( tr("HDOP / VDOP") );
  poTextDop->setFont( __qFontData );
  poTextDop->setIndent( 10 );
  poTextDop->setAlignment( Qt::AlignHCenter );
  poTextDop->resetText();
  __pqVBoxLayoutPrecision->addWidget( poTextDop, 1 );
  __poWidgetPrecision->setLayout( __pqVBoxLayoutPrecision );
  __poTabWidget->addTab( __poWidgetPrecision, tr("Precision") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame();
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenter, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddLandmark, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddRoute, 0, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CTrackPointDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CTrackPoint* __poTrackPoint = (CTrackPoint*)poOverlayObject;

  // ... name
  poTextName->setText( __poTrackPoint->getName() );

  // ... position
  if( __poTrackPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && __poTrackPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    poTextLongitude->setText( CUnitPosition::toString( __poTrackPoint->getLongitude(), CUnitPosition::LONGITUDE ) );
    poTextLatitude->setText( CUnitPosition::toString( __poTrackPoint->getLatitude(), CUnitPosition::LATITUDE ) );
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
  }

  // ... elevation
  if( __poTrackPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    poTextElevation->setText( CUnitElevation::toString( __poTrackPoint->getElevation() ) );
  else
    poTextElevation->resetText();

  // ... time
  if( __poTrackPoint->getTime() != CDataTime::UNDEFINED_TIME )
  {
    double __fdTime = __poTrackPoint->getTime();
    poTextDate->setText( CUnitDate::toString( __fdTime ) );
    poTextTime->setText( CUnitTime::toString( __fdTime ) );
    poTextTime2->setText( "("+CUnitTime::toString( __fdTime, true )+")" );
  }
  else
  {
    poTextDate->resetText();
    poTextTime->resetText();
    poTextTime2->resetText();
  }

  // ... precision
  poTextFixType->setText( __poTrackPoint->getFixType() );
  if( __poTrackPoint->getSatelliteCount() )
    poTextSatellite->setText( QString::number( __poTrackPoint->getSatelliteCount() ) );
  else
    poTextSatellite->resetText();
  if( __poTrackPoint->getDopHorizontal() != CDeviceDataDop::UNDEFINED_VALUE || __poTrackPoint->getDopVertical() != CDeviceDataDop::UNDEFINED_VALUE )
    poTextDop->setText( ( __poTrackPoint->getDopHorizontal() != CDeviceDataDop::UNDEFINED_VALUE ? QString::number( __poTrackPoint->getDopHorizontal(), 'f', 1 ) : poTextDop->emptyString() )
                        +" / "
                        +( __poTrackPoint->getDopVertical() != CDeviceDataDop::UNDEFINED_VALUE ? QString::number( __poTrackPoint->getDopVertical(), 'f', 1 ) : poTextDop->emptyString() ) );
  else
    poTextDop->resetText();
}

void CTrackPointDetailView::resetContent()
{
  poTextName->resetText();
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
  poTextDate->resetText();
  poTextTime->resetText();
  poTextTime2->resetText();
  poTextFixType->resetText();
  poTextSatellite->resetText();
  poTextDop->resetText();
}

void CTrackPointDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonAddLandmark->setEnabled( true );
    pqPushButtonAddRoute->setEnabled( true );
  }
}

void CTrackPointDetailView::disableContent()
{
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

void CTrackPointDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CTrackPoint* __poTrackPoint = (CTrackPoint*)poOverlayObject;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poTrackPoint );
  QVCTRuntime::useChartTable()->updateChart();
}

void CTrackPointDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  CTrackOverlay* __poTrackOverlay = QVCTRuntime::useTrackOverlay();
  CTrackContainer* __poTrackContainer = (CTrackContainer*)((QTreeWidgetItem*)poOverlayObject)->parent();
  CTrackPoint* __poTrackPoint = (CTrackPoint*)poOverlayObject;
  __pqMutexDataChange->lock();
  __poTrackContainer->removeChild( __poTrackPoint );
  __pqMutexDataChange->unlock();
  delete __poTrackPoint;
  QTreeWidgetItem* __pqTreeWidgetItem = __poTrackOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poTrackOverlay->showDetail( __pqTreeWidgetItem );
  __poTrackOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CTrackPointDetailView::slotAddLandmark()
{
  if( !poOverlayObject ) return;
  CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
  CLandmarkContainer* __poLandmarkContainer = __poLandmarkOverlay->pickContainer();
  if( !__poLandmarkContainer ) return;
  QString __qsName = __poLandmarkContainer->newChildName( poOverlayObject->getName() );
  CLandmarkPoint* __poLandmarkPoint = __poLandmarkContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject );
  if( !__poLandmarkPoint ) return;
  __poLandmarkOverlay->setCurrentItem( __poLandmarkPoint );
  __poLandmarkOverlay->forceRedraw();
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __poLandmarkPoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}

void CTrackPointDetailView::slotAddRoute()
{
  if( !poOverlayObject ) return;
  CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
  CRouteContainer* __poRouteContainer = __poRouteOverlay->pickContainer();
  if( !__poRouteContainer ) return;
  QString __qsName = __poRouteContainer->newChildName( poOverlayObject->getName() );
  CRoutePoint* __poRoutePoint = __poRouteContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject, true );
  if( !__poRoutePoint ) return;
  __poRouteOverlay->setCurrentItem( __poRoutePoint );
  __poRouteOverlay->forceRedraw();
  CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
  CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
  __poPointerPoint->resetPosition();
  __poPointerOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __poRoutePoint->showEdit();
  QVCTRuntime::useChartTable()->setProjectModified();
}
