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
#include <QString>
#include <QXmlStreamWriter>

// GPSD
#include "gps.h"
// #include "gpsdclient.h"

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/data/CDeviceDataFix.hpp"
#include "devices/data/CDeviceDataSkyView.hpp"
#include "devices/drivers/GpsdAis/CDeviceGpsdAis.hpp"
#include "devices/drivers/GpsdAis/CDeviceGpsdAisEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceGpsdAis::CDeviceGpsdAis( const QString& _rqsName )
  : CDevice( _rqsName )
  , qsHost( "127.0.0.1" )
  , iPort( 2947 )
  , psGpsData( 0 )
  , pqSocketNotifier( 0 )
  , bPaused( false )
{}

CDeviceGpsdAis::~CDeviceGpsdAis()
{
  stop();
}


//------------------------------------------------------------------------------
// METHODS: CDevice (implement/override)
//------------------------------------------------------------------------------

QVCT::EStatus CDeviceGpsdAis::setOperatingMode( CDevice::EOperatingMode _eOperatingMode )
{
  QVCT::EStatus __eStatus = QVCT::CRITICAL;
  switch( _eOperatingMode )
  {

  case CDevice::STOP:
    __eStatus = stop();
    break;

  case CDevice::PAUSE:
    __eStatus = pause();
    break;

  case CDevice::START:
    __eStatus = start();
    break;

  default:; // WHAT THE F*** !?!
  }
  if( __eStatus != QVCT::OK )
  {
    qCritical( "ERROR[%s]: Failed to switch operating mode; host=%s, port=%d", Q_FUNC_INFO, qPrintable( qsHost ), iPort );
  }
  return __eStatus;
}

void CDeviceGpsdAis::showDetail()
{
  QVCTRuntime::useDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::DEVICE );
}

