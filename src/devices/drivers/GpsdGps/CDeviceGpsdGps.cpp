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
#include <QDomElement> // QtXml module
#include <QString>
#include <QXmlStreamWriter>

// GPSD
#include "gps.h"
// #include "gpsdclient.h"
#ifndef STATUS_DGPS_FIX
#define STATUS_DGPS_FIX  2
#endif

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/data/CDeviceDataFix.hpp"
#include "devices/data/CDeviceDataSkyView.hpp"
#include "devices/drivers/GpsdGps/CDeviceGpsdGps.hpp"
#include "devices/drivers/GpsdGps/CDeviceGpsdGpsEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceGpsdGps::CDeviceGpsdGps( const QString& _rqsName )
  : CDevice( _rqsName )
  , qsHost( "127.0.0.1" )
  , iPort( 2947 )
  , qsSource()
  , psGpsData( 0 )
  , pqSocketNotifier( 0 )
  , bPaused( false )
{}

CDeviceGpsdGps::~CDeviceGpsdGps()
{
  stop();
}


//------------------------------------------------------------------------------
// METHODS: CDevice (implement/override)
//------------------------------------------------------------------------------

QVCT::EStatus CDeviceGpsdGps::setOperatingMode( CDevice::EOperatingMode _eOperatingMode )
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

void CDeviceGpsdGps::showDetail()
{
  QVCTRuntime::useDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::DEVICE );
}

