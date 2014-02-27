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
#include <QTabWidget>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/COverlayText.hpp"
#include "devices/CDevice.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceDetailView::CDeviceDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
  , bIgnoreUpdate( true )
{
  constructLayout();
}

void CDeviceDetailView::constructLayout()
{
  // Create the buttons
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "", this );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setToolTip( tr("Edit this device's configuration") );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setToolTip( tr("Delete this device") );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... stop
  pqPushButtonStop = new QPushButton( QIcon( ":icons/32x32/stop.png" ), "", this );
  pqPushButtonStop->setMaximumSize( 36, 34 );
  pqPushButtonStop->setToolTip( tr("Stop this device") );
  pqPushButtonStop->setCheckable( true );
  pqPushButtonStop->setEnabled( false );
  QWidget::connect( pqPushButtonStop, SIGNAL( clicked() ), this, SLOT( slotStop() ) );
  // ... pause
  pqPushButtonPause = new QPushButton( QIcon( ":icons/32x32/pause.png" ), "", this );
  pqPushButtonPause->setMaximumSize( 36, 34 );
  pqPushButtonPause->setToolTip( tr("Pause this device") );
  pqPushButtonPause->setCheckable( true );
  pqPushButtonPause->setEnabled( false );
  QWidget::connect( pqPushButtonPause, SIGNAL( clicked() ), this, SLOT( slotPause() ) );
  // ... start
  pqPushButtonStart = new QPushButton( QIcon( ":icons/32x32/start.png" ), "", this );
  pqPushButtonStart->setMaximumSize( 36, 34 );
  pqPushButtonStart->setToolTip( tr("Start this device") );
  pqPushButtonStart->setCheckable( true );
  pqPushButtonStart->setEnabled( false );
  QWidget::connect( pqPushButtonStart, SIGNAL( clicked() ), this, SLOT( slotStart() ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/device.png" ) );
  __pqLabelIcon->setToolTip( tr("Device") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Name") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget( this );
  __poTabWidget->setTabPosition( QTabWidget::South );
  __poTabWidget->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

  // ... summary
  QWidget* __poWidgetSummary = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutSummary = new QVBoxLayout( __poWidgetSummary );
  __qFontData.setPixelSize( 20 );
  poTextDriver = new COverlayText( this );
  poTextDriver->setToolTip( tr("Driver") );
  poTextDriver->setFont( __qFontData );
  poTextDriver->setIndent( 10 );
  poTextDriver->setAlignment( Qt::AlignHCenter );
  poTextDriver->resetText();
  __pqVBoxLayoutSummary->addWidget( poTextDriver, 0 );
  pqLabelActivity = new COverlayText( this );
  pqLabelActivity->setToolTip( tr("Activity") );
  pqLabelActivity->setFont( __qFontData );
  pqLabelActivity->setIndent( 10 );
  pqLabelActivity->setAlignment( Qt::AlignHCenter );
  __pqVBoxLayoutSummary->addWidget( pqLabelActivity, 1 );
  __poWidgetSummary->setLayout( __pqVBoxLayoutSummary );
  __poTabWidget->addTab( __poWidgetSummary, tr("Summary") );

  // ... [end]
  __pqVBoxLayout->addWidget( __poTabWidget, 1 );

  // Add separator
  QFrame* __pqFrameSeparator = new QFrame( this );
  __pqFrameSeparator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  __pqVBoxLayout->addWidget( __pqFrameSeparator );

  // Add buttons
  QHBoxLayout* __pqHBoxLayoutButtons = new QHBoxLayout();
  __pqHBoxLayoutButtons->addWidget( pqPushButtonEdit, 0, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonDelete, 1, Qt::AlignLeft );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonStop, 1, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonPause, 0, Qt::AlignRight );
  __pqHBoxLayoutButtons->addWidget( pqPushButtonStart, 0, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CDeviceDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CDevice* __poDevice = (CDevice*)poOverlayObject;
  poTextName->setText( __poDevice->getName() );
  poTextDriver->setText( CDeviceDriver::toName( __poDevice->getDriver() ) );
  pqLabelActivity->setText( "" );

  // ... buttons
  bIgnoreUpdate = true;
  pqPushButtonStop->setChecked( __poDevice->getOperatingMode() == CDevice::STOP );
  pqPushButtonPause->setChecked( __poDevice->getOperatingMode() == CDevice::PAUSE );
  pqPushButtonStart->setChecked( __poDevice->getOperatingMode() == CDevice::START );
  bIgnoreUpdate = false;
}

void CDeviceDetailView::resetContent()
{
  poTextName->resetText();
  poTextDriver->resetText();
  pqLabelActivity->setText( "" );
}

void CDeviceDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonStop->setEnabled( true );
    pqPushButtonPause->setEnabled( true );
    pqPushButtonStart->setEnabled( true );
    QObject::connect( (CDevice*)poOverlayObject, SIGNAL( signalOperatingMode(CDevice::EOperatingMode) ), this, SLOT( slotOperatingMode(CDevice::EOperatingMode) ) );
    QObject::connect( (CDevice*)poOverlayObject, SIGNAL( signalActivity() ), this, SLOT( slotActivity() ) );
    bIgnoreUpdate = false;
  }
}

void CDeviceDetailView::disableContent()
{
  if( poOverlayObject ) QObject::disconnect( (CDevice*)poOverlayObject, 0, this, 0 );
  pqPushButtonEdit->setEnabled( false );
  pqPushButtonDelete->setEnabled( false );
  pqPushButtonStop->setEnabled( false );
  pqPushButtonPause->setEnabled( false );
  pqPushButtonStart->setEnabled( false );
  bIgnoreUpdate = true;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CDeviceDetailView::slotOperatingMode( CDevice::EOperatingMode _eOperatingMode )
{
  pqPushButtonStop->setChecked( _eOperatingMode == CDevice::STOP );
  pqPushButtonPause->setChecked( _eOperatingMode == CDevice::PAUSE );
  pqPushButtonStart->setChecked( _eOperatingMode == CDevice::START );
}

void CDeviceDetailView::slotActivity()
{
  static double __fdTimestampLast = 0;
  double __fdTimestampNow = microtime();
  if( __fdTimestampNow - __fdTimestampLast < 0.1 ) return;
  pqLabelActivity->setText( pqLabelActivity->text() == "\\" ? "/" : "\\" );
  __fdTimestampLast = __fdTimestampNow;
}

void CDeviceDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CDevice*)poOverlayObject)->showEdit();
}

void CDeviceDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  CDevice* __poDevice = (CDevice*)poOverlayObject;
  __poDeviceOverlay->removeChild( __poDevice );
  delete __poDevice;
  QTreeWidgetItem* __pqTreeWidgetItem = __poDeviceOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poDeviceOverlay->showDetail( __pqTreeWidgetItem );
  __pqMutexDataChange->unlock();
}

void CDeviceDetailView::slotStop()
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  ((CDevice*)poOverlayObject)->setOperatingMode( CDevice::STOP );
  refreshContent();
}

void CDeviceDetailView::slotPause()
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  ((CDevice*)poOverlayObject)->setOperatingMode( CDevice::PAUSE );
  refreshContent();
}

void CDeviceDetailView::slotStart()
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  ((CDevice*)poOverlayObject)->setOperatingMode( CDevice::START );
  refreshContent();
}
