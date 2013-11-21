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
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceDetailView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainerDeviceDetailView::CVesselContainerDeviceDetailView( QWidget* _pqParent )
  : COverlayObjectDetailView( _pqParent )
  , bIgnoreUpdate( true )
{
  constructLayout();
}

void CVesselContainerDeviceDetailView::constructLayout()
{
  // Create the buttons
  // ... edit
  pqPushButtonEdit = new QPushButton( QIcon( ":icons/32x32/edit.png" ), "", this );
  pqPushButtonEdit->setToolTip( tr("Edit this device") );
  pqPushButtonEdit->setMaximumSize( 36, 34 );
  pqPushButtonEdit->setEnabled( false );
  QWidget::connect( pqPushButtonEdit, SIGNAL( clicked() ), this, SLOT( slotEdit() ) );
  // ... delete
  pqPushButtonDelete = new QPushButton( QIcon( ":icons/32x32/delete.png" ), "", this );
  pqPushButtonDelete->setToolTip( tr("Delete this device") );
  pqPushButtonDelete->setMaximumSize( 36, 34 );
  pqPushButtonDelete->setEnabled( false );
  QWidget::connect( pqPushButtonDelete, SIGNAL( clicked() ), this, SLOT( slotDelete() ) );
  // ... connect (device)
  pqPushButtonConnect = new QPushButton( QIcon( ":icons/32x32/device_disconnect.png" ), "", this );
  pqPushButtonConnect->setToolTip( tr("Connect to the actual device/source") );
  pqPushButtonConnect->setMaximumSize( 36, 34 );
  pqPushButtonConnect->setEnabled( false );
  pqPushButtonConnect->setCheckable( true );
  QWidget::connect( pqPushButtonConnect, SIGNAL( toggled(bool) ), this, SLOT( slotConnect(bool) ) );

  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_device.png" ) );
  __pqLabelIcon->setToolTip( tr("Device") );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  poTextName = new COverlayText( this );
  poTextName->setToolTip( tr("Name/Source") );
  poTextName->setFont( __qFontHeader );
  poTextName->setWordWrap( true );
  __pqHBoxLayoutHeader->addWidget( poTextName, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  QFont __qFontData;
  QTabWidget* __poTabWidget = new QTabWidget( this );
  __poTabWidget->setTabPosition( QTabWidget::South );

  // ... device
  QWidget* __poWidgetText = new QWidget( __poTabWidget );
  QVBoxLayout* __pqVBoxLayoutText = new QVBoxLayout( __poWidgetText );
  __qFontData.setPixelSize( 12 );
  QLabel* __pqLabel = new QLabel( this );
  __pqLabel->setToolTip( tr("Dynamic Flotilla Device") );
  __pqLabel->setFont( __qFontData );
  __pqLabel->setIndent( 10 );
  __pqLabel->setWordWrap( true );
  __pqLabel->setText( tr("This device is used to dynamically create vessels as they appear \"on the wire\"") );
  __pqVBoxLayoutText->addWidget( __pqLabel );
  __poWidgetText->setLayout( __pqVBoxLayoutText );
  __poTabWidget->addTab( __poWidgetText, tr("Device") );

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
  __pqHBoxLayoutButtons->addWidget( pqPushButtonConnect, 1, Qt::AlignRight );
  __pqVBoxLayout->addLayout( __pqHBoxLayoutButtons );

  // Set the layout
  COverlayObjectDetailView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: COverlayObjectDetailView (implement/override)
//------------------------------------------------------------------------------

void CVesselContainerDeviceDetailView::refreshContent()
{
  if( !poOverlayObject ) return;
  CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)poOverlayObject;
  poTextName->setText( __poVesselContainerDevice->getName() + " (TTL:"+QString::number(__poVesselContainerDevice->getTTL())+")" );

  // ... buttons
  bIgnoreUpdate = true;
  bool __bConnected = __poVesselContainerDevice->isDeviceConnected();
  pqPushButtonConnect->setChecked( __bConnected );
  pqPushButtonConnect->setIcon( QIcon( __bConnected ? ":icons/32x32/device_connect.png" : ":icons/32x32/device_disconnect.png" ) );
  bIgnoreUpdate = false;
}

void CVesselContainerDeviceDetailView::resetContent()
{
  poTextName->resetText();
}

void CVesselContainerDeviceDetailView::enableContent()
{
  if( poOverlayObject )
  {
    pqPushButtonEdit->setEnabled( true );
    pqPushButtonDelete->setEnabled( true );
    pqPushButtonConnect->setEnabled( true );
    QObject::connect( (CVesselContainerDevice*)poOverlayObject, SIGNAL( signalRefreshContent() ), this, SLOT( slotRefreshContent() ) );
    bIgnoreUpdate = false;
  }
}

void CVesselContainerDeviceDetailView::disableContent()
{
  if( poOverlayObject ) QObject::disconnect( (CVesselContainerDevice*)poOverlayObject, 0, this, 0 );
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

void CVesselContainerDeviceDetailView::slotRefreshContent()
{
  refreshContent();
}

void CVesselContainerDeviceDetailView::slotEdit()
{
  if( !poOverlayObject ) return;
  ((CVesselContainerDevice*)poOverlayObject)->showEdit();
}

void CVesselContainerDeviceDetailView::slotDelete()
{
  if( !poOverlayObject ) return;
  if( !QVCTRuntime::useMainWindow()->deleteConfirm( poOverlayObject->getName() ) ) return;
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  CVesselOverlay* __poVesselOverlay = QVCTRuntime::useVesselOverlay();
  CVesselContainer* __poVesselContainer = (CVesselContainer*)((QTreeWidgetItem*)poOverlayObject)->parent();
  __poVesselContainer->clearDevice();
  QTreeWidgetItem* __pqTreeWidgetItem = __poVesselOverlay->currentItem();
  if( __pqTreeWidgetItem ) __poVesselOverlay->showDetail( __pqTreeWidgetItem );
   __poVesselOverlay->forceRedraw();
  QVCTRuntime::useChartTable()->updateChart();
 __pqMutexDataChange->unlock();
}

void CVesselContainerDeviceDetailView::slotConnect( bool _bConnect )
{
  if( !poOverlayObject || bIgnoreUpdate ) return;
  CVesselContainerDevice* __poVesselContainerDevice = (CVesselContainerDevice*)poOverlayObject;
  if( _bConnect ) __poVesselContainerDevice->connectDevice();
  else __poVesselContainerDevice->disconnectDevice();
  refreshContent();
}