void CDeviceGpsdAis::showEdit()
{
  CDeviceGpsdAisEditView* __poDeviceGpsdAisEditView = new CDeviceGpsdAisEditView( this );
  if( __poDeviceGpsdAisEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poDeviceGpsdAisEditView;
}

void CDeviceGpsdAis::parseQVCT( const QDomElement& _rqDomElement )
{
  QDomElement __qDomElement = _rqDomElement.firstChildElement( "Configuration" );
  if( __qDomElement.isNull() ) return;
  qsHost = __qDomElement.attribute( "host", "127.0.0.1" );
  iPort = __qDomElement.attribute( "port", "2947" ).toInt();
}

void CDeviceGpsdAis::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // Device
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... driver
  _rqXmlStreamWriter.writeAttribute( "name", qsName );
  _rqXmlStreamWriter.writeAttribute( "driver", "gpsd_ais" );
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
// SLOTS
//
void CDeviceGpsdAis::slotProcessData( int )
{
  //qDebug( "DEBUG[%s]: Begin", Q_FUNC_INFO );
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();
  pqSocketNotifier->setEnabled( false );

  do // data-processing loop
  {
    //qDebug( "DEBUG[%s]: GPS data are waiting to be read", Q_FUNC_INFO );

    // Retrieve data
    int __iStatus;
#if GPSD_API_MAJOR_VERSION >= 5
    __iStatus = gps_read( psGpsData );
#else
    __iStatus = gps_poll( psGpsData );
#endif
    if( __iStatus == 0 ) break; // No data available
    if( __iStatus < 0 )
    {
      qCritical( "ERROR[%s]: Failed to read data from device; status=%d", Q_FUNC_INFO, __iStatus );
      emit signalError( QString( tr("Failed to read data from device")+"; status=%1" ).arg( __iStatus ) );
      stop();
      return;
    }
    //qDebug( "DEBUG[%s]: GPS data successfully read", Q_FUNC_INFO );
    //qDebug( "DEBUG[%s]: SET=%s", Q_FUNC_INFO, qPrintable( QString::number( psGpsData->set, 16 ) ) );

    // Do not process data if paused
    if( bPaused ) continue;

    // Check data status
    if( !( psGpsData->set & PACKET_SET ) ) continue; // We need a valid packet
    emit signalActivity();

    // Check AIS data status
    if( !( psGpsData->set & AIS_SET ) ) continue; // We need a valid AIS packet
    if( !psGpsData->ais.mmsi ) continue; // We need a valid MMSI number
    QString __qsSource = QString::number( psGpsData->ais.mmsi );
    //qDebug( "DEBUG[%s]: GPS data are available from source %s", Q_FUNC_INFO, qPrintable( __qsSource ) );

    // Parse AIS data
    // NOTE: we handle only message types: 1, 2, 3, 5, 18, 19, 24
    CDeviceDataFix __oDeviceDataFix( __qsSource );
    __oDeviceDataFix.setSourceType( CDeviceDataSource::AIS );
    __oDeviceDataFix.setCourseFromPosition( false );
    bool __bDataAvailable = false;
    int __iSecond = AIS_SEC_NOT_AVAILABLE, __iLongitude = AIS_LON_NOT_AVAILABLE, __iLatitude = AIS_LAT_NOT_AVAILABLE;
    int __iCourse = AIS_COURSE_NOT_AVAILABLE, __iSpeed = AIS_SPEED_NOT_AVAILABLE;
    QString __qsShipname;
    QString __qsCallsign;
    switch( psGpsData->ais.type )
    {
    case 1:
    case 2:
    case 3:
      if( psGpsData->ais.type1.second < 60 ) __iSecond = psGpsData->ais.type1.second;
      if( psGpsData->ais.type1.lon != AIS_LON_NOT_AVAILABLE ) __iLongitude = psGpsData->ais.type1.lon;
      if( psGpsData->ais.type1.lat != AIS_LAT_NOT_AVAILABLE ) __iLatitude = psGpsData->ais.type1.lat;
      if( psGpsData->ais.type1.course < 3600 ) __iCourse = psGpsData->ais.type1.course;
      if( psGpsData->ais.type1.speed < 1000 ) __iSpeed = psGpsData->ais.type1.speed;
      break;

    case 5:
      __qsShipname =  psGpsData->ais.type5.shipname;
      __qsCallsign =  psGpsData->ais.type5.callsign;
      break;

    case 18:
      if( psGpsData->ais.type18.second < 60 ) __iSecond = psGpsData->ais.type18.second;
      if( psGpsData->ais.type18.lon != AIS_LON_NOT_AVAILABLE ) __iLongitude = psGpsData->ais.type18.lon;
      if( psGpsData->ais.type18.lat != AIS_LAT_NOT_AVAILABLE ) __iLatitude = psGpsData->ais.type18.lat;
      if( psGpsData->ais.type18.course < 3600 ) __iCourse = psGpsData->ais.type18.course;
      if( psGpsData->ais.type18.speed < 1000 ) __iSpeed = psGpsData->ais.type18.speed;
      break;

    case 19:
      if( psGpsData->ais.type19.second < 60 ) __iSecond = psGpsData->ais.type19.second;
      if( psGpsData->ais.type19.lon != AIS_LON_NOT_AVAILABLE ) __iLongitude = psGpsData->ais.type19.lon;
      if( psGpsData->ais.type19.lat != AIS_LAT_NOT_AVAILABLE ) __iLatitude = psGpsData->ais.type19.lat;
      if( psGpsData->ais.type19.course < 3600 ) __iCourse = psGpsData->ais.type19.course;
      if( psGpsData->ais.type19.speed < 1000 ) __iSpeed = psGpsData->ais.type19.speed;
      break;

    case 24:
      __qsShipname =  psGpsData->ais.type24.shipname;
      __qsCallsign =  psGpsData->ais.type24.callsign;
      break;

    default:
      continue;

    }

    // ... time
    if( __iSecond != AIS_SEC_NOT_AVAILABLE )
    {
      uint __iCurrentTime = QDateTime::currentDateTime().toTime_t();
      int __iCurrentSecond = __iCurrentTime % 60;
      __iCurrentTime -= __iCurrentSecond;
      __iCurrentTime += __iSecond;
      if( __iSecond > 45 && __iCurrentSecond < 15 ) __iCurrentTime -= 60;
      __oDeviceDataFix.setTime( (double)__iCurrentTime );
      __bDataAvailable = true;
    }

    // ... position
    if( __iLongitude != AIS_LON_NOT_AVAILABLE && __iLatitude != AIS_LAT_NOT_AVAILABLE )
    {
      __oDeviceDataFix.setPosition( (double)__iLongitude / 600000.0, (double)__iLatitude / 600000.0 );
      __oDeviceDataFix.setType( CDeviceDataFix::FIX_2D );
      __bDataAvailable = true;
    }

    // ... course
    if( __iCourse != AIS_COURSE_NOT_AVAILABLE )
    {
      __oDeviceDataFix.setBearing( (double)__iCourse / 10.0 ); // AIS: decidegrees
      __bDataAvailable = true;
    }

    // ... speed
    if( __iSpeed != AIS_SPEED_NOT_AVAILABLE && __iSpeed > 0 )
    {
      __oDeviceDataFix.setSpeed( __iSpeed / 19.4384449244 ); // AIS: deciknot
      __bDataAvailable = true;
    }

    // ... textual data
    if( !__qsShipname.isEmpty() || !__qsCallsign.isEmpty() )
    {
      QString __qsName = __qsShipname;
      if( !__qsCallsign.isEmpty() ) __qsName += __qsName.isEmpty() ? __qsCallsign : " ("+__qsCallsign+")";
      __oDeviceDataFix.setText( __qsName );
      __bDataAvailable = true;
    }

    // [end]
    if( __bDataAvailable )
    {
      emit signalDataFix( __oDeviceDataFix );
    }

  }
  while( true ); // data-processing loop

  pqSocketNotifier->setEnabled( true );
  __pqMutexDataChange->unlock();
  //qDebug( "DEBUG[%s]: End", Q_FUNC_INFO );
}

//
// OTHER
//

QVCT::EStatus CDeviceGpsdAis::stop()
{
  bPaused = false;
  if( pqSocketNotifier )
  {
    pqSocketNotifier->setEnabled( false );
    delete pqSocketNotifier;
    pqSocketNotifier = 0;
  }
  if( psGpsData )
  {
    gps_stream( psGpsData, WATCH_DISABLE, NULL );
    gps_close( psGpsData );
    psGpsData = 0;
  }
  qDebug( "DEBUG[%s]: Device successfully stopped", Q_FUNC_INFO );
  emit signalOperatingMode( CDevice::STOP );
  return QVCT::OK;
}

QVCT::EStatus CDeviceGpsdAis::pause()
{
  if( psGpsData )
  {
    bPaused = !bPaused;
  }
  qDebug( "DEBUG[%s]: Device successfully paused", Q_FUNC_INFO );
  emit signalOperatingMode( CDevice::PAUSE );
  return QVCT::OK;
}

QVCT::EStatus CDeviceGpsdAis::start()
{
  if( psGpsData ) return QVCT::OK;
  int __iStatus = 0;
  do // error-catching loop
  {
    bPaused = false;

    // Open device
#if GPSD_API_MAJOR_VERSION >= 5
    __iStatus = gps_open( qsHost.toLocal8Bit().constData(), QString::number( iPort ).toLocal8Bit().constData(), &sGpsData );
    psGpsData = &sGpsData;
#else
    psGpsData = gps_open( qsHost.toLocal8Bit().constData(), QString::number( iPort ).toLocal8Bit().constData() );
    if( !psGpsData ) __iStatus = -1;
#endif
    if( __iStatus < 0 )
    {
      psGpsData = 0;
      qCritical( "ERROR[%s]: Failed to open device; status=%d", Q_FUNC_INFO, __iStatus );
      emit signalError( QString( tr("Failed to open device")+"; status=%1" ).arg( __iStatus ) );
      break;
    }

    // Initialize socket notifier
    pqSocketNotifier = new QSocketNotifier( psGpsData->gps_fd, QSocketNotifier::Read );
    QObject::connect( pqSocketNotifier, SIGNAL( activated(int) ), this, SLOT( slotProcessData(int) ) );
    pqSocketNotifier->setEnabled( true );

    // Start GPS data streaming
    __iStatus = gps_stream( psGpsData, WATCH_ENABLE|WATCH_NEWSTYLE, NULL );
    if( __iStatus < 0 )
    {
      qCritical( "ERROR[%s]: Failed to stream data from device; status=%d", Q_FUNC_INFO, __iStatus );
      emit signalError( QString( tr("Failed to stream data from device")+"; status=%1" ).arg( __iStatus ) );
      break;
    }

  }
  while( false ); // error-catching loop
  if( __iStatus < 0 )
  {
    stop();
    return QVCT::ERROR;
  }
  qDebug( "DEBUG[%s]: Device successfully started; socket=%d", Q_FUNC_INFO, psGpsData->gps_fd );
  emit signalOperatingMode( CDevice::START );
  return QVCT::OK;
}

CDevice::EOperatingMode CDeviceGpsdAis::status()
{
  if( !psGpsData ) return CDevice::STOP;
  if( bPaused ) return CDevice::PAUSE;
  return CDevice::START;
}
