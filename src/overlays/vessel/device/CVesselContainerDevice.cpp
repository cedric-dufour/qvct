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

// C/C++
#include <cmath>

// QT
#include <QDateTime>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/CDevice.hpp"
#include "overlays/vessel/CVesselContainer.hpp"
#include "overlays/vessel/device/CVesselContainerDevice.hpp"
#include "overlays/vessel/device/CVesselContainerDeviceEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselContainerDevice::CVesselContainerDevice( const QString& _rqsName )
  : COverlayItem( COverlayObject::SUBITEM1, _rqsName )
  , poDevice( 0 )
  , iTTL( 60 )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
  QTreeWidgetItem::setText( CVesselOverlay::NAME, getName() );
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CVesselContainerDevice::showDetail()
{
  QVCTRuntime::useVesselContainerDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useVesselContainerDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::VESSEL_CONTAINER_DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::VESSEL );
}

void CVesselContainerDevice::showEdit()
{
  CVesselContainerDeviceEditView* __poVesselContainerDeviceEditView = new CVesselContainerDeviceEditView( this );
  if( __poVesselContainerDeviceEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poVesselContainerDeviceEditView;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselContainerDevice::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poDevice != _pqObject ) return;
  disconnectDevice();
}

void CVesselContainerDevice::slotDataFix( const CDeviceDataFix& _roDeviceDataFix )
{
  if( _roDeviceDataFix.getSourceName().isEmpty() ) return;
  CVesselContainer* __poVesselContainer = (CVesselContainer*)QTreeWidgetItem::parent();
  __poVesselContainer->addPointDynamic( _roDeviceDataFix.getSourceName(), COverlayObject::getName() );
}

//
// GETTERS
//

bool CVesselContainerDevice::isDeviceConnected() const
{
  return( poDevice && ((CVesselContainer*)QTreeWidgetItem::parent())->isDeviceConnected() );
};

//
// OTHER
//

bool CVesselContainerDevice::connectDevice()
{
  if( !poDevice )
  {
    CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
    CDevice* __poDevice = __poDeviceOverlay->pickDevice( COverlayObject::getName() );
    if( !__poDevice ) return false;
    poDevice = __poDevice;
    ((CVesselContainer*)QTreeWidgetItem::parent())->connectDevice();
    QObject::connect( poDevice, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
    QObject::connect( poDevice, SIGNAL( signalDataFix(const CDeviceDataFix&) ), this, SLOT( slotDataFix(const CDeviceDataFix&) ) );
  }
  return true;
}

void CVesselContainerDevice::disconnectDevice()
{
  if( !poDevice ) return;
  QObject::disconnect( poDevice, 0, this, 0 );
  ((CVesselContainer*)QTreeWidgetItem::parent())->disconnectDevice();
  poDevice = 0;
}

void CVesselContainerDevice::parseQVCT( const QDomElement& _rqDomElement )
{
  iTTL = _rqDomElement.attribute( "ttl", "60" ).toInt();
  if( iTTL < 5 ) iTTL = 5;
}

void CVesselContainerDevice::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{

  // Data
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... name
  if( !getName().isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", getName() );
  // ... (vessels) time-to-live
  _rqXmlStreamWriter.writeAttribute( "ttl", QString::number( iTTL ) );
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Device
}
