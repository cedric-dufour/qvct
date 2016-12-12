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
#include "overlays/vessel/device/CVesselPointDevice.hpp"
#include "overlays/vessel/device/CVesselPointDeviceDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointDeviceDetailView::CVesselPointDeviceDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
  , bIgnoreUpdate( true )
{
  constructLayout();
}

void CVesselPointDeviceDetailView::constructLayout()
{
  // Create the buttons
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "" );
  pqPushButtonEdit->setToolTip( tr("Edit this device") );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "" );
  pqPushButtonDelete->setToolTip( tr("Delete this device") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... connect (device)
  pqPushButtonConnect = new QPushButton( QIcon( ":icons/32x32/device_disconnect.png" ), "" );
  pqPushButtonConnect->setToolTip( tr("Connect to the actual device/source") );
  pqPushButtonConnect->setMaximumSize( 36, 34 );
  pqPushButtonConnect->setEnabled( false );
  pqPushButtonConnect->setCheckable( true );
  QWidget::connect( pqPushButtonConnect, SIGNAL( toggled(bool) ), this, SLOT( slotConnect(bool) ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_device.png" ) );
  __pqLabelIcon->setToolTip( tr("Device") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText();
  poTextName->setToolTip( tr("Name/Source") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget();
  __poTabWidget->setTabPosition( QTabWidget::South );

  // ... position
  QWidget* __poWidgetPosition = new QWidget();
  QVBoxLayout* __pqVBoxLayoutPosition = new QVBoxLayout();
  __qFontData.setPixelSize( 20 );
  poTextLongitude = new COverlayText();
  poTextLongitude->setToolTip( tr("Longitude & Error[m]") );
  poTextLongitude->setFont( __qFontData );
  poTextLongitude->setIndent( 10 );
  poTextLongitude->setAlignment( Qt::AlignHCenter );
  poTextLongitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLongitude );
  poTextLatitude = new COverlayText();
  poTextLatitude->setToolTip( tr("Latitude & Error[m]") );
  poTextLatitude->setFont( __qFontData );
  poTextLatitude->setIndent( 10 );
  poTextLatitude->setAlignment( Qt::AlignHCenter );
  poTextLatitude->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextLatitude );
  poTextElevation = new COverlayText();
  poTextElevation->setToolTip( tr("Elevation & Error[m]") );
  poTextElevation->setFont( __qFontData );
  poTextElevation->setIndent( 10 );
  poTextElevation->setAlignment( Qt::AlignHCenter );
  poTextElevation->resetText();
  __pqVBoxLayoutPosition->addWidget( poTextElevation, 1 );
  __poWidgetPosition->setLayout( __pqVBoxLayoutPosition );
  __poTabWidget->addTab( __poWidgetPosition, tr("Position") );

  // ... course
  QWidget* __poWidgetCourse = new QWidget();
  QVBoxLayout* __pqVBoxLayoutCourse = new QVBoxLayout();
  __qFontData.setPixelSize( 20 );
  poTextBearing = new COverlayText();
  poTextBearing->setToolTip( tr("Bearing & Error[deg]") );
  poTextBearing->setFont( __qFontData );
  poTextBearing->setIndent( 10 );
  poTextBearing->setAlignment( Qt::AlignHCenter );
  poTextBearing->resetText();
  __pqVBoxLayoutCourse->addWidget( poTextBearing );
  poTextSpeed = new COverlayText();
  poTextSpeed->setToolTip( tr("Speed (horizontal) & Error[m/s]") );
  poTextSpeed->setFont( __qFontData );
  poTextSpeed->setIndent( 10 );
  poTextSpeed->setAlignment( Qt::AlignHCenter );
  poTextSpeed->resetText();
  __pqVBoxLayoutCourse->addWidget( poTextSpeed );
  poTextSpeedVertical = new COverlayText();
  poTextSpeedVertical->setToolTip( tr("Speed (vertical) & Error[m/s]") );
  poTextSpeedVertical->setFont( __qFontData );
  poTextSpeedVertical->setIndent( 10 );
  poTextSpeedVertical->setAlignment( Qt::AlignHCenter );
  poTextSpeedVertical->resetText();
  __pqVBoxLayoutCourse->addWidget( poTextSpeedVertical, 1 );
  __poWidgetCourse->setLayout( __pqVBoxLayoutCourse );
  __poTabWidget->addTab( __poWidgetCourse, tr("Course") );

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
  poTextTime->setToolTip( tr("Time & Error[s]") );
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
  poTextSources = new COverlayText();
  poTextSources->setToolTip( tr("Sources Used / Visible") );
  poTextSources->setFont( __qFontData );
  poTextSources->setIndent( 10 );
  poTextSources->setAlignment( Qt::AlignHCenter );
  poTextSources->resetText();
  __pqVBoxLayoutPrecision->addWidget( poTextSources );
  poTextDop = new COverlayText();
  poTextDop->setToolTip( tr("HDOP / VDOP / TDOP") );
  poTextDop->setFont( __qFontData );
  poTextDop->setIndent( 10 );
  poTextDop->setAlignment( Qt::AlignHCenter );
  poTextDop->resetText();
  __pqVBoxLayoutPrecision->addWidget( poTextDop, 1 );
  __poWidgetPrecision->setLayout( __pqVBoxLayoutPrecision );
  __poTabWidget->addTab( __poWidgetPrecision, tr("Precision") );

  // ... text
  QScrollArea* __poScrollAreaText = new QScrollArea();
  __poScrollAreaText->setStyleSheet( ".QScrollArea, .QWidget { BACKGROUND-COLOR: rgba(0, 0, 0, 0); BORDER: none; }" );
  __poScrollAreaText->setWidgetResizable( true );
  QWidget* __poWidgetText = new QWidget();
  QVBoxLayout* __pqVBoxLayoutText = new QVBoxLayout();
  __qFontData.setPixelSize( 12 );
  poText = new COverlayText();
  poText->setToolTip( tr("Additional Textual Data") );
  poText->setFont( __qFontData );
  poText->setIndent( 10 );
  poText->setWordWrap( true );
  poText->resetText();
  __pqVBoxLayoutText->addWidget( poText, 1 );
  __poWidgetText->setLayout( __pqVBoxLayoutText );
  __poScrollAreaText->setWidget( __poWidgetText );
  __poTabWidget->addTab( __poScrollAreaText, tr("Text") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame();
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonEdit, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonConnect, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CVesselPointDeviceDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)poOverlayObject;

  // ... name
  poTextName->setText( __poVesselPointDevice->getName()+":"+__poVesselPointDevice->getSourceName() );

  // ... position
  if( __poVesselPointDevice->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
      && __poVesselPointDevice->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
  {
    QString __qsError = __poVesselPointDevice->getErrorHorizontal() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorHorizontal(), 'f', 1 )+")" : "(n/a)";
    poTextLongitude->setText( CUnitPosition::toString( __poVesselPointDevice->getLongitude(), CUnitPosition::LONGITUDE )+" "+__qsError );
    poTextLatitude->setText( CUnitPosition::toString( __poVesselPointDevice->getLatitude(), CUnitPosition::LATITUDE )+" "+__qsError );
  }
  else
  {
    poTextLongitude->resetText();
    poTextLatitude->resetText();
  }

  // ... elevation
  if( __poVesselPointDevice->getElevation() != CDataPosition::UNDEFINED_ELEVATION )
  {
    QString __qsError = __poVesselPointDevice->getErrorVertical() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorVertical(), 'f', 1 )+")" : "(n/a)";
    poTextElevation->setText( CUnitElevation::toString( __poVesselPointDevice->getElevation() )+" "+__qsError );
  }
  else
    poTextElevation->resetText();

  // ... course
  if( __poVesselPointDevice->getBearing() != CDataCourse::UNDEFINED_BEARING )
  {
    QString __qsError = __poVesselPointDevice->getErrorBearing() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorBearing(), 'f', 1 )+")" : "(n/a)";
    poTextBearing->setText( CUnitBearing::toString( __poVesselPointDevice->getBearing() )+" "+__qsError );
  }
  else poTextBearing->resetText();
  if( __poVesselPointDevice->getSpeed() != CDataCourse::UNDEFINED_SPEED )
  {
    QString __qsError = __poVesselPointDevice->getErrorSpeed() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorSpeed(), 'f', 1 )+")" : "(n/a)";
    poTextSpeed->setText( CUnitSpeed::toString( __poVesselPointDevice->getSpeed() )+" "+__qsError );
  }
  else poTextSpeed->resetText();
  if( __poVesselPointDevice->getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
  {
    QString __qsError = __poVesselPointDevice->getErrorSpeedVertical() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorSpeedVertical(), 'f', 1 )+")" : "(n/a)";
    poTextSpeedVertical->setText( CUnitSpeed::toString( __poVesselPointDevice->getSpeedVertical() )+" "+__qsError );
  }
  else poTextSpeedVertical->resetText();

  // ... time
  if( __poVesselPointDevice->getTime() != CDataTime::UNDEFINED_TIME )
  {
    double __fdTime = __poVesselPointDevice->getTime();
    QString __qsError = __poVesselPointDevice->getErrorTime() != CDataValidity::UNDEFINED_VALUE
      ? "("+QString::number( __poVesselPointDevice->getErrorTime(), 'f', 3 )+")" : "(n/a)";
    poTextDate->setText( CUnitDate::toString( __fdTime ) );
    poTextTime->setText( CUnitTime::toString( __fdTime )+" "+__qsError );
    poTextTime2->setText( "("+CUnitTime::toString( __fdTime, true )+")" );
  }
  else
  {
    poTextDate->resetText();
    poTextTime->resetText();
    poTextTime2->resetText();
  }

  // ... precision
  poTextFixType->setText( __poVesselPointDevice->getTypeString() );
  if( __poVesselPointDevice->getSourcesUsed() || __poVesselPointDevice->getSourcesSeen() )
    poTextSources->setText( QString::number( __poVesselPointDevice->getSourcesUsed() )+" / "+QString::number( __poVesselPointDevice->getSourcesSeen() ) );
  else
    poTextSources->resetText();
  {
    double __fdDopHorizontal = __poVesselPointDevice->getDopHorizontal();
    double __fdDopVertical = __poVesselPointDevice->getDopVertical();
    double __fdDopTime = __poVesselPointDevice->getDopTime();
    if( __fdDopHorizontal != CDeviceDataDop::UNDEFINED_VALUE
        || __fdDopVertical != CDeviceDataDop::UNDEFINED_VALUE
        || __fdDopTime != CDeviceDataDop::UNDEFINED_VALUE )
      poTextDop->setText( ( __fdDopHorizontal != CDeviceDataDop::UNDEFINED_VALUE ? QString::number( __fdDopHorizontal, 'f', 1 ) : poTextDop->emptyString() )
                          +" / "+( __fdDopVertical != CDeviceDataDop::UNDEFINED_VALUE ? QString::number( __fdDopVertical, 'f', 1 ) : poTextDop->emptyString() )
                          +" / "+( __fdDopTime != CDeviceDataDop::UNDEFINED_VALUE ? QString::number( __fdDopTime, 'f', 1 ) : poTextDop->emptyString() ) );
    else
      poTextDop->resetText();
  }

  // ... text
  poText->setText( __poVesselPointDevice->getText() );

  // ... buttons
  bIgnoreUpdate = true;
  bool __bDynamic = __poVesselPointDevice->isDynamic();
  pqPushButtonEdit->setEnabled( !__bDynamic );
  pqPushButtonDelete->setEnabled( !__bDynamic );
  bool __bConnected = __poVesselPointDevice->isDeviceConnected();
  pqPushButtonConnect->setChecked( __bConnected );
  pqPushButtonConnect->setIcon( QIcon( __bConnected ? ":icons/32x32/device_connect.png" : ":icons/32x32/device_disconnect.png" ) );
  bIgnoreUpdate = false;
}

