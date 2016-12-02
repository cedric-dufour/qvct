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
#include <QAbstractSocket> // QtNetwork module
#include <QDateTime>
#include <QDomElement> // QtXml module
#include <QHash>
#include <QList>
#include <QRegExp>
#include <QString>
#include <QTcpSocket> // QtNetwork module
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/data/CDeviceDataFix.hpp"
#include "devices/data/CDeviceDataSkyView.hpp"
#include "devices/drivers/TcpSbs1/CDeviceTcpSbs1.hpp"
#include "devices/drivers/TcpSbs1/CDeviceTcpSbs1EditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceTcpSbs1::CDeviceTcpSbs1( const QString& _rqsName )
  : CDevice( _rqsName )
  , qsHost( "127.0.0.1" )
  , iPort( 30003 )
  , eTimeZone( CUnitTimeZone::LOCAL )
  , bCallsignLookup( true )
  , bGroundTraffic( false )
  , pqTcpSocket( 0 )
  , bStarted( false )
  , bPaused( false )
  , fdCallsignCleanupTimestamp( 0 )
{
  pqTcpSocket = new QTcpSocket( this );
  QObject::connect( pqTcpSocket, SIGNAL( connected() ), this, SLOT( slotTcpConnected() ) );
  QObject::connect( pqTcpSocket, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( slotTcpError( QAbstractSocket::SocketError ) ) );
  QObject::connect( pqTcpSocket, SIGNAL( readyRead() ), this, SLOT( slotProcessData() ) );
}

CDeviceTcpSbs1::~CDeviceTcpSbs1()
{
  stop();
}


//------------------------------------------------------------------------------
// METHODS: CDevice (implement/override)
//------------------------------------------------------------------------------

QVCT::EStatus CDeviceTcpSbs1::setOperatingMode( CDevice::EOperatingMode _eOperatingMode )
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

void CDeviceTcpSbs1::showDetail()
{
  QVCTRuntime::useDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::DEVICE );
}

void CDeviceTcpSbs1::showEdit()
{
  CDeviceTcpSbs1EditView* __poDeviceTcpSbs1EditView = new CDeviceTcpSbs1EditView( this );
  if( __poDeviceTcpSbs1EditView->exec() == QDialog::Accepted ) showDetail();
  delete __poDeviceTcpSbs1EditView;
}

void CDeviceTcpSbs1::parseQVCT( const QDomElement& _rqDomElement )
{
  QDomElement __qDomElement = _rqDomElement.firstChildElement( "Configuration" );
  if( __qDomElement.isNull() ) return;
  qsHost = __qDomElement.attribute( "host", "127.0.0.1" );
  iPort = __qDomElement.attribute( "port", "30003" ).toInt();
  eTimeZone = CUnitTimeZone::fromCode( __qDomElement.attribute( "time_zone", "local" ) );
  bCallsignLookup = (bool)__qDomElement.attribute( "callsign_lookup", "1" ).toInt();
  bGroundTraffic = (bool)__qDomElement.attribute( "ground_traffic", "0" ).toInt();
}

void CDeviceTcpSbs1::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // Device
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... driver
  _rqXmlStreamWriter.writeAttribute( "name", qsName );
  _rqXmlStreamWriter.writeAttribute( "driver", "tcp_sbs1" );
  // ... configuration
  _rqXmlStreamWriter.writeStartElement( "Configuration" );
  _rqXmlStreamWriter.writeAttribute( "host", qsHost );
  _rqXmlStreamWriter.writeAttribute( "port", QString::number( iPort ) );
  _rqXmlStreamWriter.writeAttribute( "time_zone", CUnitTimeZone::toCode( eTimeZone ) );
  _rqXmlStreamWriter.writeAttribute( "callsign_lookup", QString::number( (int)bCallsignLookup ) );
  _rqXmlStreamWriter.writeAttribute( "ground_traffic", QString::number( (int)bGroundTraffic ) );
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

void CDeviceTcpSbs1::slotTcpConnected()
{
  qDebug( "DEBUG[%s]: Device successfully started", Q_FUNC_INFO );
  bStarted = true;
  emit signalOperatingMode( CDevice::START );
}


void CDeviceTcpSbs1::slotTcpError( QAbstractSocket::SocketError _qSocketError )
{
  QString __qsError = pqTcpSocket->errorString();
  qCritical( "ERROR[%s]: Connection error; %s", Q_FUNC_INFO, qPrintable( __qsError ) );
  emit signalError( QString( tr("Connection error")+"; %1" ).arg( __qsError ) );
  stop();
}