void CDeviceGpsdGps::showEdit()
{
  CDeviceGpsdGpsEditView* __poDeviceGpsdGpsEditView = new CDeviceGpsdGpsEditView( this );
  if( __poDeviceGpsdGpsEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poDeviceGpsdGpsEditView;
}

void CDeviceGpsdGps::parseQVCT( const QDomElement& _rqDomElement )
{
  QDomElement __qDomElement = _rqDomElement.firstChildElement( "Configuration" );
  if( __qDomElement.isNull() ) return;
  qsHost = __qDomElement.attribute( "host", "127.0.0.1" );
  iPort = __qDomElement.attribute( "port", "2947" ).toInt();
  qsSource = __qDomElement.attribute( "source" );
}

void CDeviceGpsdGps::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // Device
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... driver
  _rqXmlStreamWriter.writeAttribute( "name", qsName );
  _rqXmlStreamWriter.writeAttribute( "driver", "gpsd_gps" );
  // ... configuration
  _rqXmlStreamWriter.writeStartElement( "Configuration" );
  _rqXmlStreamWriter.writeAttribute( "host", qsHost );
  _rqXmlStreamWriter.writeAttribute( "port", QString::number( iPort ) );
  if( !qsSource.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "source", qsSource );
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
void CDeviceGpsdGps::slotProcessData( int )
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
#if GPSD_API_MAJOR_VERSION >= 7
    __iStatus = gps_read( psGpsData, NULL, 0 );
#elif GPSD_API_MAJOR_VERSION >= 5
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

    // Check device data
    // NOTE: ( psGpsData->set & DEVICE_SET ) is unreliable; let's use an alternate method
    if( psGpsData->dev.path[0] == '\0' ) continue; // We MUST have a device/source name
    QString __qsSource = QString::fromLatin1( psGpsData->dev.path );
    //qDebug( "DEBUG[%s]: GPS data are available from source %s", Q_FUNC_INFO, qPrintable( __qsSource ) );

    // Check source (filter)
    if( !qsSource.isEmpty() && qsSource != __qsSource ) continue;

    // Check position/course data
    if( psGpsData->set & (TIME_SET|LATLON_SET|ALTITUDE_SET|TRACK_SET|SPEED_SET|CLIMB_SET) )
    {
      CDeviceDataFix __oDeviceDataFix( __qsSource );
      __oDeviceDataFix.setSourceType( CDeviceDataSource::GPS );

      // Fix type
      {
        int __eFixType = CDeviceDataFix::FIX_UNDEFINED;
        if( psGpsData->set & STATUS_SET )
        {
          __eFixType &= ~CDeviceDataFix::FIX_UNDEFINED;
#if GPSD_API_MAJOR_VERSION >= 10
          switch( psGpsData->fix.status )
#else
          switch( psGpsData->status )
#endif
          {
          case STATUS_NO_FIX: __eFixType |= CDeviceDataFix::FIX_NONE; break;
          case STATUS_FIX: __eFixType |= CDeviceDataFix::FIX_2D; break;
          case STATUS_DGPS_FIX: __eFixType |= CDeviceDataFix::FIX_2D | CDeviceDataFix::FIX_DGPS; break;
          default:;
          }
        }
        if( psGpsData->fix.mode != MODE_NOT_SEEN )
        {
          __eFixType &= ~CDeviceDataFix::FIX_UNDEFINED;
          switch( psGpsData->fix.mode )
          {
          case MODE_NO_FIX: __eFixType |= CDeviceDataFix::FIX_NONE; break;
          case MODE_2D: __eFixType |= CDeviceDataFix::FIX_2D; break;
          case MODE_3D: __eFixType |= CDeviceDataFix::FIX_2D | CDeviceDataFix::FIX_3D; break;
          default:;
          }
        }
        __oDeviceDataFix.setType( __eFixType );
      }

      // Time
#if GPSD_API_MAJOR_VERSION >= 9
      if( psGpsData->fix.time.tv_sec > 0 ) __oDeviceDataFix.setTime( psGpsData->fix.time.tv_sec, psGpsData->fix.time.tv_nsec );
#else
      if( !std::isnan( psGpsData->fix.time ) && psGpsData->fix.time > 0.0 ) __oDeviceDataFix.setTime( psGpsData->fix.time );
#endif

      // Position
      if( !std::isnan( psGpsData->fix.longitude ) && !std::isnan( psGpsData->fix.latitude ) )
        __oDeviceDataFix.setPosition( psGpsData->fix.longitude, psGpsData->fix.latitude,
                                      !std::isnan( psGpsData->fix.altitude ) ? psGpsData->fix.altitude : CDataPosition::UNDEFINED_ELEVATION );

      // Course
      if( !std::isnan( psGpsData->fix.track ) ) __oDeviceDataFix.setBearing( psGpsData->fix.track );
      if( !std::isnan( psGpsData->fix.speed ) && fabs( psGpsData->fix.speed ) > 0.0 ) __oDeviceDataFix.setSpeed( psGpsData->fix.speed );
      if( !std::isnan( psGpsData->fix.climb ) && fabs( psGpsData->fix.climb ) > 0.0 ) __oDeviceDataFix.setSpeedVertical( psGpsData->fix.climb );

      // Errors
      if( !std::isnan( psGpsData->fix.ept ) && psGpsData->fix.ept > 0.0 ) __oDeviceDataFix.setErrorTime( psGpsData->fix.ept );
      if( !std::isnan( psGpsData->fix.epx ) && !std::isnan( psGpsData->fix.epy )
          && ( psGpsData->fix.epx > 0.0 || psGpsData->fix.epy > 0.0 ) )
        __oDeviceDataFix.setErrorPosition( sqrt( psGpsData->fix.epx*psGpsData->fix.epx + psGpsData->fix.epy*psGpsData->fix.epy ),
                                           !std::isnan( psGpsData->fix.epv ) && psGpsData->fix.epv > 0.0 ? psGpsData->fix.epv : CDataValidity::UNDEFINED_VALUE );
      if( !std::isnan( psGpsData->fix.epd ) && psGpsData->fix.epd > 0.0 ) __oDeviceDataFix.setErrorBearing( psGpsData->fix.epd );
      if( !std::isnan( psGpsData->fix.eps ) && psGpsData->fix.eps > 0.0 )
        __oDeviceDataFix.setErrorSpeed( psGpsData->fix.eps,
                                        !std::isnan( psGpsData->fix.epc ) && psGpsData->fix.epc > 0.0 ? psGpsData->fix.epc : CDataValidity::UNDEFINED_VALUE );

      // DOPs
      double __fdDopHorizontal = CDeviceDataDop::UNDEFINED_VALUE;
      if( !std::isnan( psGpsData->dop.hdop ) && psGpsData->dop.hdop > 0.0 ) __fdDopHorizontal = psGpsData->dop.hdop;
      else if( !std::isnan( psGpsData->dop.xdop ) && psGpsData->dop.xdop > 0.0
               && !std::isnan( psGpsData->dop.ydop ) && psGpsData->dop.ydop > 0.0 )
        __fdDopHorizontal = sqrt( psGpsData->dop.xdop*psGpsData->dop.xdop + psGpsData->dop.ydop*psGpsData->dop.ydop );
      __oDeviceDataFix.setDopPosition( __fdDopHorizontal,
                                       !std::isnan( psGpsData->dop.vdop ) && psGpsData->dop.vdop > 0.0 ? psGpsData->dop.vdop : CDeviceDataDop::UNDEFINED_VALUE );
      if( !std::isnan( psGpsData->dop.tdop ) && psGpsData->dop.tdop > 0.0 ) __oDeviceDataFix.setDopTime( psGpsData->dop.tdop );

      // Sources (satellites)
      __oDeviceDataFix.setSources( psGpsData->satellites_visible, psGpsData->satellites_used );

      // [end]
      emit signalDataFix( __oDeviceDataFix );
    }

    // Check sky view data
    if( psGpsData->set & SATELLITE_SET )
    {
      CDeviceDataSkyView __oDeviceDataSkyView( __qsSource );
      __oDeviceDataSkyView.setSourceType( CDeviceDataSource::GPS );

      // Time
#if GPSD_API_MAJOR_VERSION >= 9
      if( psGpsData->skyview_time.tv_sec > 0 ) __oDeviceDataSkyView.setTime( psGpsData->skyview_time.tv_sec, psGpsData->skyview_time.tv_nsec );
#else
      if( !std::isnan( psGpsData->skyview_time ) && psGpsData->skyview_time > 0.0 ) __oDeviceDataSkyView.setTime( psGpsData->skyview_time );
#endif

      // Loop through satellites data
      for( int __i = 0; __i < psGpsData->satellites_visible; __i++ )
      {
#if GPSD_API_MAJOR_VERSION >= 6
        int __iPRN = psGpsData->skyview[ __i ].PRN;
        CDeviceDataSatellite __oDeviceDataSatellite( __iPRN );
        __oDeviceDataSatellite.setAzimuth( psGpsData->skyview[ __i ].azimuth );
        __oDeviceDataSatellite.setElevation( psGpsData->skyview[ __i ].elevation );
        __oDeviceDataSatellite.setSignal( psGpsData->skyview[ __i ].ss );
#else
         int __iPRN = psGpsData->PRN[ __i ];
         CDeviceDataSatellite __oDeviceDataSatellite( __iPRN );
         __oDeviceDataSatellite.setAzimuth( psGpsData->azimuth[ __i ] );
         __oDeviceDataSatellite.setElevation( psGpsData->elevation[ __i ] );
         __oDeviceDataSatellite.setSignal( psGpsData->ss[ __i ] );
#endif
        bool __bUsed = false;
        for( int __j = 0; __j < psGpsData->satellites_used; __j++ )
        {
#if GPSD_API_MAJOR_VERSION >= 6
          if( psGpsData->skyview[ __j ].used == __iPRN )
#else
          if( psGpsData->used[ __j ] == __iPRN )
#endif
          {
            __bUsed = true;
            break;
          }
        }
        __oDeviceDataSatellite.setUsed( __bUsed );
        __oDeviceDataSkyView.append( __oDeviceDataSatellite );
      }

      // DOPs
      double __fdDopHorizontal = CDeviceDataDop::UNDEFINED_VALUE;
      if( !std::isnan( psGpsData->dop.hdop ) && psGpsData->dop.hdop > 0.0 ) __fdDopHorizontal = psGpsData->dop.hdop;
      else if( !std::isnan( psGpsData->dop.xdop ) && psGpsData->dop.xdop > 0.0
               && !std::isnan( psGpsData->dop.ydop ) && psGpsData->dop.ydop > 0.0 )
        __fdDopHorizontal = sqrt( psGpsData->dop.xdop*psGpsData->dop.xdop + psGpsData->dop.ydop*psGpsData->dop.ydop );
      __oDeviceDataSkyView.setDopPosition( __fdDopHorizontal,
                                       !std::isnan( psGpsData->dop.vdop ) && psGpsData->dop.vdop > 0.0 ? psGpsData->dop.vdop : CDeviceDataDop::UNDEFINED_VALUE );
      if( !std::isnan( psGpsData->dop.tdop ) && psGpsData->dop.tdop > 0.0 ) __oDeviceDataSkyView.setDopTime( psGpsData->dop.tdop );

      // [end]
      emit signalDataSkyView( __oDeviceDataSkyView );
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

QVCT::EStatus CDeviceGpsdGps::stop()
{
  bPaused = false;
  if( pqSocketNotifier )
  {
    QObject::disconnect( pqSocketNotifier, 0, this, 0 );
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

QVCT::EStatus CDeviceGpsdGps::pause()
{
  if( psGpsData )
  {
    bPaused = !bPaused;
  }
  qDebug( "DEBUG[%s]: Device successfully paused", Q_FUNC_INFO );
  emit signalOperatingMode( CDevice::PAUSE );
  return QVCT::OK;
}

QVCT::EStatus CDeviceGpsdGps::start()
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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    pqSocketNotifier = new QSocketNotifier( (qintptr)psGpsData->gps_fd, QSocketNotifier::Read );
#else
    pqSocketNotifier = new QSocketNotifier( (quintptr)psGpsData->gps_fd, QSocketNotifier::Read );
#endif
    QObject::connect( pqSocketNotifier, SIGNAL( activated(int) ), this, SLOT( slotProcessData(int) ) );
    pqSocketNotifier->setEnabled( true );

    // Start GPS data streaming
    // F***: Source filtering should be achieved using 'gps_stream' ad-hoc mechanism... except
    //       it requires 'gpsdclient.{h,c}' resources which are not available as part of 'libgps' (...)
    // if( !qsSource.isEmpty() )
    // {
    //   gpsd_source_spec( QString( qsHost+":"+QString::number( iPort )+":"+qsSource ).toAscii().data(), &sGpsSource );
    //   __iStatus = gps_stream( psGpsData, WATCH_DEVICE|WATCH_ENABLE|WATCH_NEWSTYLE, &sGpsSource );
    // }
    // else
    // {
    // __iStatus = gps_stream( psGpsData, WATCH_ENABLE|WATCH_NEWSTYLE, NULL );
    // }
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
#ifndef USE_QT
  qDebug( "DEBUG[%s]: Device successfully started; socket=%d", Q_FUNC_INFO, psGpsData->gps_fd );
#else
  qDebug( "DEBUG[%s]: Device successfully started; socket=%p", Q_FUNC_INFO, psGpsData->gps_fd );
#endif
  emit signalOperatingMode( CDevice::START );
  return QVCT::OK;
}

CDevice::EOperatingMode CDeviceGpsdGps::status()
{
  if( !psGpsData ) return CDevice::STOP;
  if( bPaused ) return CDevice::PAUSE;
  return CDevice::START;
}