void CVesselPointDeviceDetailView::resetContent()
{
  poTextName->resetText();
  poTextLongitude->resetText();
  poTextLatitude->resetText();
  poTextElevation->resetText();
  poTextBearing->resetText();
  poTextSpeed->resetText();
  poTextSpeedVertical->resetText();
  poTextDate->resetText();
  poTextTime->resetText();
  poTextTime2->resetText();
  poTextFixType->resetText();
  poTextSources->resetText();
  poTextDop->resetText();
  poText->resetText();
}

void CVesselPointDeviceDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonConnect->setEnabled( true );
    QObject::connect( (CVesselPointDevice*)poOverlayObject, SIGNAL( signalRefreshContent() ), this, SLOT( slotRefreshContent() ) );
    bIgnoreUpdate = false;
  }
}

void CVesselPointDeviceDetailView::disableContent()
{
  if( poOverlayObject ) QObject::disconnect( (CVesselPointDevice*)poOverlayObject, 0, this, 0 );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonConnect->setEnabled( false );
  bIgnoreUpdate = true;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselPointDeviceDetailView::slotRefreshContent()
{
  refreshContent();
}

void CVesselPointDeviceDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CVesselPointDevice*)poOverlayObject)->showEdit();
}

void CVesselPointDeviceDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  CVesselPoint* __poVesselPoint = (CVesselPoint*)((QTreeWidgetItem*)poOverlayObject)->parent();
  CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)poOverlayObject;
  __poVesselPoint->removeChild( __poVesselPointDevice );
  delete __poVesselPointDevice;
  QTreeWidgetItem* __pqTreeWidgetItem = __poVesselOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poVesselOverlay->showDetail( __pqTreeWidgetItem );
  __poVesselOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
  __pqMutexDataChange->unlock();
}

void CVesselPointDeviceDetailView::slotConnect( bool _bConnect )
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)poOverlayObject;
  if( _bConnect ) __poVesselPointDevice->connectDevice();
  else __poVesselPointDevice->disconnectDevice();
  refreshContent();
}