void CDeviceTcpSbs1::slotProcessData()
{
  // NOTE: SBS-1 protocol: http://www.homepages.mcb.net/bones/SBS/Article/Barebones42_Socket_Data.htm
  // NOTE: as per QT documentation: "[...] within a slot connected to the readyRead() signal, readyRead() will not be reemitted"

  static const QRegExp __qRegExpYMD( "^(197\\d|19[89]\\d|[2-9]\\d\\d\\d)/(0?[1-9]|1[12])/(0?[1-9]|[12]\\d|3[01])$" );
  static const QRegExp __qRegExpHMS( "^(0?\\d|1\\d|2[0-3]):(0?\\d|[1-5]\\d):(0?\\d|[1-5]\\d(\\.\\d+)?)$" );

  //qDebug( "DEBUG[%s]: Begin", Q_FUNC_INFO );
  QMutex* __pqMutexDataChange = QVCTRuntime::useMutexDataChange();
  __pqMutexDataChange->lock();

  do // data-processing loop
  {

    // Check data availability
    if( !pqTcpSocket->canReadLine() ) break;
    //qDebug( "DEBUG[%s]: SBS-1 data are waiting to be read", Q_FUNC_INFO );

    // Retrieve data
    QString __qsDataLine = QString::fromLatin1( pqTcpSocket->readLine().trimmed() );
    //qDebug( "DEBUG[%s]: SBS-1 data successfully read; data=%s", Q_FUNC_INFO, qPrintable( __qsDataLine ) );

    // Do not process data if paused
    if( bPaused ) continue;

    // Check data status
    if( __qsDataLine.isEmpty() ) continue; // We need a non-empty string
    emit signalActivity();

    // Check SBS-1 data status
    int __iMSG;
    QStringList __qDataFields = __qsDataLine.split( "," );
    if( __qDataFields.at( 0 ) != "MSG" ) continue; // We process only SBS-1's "MSG" message type
    if( __qDataFields.size() != 22 ) continue; // "MSG" message type ought to be 22-fields long
    if( ( __iMSG = __qDataFields.at( 1 ).toInt() ) > 4 ) continue; // Transmission types greater than "4" are useless to us
    if( __qDataFields.at( 21 ).toInt() < 0 && !bGroundTraffic ) continue; // Ignore ground traffic
    if( __qDataFields.at( 4 ).isEmpty() ) continue; // We need a valid "HexIdent"
    QString __qsSource = __qDataFields.at( 4 );
    //qDebug( "DEBUG[%s]: SBS-1 data are available from source %s", Q_FUNC_INFO, qPrintable( __qsSource ) );

    // Callsign lookup
    if( bCallsignLookup )
    {
      QString __qsCallsign;
      double __fdTimestamp = microtime();

      // Dictionary cleanup (every 300 seconds)
      if( __fdTimestamp - fdCallsignCleanupTimestamp > (double)300.0 )
      {
        fdCallsignCleanupTimestamp = __fdTimestamp;
        QList<QString> __qListSource = qHashCallsign.keys();
        for( QList<QString>::const_iterator __iqsSource = __qListSource.begin();
             __iqsSource != __qListSource.end();
             ++__iqsSource )
          // Cleanup stale entries (older than 900 seconds)
          if( __fdTimestamp - qHashCallsign[ *__iqsSource ].fdTimestamp > (double)900.0 )
            qHashCallsign.remove( *__iqsSource );
      }

      // Actual lookup
      if( !__qDataFields.at( 10 ).isEmpty() )
      {
        __qsCallsign = __qDataFields.at( 10 ).trimmed();
        if( qHashCallsign.contains( __qsSource ) )
          qHashCallsign[ __qsSource ].update( __qsCallsign, __fdTimestamp );
        else
          qHashCallsign.insert( __qsSource, CCallsign( __qsCallsign, __fdTimestamp ) );
      }
      else
      {
        if( !qHashCallsign.contains( __qsSource ) ) continue;
        __qsCallsign = qHashCallsign[ __qsSource ].get( __fdTimestamp );
      }

      // Use looked-up callsign
      __qsSource = __qsCallsign;
    }
    if( __iMSG == 1 ) continue;

    // Parse SBS-1 data
    CDeviceDataFix __oDeviceDataFix( __qsSource );
    __oDeviceDataFix.setSourceType( CDeviceDataSource::SBS );
    __oDeviceDataFix.setCourseFromPosition( false );
    bool __bDataAvailable = false;

    // ... time
    if( !__qDataFields.at( 6 ).isEmpty() && !__qDataFields.at( 7 ).isEmpty()
        && __qRegExpYMD.exactMatch( __qDataFields.at( 6 ) ) && __qRegExpHMS.exactMatch( __qDataFields.at( 7 ) ) )
    {
      QDateTime __qDateTime;
      __qDateTime.setTimeSpec( eTimeZone == CUnitTimeZone::UTC ? Qt::UTC : Qt::LocalTime );
      __qDateTime.setDate( QDate( __qRegExpYMD.cap(1).toInt(), __qRegExpYMD.cap(2).toInt(), __qRegExpYMD.cap(3).toInt() ) );
      __qDateTime.setTime( QTime( __qRegExpHMS.cap(1).toInt(), __qRegExpHMS.cap(2).toInt(), __qRegExpHMS.cap(3).toInt(), !__qRegExpHMS.cap(4).isEmpty() ? 1000*__qRegExpHMS.cap(4).toDouble() : 0 ) );
      __oDeviceDataFix.setTime( (double)__qDateTime.toTime_t() );
    }
    else
    {
      // Let's default to use current system time (some SBS-1 devices do not set the date/time fields)
      __oDeviceDataFix.setTime( (double)QDateTime::currentDateTime().toTime_t() );
    }

    // ... position / elevation
    //     SBS-1 elevation: feet
    if( !__qDataFields.at( 14 ).isEmpty() && !__qDataFields.at( 15 ).isEmpty() )
    {
      if( !__qDataFields.at( 11 ).isEmpty() )
      {
        __oDeviceDataFix.setPosition( __qDataFields.at( 15 ).toDouble(), __qDataFields.at( 14 ).toDouble(), __qDataFields.at( 11 ).toDouble() * 0.3048 );
        __oDeviceDataFix.setType( CDeviceDataFix::FIX_3D );
      }
      else
      {
        __oDeviceDataFix.setPosition( __qDataFields.at( 15 ).toDouble(), __qDataFields.at( 14 ).toDouble() );
        __oDeviceDataFix.setType( CDeviceDataFix::FIX_2D );
      }
      __bDataAvailable = true;
    }

    // ... course
    if( !__qDataFields.at( 13 ).isEmpty() )
    {
      __oDeviceDataFix.setBearing( __qDataFields.at( 13 ).toDouble() );
      __bDataAvailable = true;
    }

    // ... speed
    //     SBS-1 (horizontal) speed: knots
    //     SBS-1 vertical speed: feet/minute
    if( !__qDataFields.at( 12 ).isEmpty() )
    {
      if( !__qDataFields.at( 16 ).isEmpty() )
      {
        __oDeviceDataFix.setSpeed( __qDataFields.at( 12 ).toDouble() / 1.94384449244, __qDataFields.at( 16 ).toDouble() / 196.8503937 );
      }
      else
      {
        __oDeviceDataFix.setSpeed( __qDataFields.at( 12 ).toDouble() / 1.94384449244 );
      }
      __bDataAvailable = true;
    }

    // ... hexident/callsign
    if( bCallsignLookup || !__qDataFields.at( 10 ).isEmpty() )
    {
      __oDeviceDataFix.setText( bCallsignLookup ? "HEX:"+__qDataFields.at( 4 ) : "C/S:"+__qDataFields.at( 10 ) );
      __bDataAvailable = true;
    }

    // [end]
    if( __bDataAvailable )
    {
      emit signalDataFix( __oDeviceDataFix );
    }

  }
  while( true ); // data-processing loop

  __pqMutexDataChange->unlock();
  //qDebug( "DEBUG[%s]: End", Q_FUNC_INFO );
}

