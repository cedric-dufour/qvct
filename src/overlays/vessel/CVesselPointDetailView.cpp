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
#include "overlays/COverlayText.hpp"
#include "overlays/COverlayUrl.hpp"
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/device/CVesselPointDevice.hpp"
#include "overlays/vessel/device/CVesselPointDeviceCreateView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointDetailView::CVesselPointDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
  , bIgnoreUpdate( true )
{
  constructLayout();
}

void CVesselPointDetailView::constructLayout()
{
  // Create the buttons
  // ... visible
  pqPushButtonVisible = new QPushButton( QIcon( ":icons/32x32/visible.png" ), "", this );
  pqPushButtonVisible->setToolTip( tr("Toggle this vessel's visibility") );
  pqPushButtonVisible->setMaximumSize( 36, 34 );
  pqPushButtonVisible->setEnabled( false );
  QWidget::connect( pqPushButtonVisible, SIGNAL( clicked() ), this, SLOT( slotToggleVisible() ) );
  // ... center
  pqPushButtonCenter = new QPushButton( QIcon( ":icons/32x32/center.png" ), "", this );
  pqPushButtonCenter->setToolTip( tr("Center chart on this vessel") );
  pqPushButtonCenter->setMaximumSize( 36, 34 );
  pqPushButtonCenter->setEnabled( false );
  QWidget::connect( pqPushButtonCenter, SIGNAL( clicked() ), this, SLOT( slotPositionCenter() ) );
  // ... center (lock)
  pqPushButtonCenterLock = new QPushButton( QIcon( ":icons/32x32/center_unlock.png" ), "", this );
  pqPushButtonCenterLock->setToolTip( tr("Center and lock chart on this vessel") );
  pqPushButtonCenterLock->setMaximumSize( 36, 34 );
  pqPushButtonCenterLock->setEnabled( false );
  pqPushButtonCenterLock->setCheckable( true );
  QWidget::connect( pqPushButtonCenterLock, SIGNAL( toggled(bool) ), this, SLOT( slotPositionCenterLock(bool) ) );
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "", this );
  pqPushButtonEdit->setToolTip( tr("Edit this vessel") );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete this vessel") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // // ... Add landmark
  // pqPushButtonAddLandmark = new QPushButton( QIcon( ":icons/32x32/landmark_add.png" ), "", this );
  // pqPushButtonAddLandmark->setToolTip( tr("Create a new landmark at the vessel position") );
  // pqPushButtonAddLandmark->setMaximumSize( 36, 34 );
  // pqPushButtonAddLandmark->setEnabled( false );
  // QWidget::connect( pqPushButtonAddLandmark, SIGNAL( clicked() ), this, SLOT( slotAddLandmark() ) );
  // // ... Add route
  // pqPushButtonAddRoute = new QPushButton( QIcon( ":icons/32x32/route_add.png" ), "", this );
  // pqPushButtonAddRoute->setToolTip( tr("Create a new (route) waypoint at the vessel position") );
  // pqPushButtonAddRoute->setMaximumSize( 36, 34 );
  // pqPushButtonAddRoute->setEnabled( false );
  // QWidget::connect( pqPushButtonAddRoute, SIGNAL( clicked() ), this, SLOT( slotAddRoute() ) );
  // ... Track recording status
  pqPushButtonTrackRecord = new QPushButton( QIcon( ":icons/32x32/track_stop.png" ), "", this );
  pqPushButtonTrackRecord->setToolTip( tr("Switch the track recording status for this vessel") );
  pqPushButtonTrackRecord->setMaximumSize( 36, 34 );
  pqPushButtonTrackRecord->setEnabled( false );
  pqPushButtonTrackRecord->setCheckable( true );
  QWidget::connect( pqPushButtonTrackRecord, SIGNAL( toggled(bool) ), this, SLOT( slotTrackRecord(bool) ) );
  // ... Add device
  pqPushButtonAddDevice = new QPushButton( QIcon( ":icons/32x32/device_add.png" ), "", this );
  pqPushButtonAddDevice->setToolTip( tr("Create a new device to slave this vessel's position and course") );
  pqPushButtonAddDevice->setMaximumSize( 36, 34 );
  pqPushButtonAddDevice->setEnabled( false );
  QWidget::connect( pqPushButtonAddDevice, SIGNAL( clicked() ), this, SLOT( slotAddDevice() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_point.png" ) );
  __pqLabelIcon->setToolTip( tr("Vessel") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  pqLabelSymbol = new QLabel( this );
  pqLabelSymbol->setMaximumSize( 32, 32 );
  pqLabelSymbol->setVisible( false );
  __pqHBoxLayoutHeader->addWidget( pqLabelSymbol );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget( this );
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... position
  QWidget* __poWidgetPosition = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutPosition = new QVBoxLayout( __poWidgetPosition );
  __qFontData.setPixelSize( 20 );
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

  // ... ground course
  QWidget* __poWidgetGroundCourse = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutGroundCourse = new QVBoxLayout( __poWidgetGroundCourse );
  __qFontData.setPixelSize( 20 );
  poTextGroundBearing = new COverlayText( this );
  poTextGroundBearing->setToolTip( tr("Ground Bearing") );
  poTextGroundBearing->setFont( __qFontData );
  poTextGroundBearing->setIndent( 10 );
  poTextGroundBearing->setAlignment( Qt::AlignHCenter );
  poTextGroundBearing->resetText();
  __pqVBoxLayoutGroundCourse->addWidget( poTextGroundBearing );
  poTextGroundSpeed = new COverlayText( this );
  poTextGroundSpeed->setToolTip( tr("Ground Speed (horizontal)") );
  poTextGroundSpeed->setFont( __qFontData );
  poTextGroundSpeed->setIndent( 10 );
  poTextGroundSpeed->setAlignment( Qt::AlignHCenter );
  poTextGroundSpeed->resetText();
  __pqVBoxLayoutGroundCourse->addWidget( poTextGroundSpeed );
  poTextGroundSpeedVertical = new COverlayText( this );
  poTextGroundSpeedVertical->setToolTip( tr("Ground Speed (vertical)") );
  poTextGroundSpeedVertical->setFont( __qFontData );
  poTextGroundSpeedVertical->setIndent( 10 );
  poTextGroundSpeedVertical->setAlignment( Qt::AlignHCenter );
  poTextGroundSpeedVertical->resetText();
  __pqVBoxLayoutGroundCourse->addWidget( poTextGroundSpeedVertical, 1 );
  __poWidgetGroundCourse->setLayout( __pqVBoxLayoutGroundCourse );
  __poTabWidget->addTab( __poWidgetGroundCourse, tr("Ground") );

  // ... apparent course
  QWidget* __poWidgetApparentCourse = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutApparentCourse = new QVBoxLayout( __poWidgetApparentCourse );
  __qFontData.setPixelSize( 20 );
  poTextApparentBearing = new COverlayText( this );
  poTextApparentBearing->setToolTip( tr("Apparent Bearing") );
  poTextApparentBearing->setFont( __qFontData );
  poTextApparentBearing->setIndent( 10 );
  poTextApparentBearing->setAlignment( Qt::AlignHCenter );
  poTextApparentBearing->resetText();
  __pqVBoxLayoutApparentCourse->addWidget( poTextApparentBearing );
  poTextApparentSpeed = new COverlayText( this );
  poTextApparentSpeed->setToolTip( tr("Apparent Speed (horizontal)") );
  poTextApparentSpeed->setFont( __qFontData );
  poTextApparentSpeed->setIndent( 10 );
  poTextApparentSpeed->setAlignment( Qt::AlignHCenter );
  poTextApparentSpeed->resetText();
  __pqVBoxLayoutApparentCourse->addWidget( poTextApparentSpeed );
  poTextApparentSpeedVertical = new COverlayText( this );
  poTextApparentSpeedVertical->setToolTip( tr("Apparent Speed (vertical)") );
  poTextApparentSpeedVertical->setFont( __qFontData );
  poTextApparentSpeedVertical->setIndent( 10 );
  poTextApparentSpeedVertical->setAlignment( Qt::AlignHCenter );
  poTextApparentSpeedVertical->resetText();
  __pqVBoxLayoutApparentCourse->addWidget( poTextApparentSpeedVertical, 1 );
  __poWidgetApparentCourse->setLayout( __pqVBoxLayoutApparentCourse );
  __poTabWidget->addTab( __poWidgetApparentCourse, tr("Apparent") );

  // ... date/time
  QWidget* __poWidgetTime = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutTime = new QVBoxLayout( __poWidgetTime );
  poTextDate = new COverlayText( this );
  poTextDate->setToolTip( tr("Date") );
  poTextDate->setFont( __qFontData );
  poTextDate->setIndent( 10 );
  poTextDate->setAlignment( Qt::AlignHCenter );
  poTextDate->resetText();
  __pqVBoxLayoutTime->addWidget( poTextDate );
  poTextTime = new COverlayText( this );
  poTextTime->setToolTip( tr("Time") );
  poTextTime->setFont( __qFontData );
  poTextTime->setIndent( 10 );
  poTextTime->setAlignment( Qt::AlignHCenter );
  poTextTime->resetText();
  __pqVBoxLayoutTime->addWidget( poTextTime );
  poTextTime2 = new COverlayText( this );
  poTextTime2->setToolTip( tr("Time (alternate timezone)") );
  poTextTime2->setFont( __qFontData );
  poTextTime2->setIndent( 10 );
  poTextTime2->setAlignment( Qt::AlignHCenter );
  poTextTime2->resetText();
  __pqVBoxLayoutTime->addWidget( poTextTime2, 1 );
  __poWidgetTime->setLayout( __pqVBoxLayoutTime );
  __poTabWidget->addTab( __poWidgetTime, tr("Time") );

  // ... info
  QScrollArea* __poScrollAreaInfo = new QScrollArea( __poTabWidget );
  __poScrollAreaInfo->setStyleSheet( ".QScrollArea, .QWidget { BACKGROUND-COLOR: rgba(0, 0, 0, 0); BORDER: none; }" );
  __poScrollAreaInfo->setWidgetResizable( true );
  QWidget* __poWidgetInfo = new QWidget( __poScrollAreaInfo );
  QVBoxLayout* __pqVBoxLayoutInfo = new QVBoxLayout( __poWidgetInfo );
  __qFontData.setPixelSize( 12 );
  __qFontData.setBold( true );
  poTextType = new COverlayText( this );
  poTextType->setToolTip( tr("Type") );
  poTextType->setFont( __qFontData );
  poTextType->setIndent( 10 );
  poTextType->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextType );
  __qFontData.setBold( false );
  poTextDescription = new COverlayText( this );
  poTextDescription->setToolTip( tr("Description") );
  poTextDescription->setFont( __qFontData );
  poTextDescription->setIndent( 10 );
  poTextDescription->setWordWrap( true );
  poTextDescription->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextDescription );
  __qFontData.setPixelSize( 10 );
  poTextComment = new COverlayText( this );
  poTextComment->setToolTip( tr("Comment") );
  poTextComment->setFont( __qFontData );
  poTextComment->setIndent( 10 );
  poTextComment->setWordWrap( true );
  poTextComment->resetText();
  __pqVBoxLayoutInfo->addWidget( poTextComment );
  poUrl = new COverlayUrl( this );
  poUrl->setToolTip( tr("URL") );
  poUrl->setFont( __qFontData );
  poUrl->setIndent( 10 );
  poUrl->setWordWrap( true );
  poUrl->resetText();
  __pqVBoxLayoutInfo->addWidget( poUrl, 1 );
  __poWidgetInfo->setLayout( __pqVBoxLayoutInfo );
  __poScrollAreaInfo->setWidget( __poWidgetInfo );
  __poTabWidget->addTab( __poScrollAreaInfo, tr("Info") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonVisible, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenter, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonCenterLock, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonEdit, 0, Qt::AlignHCenter );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 0, Qt::AlignHCenter );
  // __pqHBoxLayoutButtons->addWidget( pqPushButtonAddLandmark, 1, Qt::AlignRight );
  // __pqHBoxLayoutButtons->addWidget( pqPushButtonAddRoute, 0, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonTrackRecord, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonAddDevice, 0, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CVesselPointDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CMainWindow* __poMainWindow = QVCTRuntime::useMainWindow();
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  poTextName->setText( __poVesselPoint->getName() );
  QString __qsSymbol = __poVesselPoint->getSymbol();
  if( __poMainWindow->symbolExists( __qsSymbol ) )
  {
    pqLabelSymbol->setPixmap( __poMainWindow->symbolPixmap( __qsSymbol ) );
    pqLabelSymbol->setVisible( true );
  }
  else
  {
    pqLabelSymbol->setVisible( false );
  }

  // ... position
  if( __poVesselPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED ) )
  {
    poTextLongitude->setText( CUnitPosition::toString( __poVesselPoint->getLongitude(), CUnitPosition::LONGITUDE ), !__poVesselPoint->isValidPosition() );
    poTextLatitude->setText( CUnitPosition::toString( __poVesselPoint->getLatitude(), CUnitPosition::LATITUDE ), !__poVesselPoint->isValidPosition() );
    if( __poVesselPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    {
      poTextElevation->setText( CUnitElevation::toString( __poVesselPoint->getElevation() ), !__poVesselPoint->isValidElevation() );
    }
    else
    {
      poTextElevation->resetText();
    }
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
    poTextElevation->resetText();
  }

  // ... course
  if( __poVesselPoint->GroundCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
    poTextGroundBearing->setText( CUnitBearing::toString( __poVesselPoint->GroundCourse.getBearing() ), !__poVesselPoint->GroundCourseValidity.isValidBearing() );
  else poTextGroundBearing->resetText();
  if( __poVesselPoint->GroundCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
    poTextGroundSpeed->setText( CUnitSpeed::toString( __poVesselPoint->GroundCourse.getSpeed() ), !__poVesselPoint->GroundCourseValidity.isValidSpeed() );
  else poTextGroundSpeed->resetText();
  if( __poVesselPoint->GroundCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
    poTextGroundSpeedVertical->setText( CUnitSpeedVertical::toString( __poVesselPoint->GroundCourse.getSpeedVertical() ), !__poVesselPoint->GroundCourseValidity.isValidSpeedVertical() );
  else poTextGroundSpeedVertical->resetText();
  if( __poVesselPoint->ApparentCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
    poTextApparentBearing->setText( CUnitBearing::toString( __poVesselPoint->ApparentCourse.getBearing() ), !__poVesselPoint->ApparentCourseValidity.isValidBearing() );
  else poTextApparentBearing->resetText();
  if( __poVesselPoint->ApparentCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
    poTextApparentSpeed->setText( CUnitSpeed::toString( __poVesselPoint->ApparentCourse.getSpeed() ), !__poVesselPoint->ApparentCourseValidity.isValidSpeed() );
  else poTextApparentSpeed->resetText();
  if( __poVesselPoint->ApparentCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
    poTextApparentSpeedVertical->setText( CUnitSpeedVertical::toString( __poVesselPoint->ApparentCourse.getSpeedVertical() ), !__poVesselPoint->ApparentCourseValidity.isValidSpeedVertical() );
  else poTextApparentSpeedVertical->resetText();

  // ... time
  if( __poVesselPoint->getTime() != CDataTime::UNDEFINED_TIME )
  {
    double __fdTime = __poVesselPoint->getTime();
    bool __bInvalid = !__poVesselPoint->isValidTime();
    poTextDate->setText( CUnitDate::toString( __fdTime ), __bInvalid );
    poTextTime->setText( CUnitTime::toString( __fdTime ), __bInvalid );
    poTextTime2->setText( "("+CUnitTime::toString( __fdTime, true )+")", __bInvalid );
  }
  else
  {
    poTextDate->resetText();
    poTextTime->resetText();
    poTextTime2->resetText();
  }

  // ... info
  poTextType->setText( __poVesselPoint->getType() );
  poTextDescription->setText( __poVesselPoint->getDescription() );
  poTextComment->setText( __poVesselPoint->getComment() );
  poUrl->setText( __poVesselPoint->getUrl() );

  // ... buttons
  bIgnoreUpdate = true;
  bool __bLocked = ( QVCTRuntime::useChartTable()->getVesselPointSynchronize() == __poVesselPoint );
  pqPushButtonCenterLock->setChecked( __bLocked );
  pqPushButtonCenterLock->setIcon( QIcon( __bLocked ? ":icons/32x32/center_lock.png" : ":icons/32x32/center_unlock.png" ) );
  bool __bDynamic = __poVesselPoint->isDynamic();
  pqPushButtonEdit->setEnabled( !__bDynamic );
  pqPushButtonAddDevice->setEnabled( !__bDynamic );
  bool __bTrackRecord = __poVesselPoint->isTrackRecording();
  pqPushButtonTrackRecord->setChecked( __bTrackRecord );
  pqPushButtonTrackRecord->setIcon( QIcon( __bTrackRecord ? ":icons/32x32/track_start.png" : ":icons/32x32/track_stop.png" ) );
  bIgnoreUpdate = false;
}

void CVesselPointDetailView::resetContent()
{
  poTextName->resetText();
  pqLabelSymbol->setVisible( false );
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
  poTextGroundBearing->resetText();
  poTextGroundSpeed->resetText();
  poTextGroundSpeedVertical->resetText();
  poTextApparentBearing->resetText();
  poTextApparentSpeed->resetText();
  poTextApparentSpeedVertical->resetText();
  poTextDate->resetText();
  poTextTime->resetText();
  poTextTime2->resetText();
  poTextType->resetText();
  poTextDescription->resetText();
  poTextComment->resetText();
  poUrl->resetText();
}

void CVesselPointDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonVisible->setEnabled( true );
    pqPushButtonCenter->setEnabled( true );
    pqPushButtonCenterLock->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    // pqPushButtonAddLandmark->setEnabled( true );
    // pqPushButtonAddRoute->setEnabled( true );
    pqPushButtonTrackRecord->setEnabled( true );
    bIgnoreUpdate = false;
  }
}

void CVesselPointDetailView::disableContent()
{
  pqPushButtonVisible->setEnabled( false );
  pqPushButtonCenter->setEnabled( false );
  pqPushButtonCenterLock->setEnabled( false );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  // pqPushButtonAddLandmark->setEnabled( false );
  // pqPushButtonAddRoute->setEnabled( false );
  pqPushButtonTrackRecord->setEnabled( false );
  pqPushButtonAddDevice->setEnabled( false );
  bIgnoreUpdate = true;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselPointDetailView::slotToggleVisible()
{
  if( !poOverlayObject ) return;
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  bool __bVisible = __poVesselPoint->isVisible();
  __poVesselPoint->toggleVisible();
  if( __bVisible != __poVesselPoint->isVisible() )
  {
    __poVesselPoint->setCheckState( CVesselOverlay::VISIBLE, __bVisible ? Qt::Unchecked : Qt::Checked );
  }
  __poVesselPoint->useOverlay()->forceRedraw();
  QVCTRuntime::usePointerOverlay()->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
}

void CVesselPointDetailView::slotPositionCenter()
{
  if( !poOverlayObject ) return;
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poVesselPoint );
  QVCTRuntime::useChartTable()->updateChart();
}

void CVesselPointDetailView::slotPositionCenterLock( bool _bLock )
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  if( _bLock )
  {
    pqPushButtonCenterLock->setIcon( QIcon( ":icons/32x32/center_lock.png" ) );
    QVCTRuntime::useChartTable()->setVesselPointSynchronize( __poVesselPoint );
    QVCTRuntime::useVesselPosition()->setVesselPoint( __poVesselPoint );
    QVCTRuntime::useVesselCourse()->setVesselPoint( __poVesselPoint );
    QVCTRuntime::useVesselTarget()->setVesselPoint( __poVesselPoint );
    QVCTRuntime::usePointerOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
  else if( QVCTRuntime::useChartTable()->getVesselPointSynchronize() == __poVesselPoint )
  {
    pqPushButtonCenterLock->setIcon( QIcon( ":icons/32x32/center_unlock.png" ) );
    QVCTRuntime::useChartTable()->resetVesselPointSynchronize();
    QVCTRuntime::useVesselPosition()->resetVesselPoint();
    QVCTRuntime::useVesselCourse()->resetVesselPoint();
    QVCTRuntime::useVesselTarget()->resetVesselPoint();
    QVCTRuntime::usePointerOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
}

void CVesselPointDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CVesselPoint*)poOverlayObject)->showEdit();
}

void CVesselPointDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  CVesselContainer* __poVesselContainer = (CVesselContainer*)((QTreeWidgetItem*)poOverlayObject)->parent();
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  __poVesselContainer->removeChild( __poVesselPoint );
  delete __poVesselPoint;
  QTreeWidgetItem* __pqTreeWidgetItem = __poVesselOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poVesselOverlay->showDetail( __pqTreeWidgetItem );
  __poVesselOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}

// void CVesselPointDetailView::slotAddLandmark()
// {
//   if( !poOverlayObject ) return;
//   CLandmarkOverlay* __poLandmarkOverlay = QVCTRuntime::useLandmarkOverlay();
//   CLandmarkContainer* __poLandmarkContainer = __poLandmarkOverlay->pickContainer();
//   if( !__poLandmarkContainer ) return;
//   QString __qsName = __poLandmarkContainer->newChildName( poOverlayObject->getName() );
//   CLandmarkPoint* __poLandmarkPoint = __poLandmarkContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject );
//   if( !__poLandmarkPoint ) return;
//   __poLandmarkOverlay->setCurrentItem( __poLandmarkPoint );
//   __poLandmarkOverlay->forceRedraw();
//   CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
//   CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
//   __poPointerPoint->resetPosition();
//   __poPointerOverlay->forceRedraw();
//   QVCTRuntime::useChartTable()->updateChart();
//   __poLandmarkPoint->showEdit();
// }

// void CVesselPointDetailView::slotAddRoute()
// {
//   if( !poOverlayObject ) return;
//   CRouteOverlay* __poRouteOverlay = QVCTRuntime::useRouteOverlay();
//   CRouteContainer* __poRouteContainer = __poRouteOverlay->pickContainer();
//   if( !__poRouteContainer ) return;
//   QString __qsName = __poRouteContainer->newChildName( poOverlayObject->getName() );
//   CRoutePoint* __poRoutePoint = __poRouteContainer->addPoint( __qsName, *(CPointerPoint*)poOverlayObject );
//   if( !__poRoutePoint ) return;
//   __poRouteOverlay->setCurrentItem( __poRoutePoint );
//   __poRouteOverlay->forceRedraw();
//   CPointerOverlay* __poPointerOverlay = QVCTRuntime::usePointerOverlay();
//   CPointerPoint* __poPointerPoint = __poPointerOverlay->usePointerPoint();
//   __poPointerPoint->resetPosition();
//   __poPointerOverlay->forceRedraw();
//   QVCTRuntime::useChartTable()->updateChart();
//   __poRoutePoint->showEdit();
// }

void CVesselPointDetailView::slotTrackRecord( bool _bTrackRecord )
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  bIgnoreUpdate = true;
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  _bTrackRecord = __poVesselPoint->setTrackRecord( _bTrackRecord );
  pqPushButtonTrackRecord->setChecked( _bTrackRecord );
  pqPushButtonTrackRecord->setIcon( QIcon( _bTrackRecord ? ":icons/32x32/track_start.png" : ":icons/32x32/track_stop.png" ) );
  bIgnoreUpdate = false;
}

void CVesselPointDetailView::slotAddDevice()
{
  if( !poOverlayObject ) return;
  CVesselPointDevice* __poVesselPointDevice = 0;
  CVesselPointDeviceCreateView* __poVesselPointDeviceCreateView = new CVesselPointDeviceCreateView( &__poVesselPointDevice );
  __poVesselPointDeviceCreateView->exec();
  if( __poVesselPointDevice )
  {
    ((CVesselPoint*)poOverlayObject)->addChild( __poVesselPointDevice );
    QVCTRuntime::useVesselOverlay()->setCurrentItem( __poVesselPointDevice );
    __poVesselPointDevice->showEdit();
    __poVesselPointDevice->connectDevice();
    __poVesselPointDevice->showDetail();
  }
  delete __poVesselPointDeviceCreateView;
}
