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
#include <QDomElement> // QtXml module
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/data/CDeviceDataFix.hpp"
#include "devices/data/CDeviceDataSkyView.hpp"
#include "devices/drivers/Sample/CDeviceSample.hpp"
#include "devices/drivers/Sample/CDeviceSampleEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceSample::CDeviceSample( const QString& _rqsName )
  : CDevice( _rqsName )
  , qsHost( "127.0.0.1" )
  , iPort( 12345 )
{}

CDeviceSample::~CDeviceSample()
{
  // TODO: stop the device
  // stop();
}


//------------------------------------------------------------------------------
// METHODS: CDevice (implement/override)
//------------------------------------------------------------------------------

QVCT::EStatus CDeviceSample::setOperatingMode( CDevice::EOperatingMode _eOperatingMode )
{
  QVCT::EStatus __eStatus = QVCT::CRITICAL;
  switch( _eOperatingMode )
  {

  case CDevice::STOP:
    // TODO: stop the device
    // __eStatus = stop();
    break;

  case CDevice::PAUSE:
    // TODO: pause the device
    // __eStatus = pause();
    break;

  case CDevice::START:
    // TODO: start the device
    // __eStatus = start();
    break;

  default:;
  }
  if( __eStatus != QVCT::OK )
  {
    qCritical( "ERROR[%s]: Failed to switch operating mode; host=%s, port=%d", Q_FUNC_INFO, qPrintable( qsHost ), iPort );
  }
  return __eStatus;
}

void CDeviceSample::showDetail()
{
  QVCTRuntime::useDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::DEVICE );
}

void CDeviceSample::showEdit()
{
  CDeviceSampleEditView* __poDeviceSampleEditView = new CDeviceSampleEditView( this );
  if( __poDeviceSampleEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poDeviceSampleEditView;
}

void CDeviceSample::parseQVCT( const QDomElement& _rqDomElement )
{
  QDomElement __qDomElement = _rqDomElement.firstChildElement( "Configuration" );
  if( __qDomElement.isNull() ) return;
  qsHost = __qDomElement.attribute( "host", "127.0.0.1" );
  iPort = __qDomElement.attribute( "port", "12345" ).toInt();
}

void CDeviceSample::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // Device
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... driver
  _rqXmlStreamWriter.writeAttribute( "name", qsName );
  _rqXmlStreamWriter.writeAttribute( "driver", "sample" ); // MUST match CDeviceDriverCodes
  // ... configuration
  _rqXmlStreamWriter.writeStartElement( "Configuration" );
  _rqXmlStreamWriter.writeAttribute( "host", qsHost );
  _rqXmlStreamWriter.writeAttribute( "port", QString::number( iPort ) );
  _rqXmlStreamWriter.writeEndElement(); // Configuration
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Device
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// Methods to process the device data and send corresponding CDeviceDataFix
// and/or CDeviceDataSkyView
//

/// Slots to process device data
void CDeviceSample::slotProcessData( int )
{
  //qDebug( "DEBUG[%s]: Begin", Q_FUNC_INFO );

  // Prevent application data deletion (and potential segfaults) while processing device data
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();

  // emit signalActivity();

  // CDeviceDataFix __oDeviceDataFix( __qsSourceName );
  // ...
  // emit signalDataFix( __oDeviceDataFix );

  // CDeviceDataSkyView __oDeviceDataSkyView( __qsSourceName );
  // ...
  // emit signalDataSkyView( __oDeviceDataSkyView );

  // Release application data deletion lock
  __pqMutexDataChange->unlock();

  //qDebug( "DEBUG[%s]: End", Q_FUNC_INFO );
}