//
// OTHER
//

QVCT::EStatus CDeviceTcpSbs1::stop()
{
  bPaused = false;
  if( bStarted )
  {
    pqTcpSocket->abort();
    qHashCallsign.clear();
    bStarted = false;
  }
  qDebug( "DEBUG[%s]: Device successfully stopped", Q_FUNC_INFO );
  emit signalOperatingMode( CDevice::STOP );
  return QVCT::OK;
}

QVCT::EStatus CDeviceTcpSbs1::pause()
{
  if( bStarted )
  {
    bPaused = !bPaused;
  }
  qDebug( "DEBUG[%s]: Device successfully paused", Q_FUNC_INFO );
  emit signalOperatingMode( CDevice::PAUSE );
  return QVCT::OK;
}

QVCT::EStatus CDeviceTcpSbs1::start()
{
  if( bStarted ) return QVCT::OK;
  bPaused = false;
  fdCallsignCleanupTimestamp = microtime();
  pqTcpSocket->connectToHost( qsHost, iPort, QIODevice::ReadOnly );
  // NOTE: device is further "started" once the 'connected' signal is received
  return QVCT::OK;
}

CDevice::EOperatingMode CDeviceTcpSbs1::status()
{
  if( !bStarted ) return CDevice::STOP;
  if( bPaused ) return CDevice::PAUSE;
  return CDevice::START;
}
