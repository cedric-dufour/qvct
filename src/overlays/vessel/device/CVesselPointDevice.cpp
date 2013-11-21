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
#include <QPainter>
#include <QPointF>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/CDevice.hpp"
#include "devices/data/CDeviceDataSkyView.hpp"
#include "overlays/vessel/device/CVesselPointDevice.hpp"
#include "overlays/vessel/device/CVesselPointDeviceEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointDevice::CVesselPointDevice( const QString& _rqsName, const QString& _rqsSourceName, bool _bDynamic )
  : COverlayItem( COverlayObject::SUBITEM2, _rqsName )
  , CDeviceDataFix( _rqsSourceName )
  , bDynamic( _bDynamic )
  , poDevice( 0 )
  , bSynchronizePosition( false )
  , bSynchronizeElevation( false )
  , bSynchronizeTime( false )
  , bSynchronizeGroundBearing( false )
  , bSynchronizeGroundSpeed( false )
  , bSynchronizeGroundSpeedVertical( false )
  , bSynchronizeApparentBearing( false )
  , bSynchronizeApparentSpeed( false )
  , bSynchronizeApparentSpeedVertical( false )
  , bSynchronizeText( false )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
  QTreeWidgetItem::setText( CVesselOverlay::NAME, getName() );
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CVesselPointDevice::showDetail()
{
  QVCTRuntime::useVesselPointDeviceDetailView()->setOverlayObject( this );
  QVCTRuntime::useVesselPointDeviceDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::VESSEL_POINT_DEVICE );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::VESSEL );
}

void CVesselPointDevice::showEdit()
{
  CVesselPointDeviceEditView* __poVesselPointDeviceEditView = new CVesselPointDeviceEditView( this );
  if( __poVesselPointDeviceEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poVesselPointDeviceEditView;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselPointDevice::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poDevice != _pqObject ) return;
  disconnectDevice();
}

void CVesselPointDevice::slotDataFix( const CDeviceDataFix& _roDeviceDataFix )
{
  if( !CDeviceDataFix::getSourceName().isEmpty()
      && CDeviceDataFix::getSourceName() != _roDeviceDataFix.getSourceName() ) return;

  // Copy fix data
  CDeviceDataFix::setFix( _roDeviceDataFix );

  // Retrieve data
  CSettings* __poSettings = QVCTRuntime::useSettings();
  CVesselPoint* __poVesselPoint = (CVesselPoint*)QTreeWidgetItem::parent();
  // ... system time
  double __fdSystemTime = microtime();
  // ... device time
  double __fdTime = bSynchronizeTime ? CDataTime::getTime() : CDataTime::UNDEFINED_TIME;
  double __fdTimeDelta = __fdTime != CDataTime::UNDEFINED_TIME && __poVesselPoint->getTime() != CDataTime::UNDEFINED_TIME
    ? __fdTime - __poVesselPoint->getTime() : CDataTime::UNDEFINED_TIME;
  // ... position
  double __fdLongitude = bSynchronizePosition ? CDataPosition::getLongitude() : CDataPosition::UNDEFINED_LONGITUDE;
  double __fdLatitude = bSynchronizePosition ? CDataPosition::getLatitude() : CDataPosition::UNDEFINED_LATITUDE;
  // ... elevation
  double __fdElevation = bSynchronizeElevation ? CDataPosition::getElevation() : CDataPosition::UNDEFINED_ELEVATION;
  // ... ground course
  double __fdGroundBearing = bSynchronizeGroundBearing ? CDataCourse::getBearing() : CDataCourse::UNDEFINED_BEARING;
  double __fdGroundSpeed = bSynchronizeGroundSpeed ? CDataCourse::getSpeed() : CDataCourse::UNDEFINED_SPEED;
  double __fdGroundSpeedVertical = bSynchronizeGroundSpeedVertical ? CDataCourse::getSpeedVertical() : CDataCourse::UNDEFINED_SPEED;
  // ... apparent course
  double __fdApparentBearing = bSynchronizeApparentBearing ? CDataCourse::getBearing() : CDataCourse::UNDEFINED_BEARING;
  double __fdApparentSpeed = bSynchronizeApparentSpeed ? CDataCourse::getSpeed() : CDataCourse::UNDEFINED_SPEED;
  double __fdApparentSpeedVertical = bSynchronizeApparentSpeedVertical ? CDataCourse::getSpeedVertical() : CDataCourse::UNDEFINED_SPEED;
  // ... errors
  double __fdErrorTime = CDeviceDataFix::getErrorTime();
  double __fdErrorHorizontal = CDeviceDataFix::getErrorHorizontal();
  double __fdErrorVertical = CDeviceDataFix::getErrorVertical();
  double __fdErrorBearing = CDeviceDataFix::getErrorBearing();
  double __fdErrorSpeed = CDeviceDataFix::getErrorSpeed();
  double __fdErrorSpeedVertical = CDeviceDataFix::getErrorSpeedVertical();
  // ... additional textual data string
  QString __qsText = bSynchronizeText ? CDeviceDataFix::getText() : "";

  // Synchronize data
  // NOTE: We synchronize position/time data last, in order to use those to compute position/time-derived ground course data if required
  // NOTE: For the same reason, we process ground speed before ground bearing
  bool __bVesselSynchronized = false;

  // ... horizontal ground speed
  if( bSynchronizeGroundSpeed )
  {
    if( __fdGroundSpeed == CDataCourse::UNDEFINED_SPEED
        && CDeviceDataFix::isCourseFromPosition()
        && bSynchronizePosition && bSynchronizeTime
        && __fdLongitude != CDataPosition::UNDEFINED_LONGITUDE
        && __fdLatitude != CDataPosition::UNDEFINED_LATITUDE
        && __poVesselPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
        && __poVesselPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE
        && __fdTimeDelta != CDataTime::UNDEFINED_TIME
        && __fdTimeDelta > 0.1 )
    {
      // ... compute position/time-derived value
      __fdGroundSpeed = CDataPosition::distanceRL( *__poVesselPoint, *this ) / __fdTimeDelta;
    }
    if( __fdGroundSpeed != CDataCourse::UNDEFINED_SPEED )
    {
      if( __fdErrorSpeed == CDataCourseValidity::UNDEFINED_VALUE
          && bSynchronizePosition && bSynchronizeTime
          && __fdErrorHorizontal != CDataPositionValidity::UNDEFINED_VALUE
          && __poVesselPoint->getErrorPosition() != CDataPositionValidity::UNDEFINED_VALUE
          && __fdErrorTime != CDataTimeValidity::UNDEFINED_VALUE
          && __poVesselPoint->getErrorTime() != CDataTimeValidity::UNDEFINED_VALUE
          && __fdTimeDelta != CDataTime::UNDEFINED_TIME
          && __fdTimeDelta > 0.1 )
      {
        // ... compute elevation/time-derived value
        // NOTE: Let's assume that device errors remain in the same direction over a short time.
        //       Thus, instead of using the maximum accumulated error for each value, let's use a
        //       "diminished" maxmimum error (factor 1/10) AND the error delta between the previous
        //       and the current point.
        __fdErrorSpeed =
          ( ( __fdErrorHorizontal + __poVesselPoint->getErrorPosition() ) / 10.0 + fabs( __fdErrorHorizontal - __poVesselPoint->getErrorPosition() ) ) / __fdTimeDelta
          * ( 1.0 + ( ( __fdErrorTime + __poVesselPoint->getErrorTime() ) / 10.0 + fabs(  __fdErrorTime - __poVesselPoint->getErrorTime() ) ) / __fdTimeDelta );
      }
      __poVesselPoint->GroundCourse.setSpeed( __fdGroundSpeed );
      __poVesselPoint->GroundCourseValidity.setValiditySpeed( __fdSystemTime, __fdErrorSpeed,
                                                              fabs( __fdGroundSpeed ) < __poSettings->getMinValueSpeed() );
      __bVesselSynchronized = true;
    }
  }

  // ... vertical ground speed
  if( bSynchronizeGroundSpeedVertical )
  {
    if( __fdGroundSpeedVertical == CDataCourse::UNDEFINED_SPEED
        && CDeviceDataFix::isCourseFromPosition()
        && bSynchronizeElevation && bSynchronizeTime
        && __fdElevation != CDataPosition::UNDEFINED_ELEVATION
        && __poVesselPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION
        && __fdTimeDelta != CDataTime::UNDEFINED_TIME
        && __fdTimeDelta > 0.1 )
    {
      // ... compute elevation/time-derived value
      __fdGroundSpeedVertical = ( __fdElevation - __poVesselPoint->getElevation() ) / __fdTimeDelta;
    }
    if( __fdGroundSpeedVertical != CDataCourse::UNDEFINED_SPEED )
    {
      if( __fdErrorSpeedVertical == CDataCourseValidity::UNDEFINED_VALUE
          && bSynchronizeElevation && bSynchronizeTime
          && __fdErrorVertical != CDataPositionValidity::UNDEFINED_VALUE
          && __poVesselPoint->getErrorElevation() != CDataPositionValidity::UNDEFINED_VALUE
          && __fdErrorTime != CDataTimeValidity::UNDEFINED_VALUE
          && __poVesselPoint->getErrorTime() != CDataTimeValidity::UNDEFINED_VALUE
          && __fdTimeDelta != CDataTime::UNDEFINED_TIME
          && __fdTimeDelta > 0.1 )
      {
        // ... compute elevation/time-derived value
        // NOTE: Let's assume that device errors remain in the same direction over a short time.
        //       Thus, instead of using the maximum accumulated error for each value, let's use a
        //       "diminished" maximum error (factor 1/10) AND the error delta between the previous
        //       and the current point.
        __fdErrorSpeedVertical =
          ( ( __fdErrorVertical + __poVesselPoint->getErrorElevation() ) / 10.0 + fabs( __fdErrorVertical - __poVesselPoint->getErrorElevation() ) ) / __fdTimeDelta
          * ( 1.0 + ( ( __fdErrorTime + __poVesselPoint->getErrorTime() ) / 10.0 + fabs( __fdErrorTime - __poVesselPoint->getErrorTime() ) ) / __fdTimeDelta );
      }
      __poVesselPoint->GroundCourse.setSpeedVertical( __fdGroundSpeedVertical );
      __poVesselPoint->GroundCourseValidity.setValiditySpeedVertical( __fdSystemTime, __fdErrorSpeedVertical,
                                                                      fabs( __fdGroundSpeedVertical ) < __poSettings->getMinValueSpeedVertical() );
      __bVesselSynchronized = true;
    }
  }

  // ... ground bearing
  if( bSynchronizeGroundBearing
      && ( !bSynchronizeGroundSpeed || __fdGroundSpeed != CDataCourse::UNDEFINED_SPEED ) )
  {
    if( __fdGroundBearing == CDataCourse::UNDEFINED_BEARING
        && CDeviceDataFix::isCourseFromPosition()
        && bSynchronizePosition
        && __fdLongitude != CDataPosition::UNDEFINED_LONGITUDE
        && __fdLatitude != CDataPosition::UNDEFINED_LATITUDE
        && __poVesselPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
        && __poVesselPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE )
    {
      // ... compute position-derived value
      __fdGroundBearing = CDataPosition::bearingRL( *__poVesselPoint, *this );
    }
    if( __fdGroundBearing != CDataCourse::UNDEFINED_BEARING )
    {
      if( __fdErrorBearing == CDataCourseValidity::UNDEFINED_VALUE
          && bSynchronizePosition
          && __fdLongitude != CDataPosition::UNDEFINED_LONGITUDE
          && __fdLatitude != CDataPosition::UNDEFINED_LATITUDE
          && __poVesselPoint->getLongitude() != CDataPosition::UNDEFINED_LONGITUDE
          && __poVesselPoint->getLatitude() != CDataPosition::UNDEFINED_LATITUDE
          && __fdErrorHorizontal != CDataPositionValidity::UNDEFINED_VALUE
          && __poVesselPoint->getErrorPosition() != CDataPositionValidity::UNDEFINED_VALUE )
      {
        // ... compute position/time-derived value
        // NOTE: Let's assume that device errors remain in the same direction over a short time.
        //       Thus, instead of using the maximum accumulated error for each value, let's use a
        //       "diminished" maximum error (factor 1/10) AND the error delta between the previous
        //       and the current point.
        double __fdDistance = CDataPosition::distanceRL( *__poVesselPoint, *this );
        if( __fdDistance > 1.0 )
        {
          __fdErrorBearing =
            atan( ( ( __fdErrorHorizontal + __poVesselPoint->getErrorPosition() ) / 10.0 + fabs( __fdErrorHorizontal - __poVesselPoint->getErrorPosition() ) ) / __fdDistance )
            * QVCT::RAD2DEG;
        }
      }
      __poVesselPoint->GroundCourse.setBearing( __fdGroundBearing );
      __poVesselPoint->GroundCourseValidity.setValidityBearing( __fdSystemTime, __fdErrorBearing,
                                                                bSynchronizeGroundSpeed && __fdGroundSpeed != CDataCourse::UNDEFINED_SPEED
                                                                ? fabs( __fdGroundSpeed ) < __poSettings->getMinValueSpeed() : false );
      __bVesselSynchronized = true;
    }
  }

  // ... apparent bearing
  if( __fdApparentBearing != CDataCourse::UNDEFINED_BEARING
      && ( !bSynchronizeApparentSpeed || __fdApparentSpeed != CDataCourse::UNDEFINED_SPEED ) )
  {
    __poVesselPoint->ApparentCourse.setBearing( __fdApparentBearing );
    __poVesselPoint->ApparentCourseValidity.setValidityBearing( __fdSystemTime, __fdErrorBearing,
                                                                bSynchronizeApparentSpeed && __fdApparentSpeed != CDataCourse::UNDEFINED_SPEED
                                                                ? fabs( __fdApparentSpeed ) < __poSettings->getMinValueSpeed() : false );
    __bVesselSynchronized = true;
  }

  // ... horizontal apparent speed
  if( __fdApparentSpeed != CDataCourse::UNDEFINED_SPEED )
  {
    __poVesselPoint->ApparentCourse.setSpeed( __fdApparentSpeed );
    __poVesselPoint->ApparentCourseValidity.setValiditySpeed( __fdSystemTime, __fdErrorSpeed,
                                                              fabs( __fdApparentSpeed ) < __poSettings->getMinValueSpeed() );
    __bVesselSynchronized = true;
  }

  // ... vertical apparent speed
  if( __fdApparentSpeedVertical != CDataCourse::UNDEFINED_SPEED )
  {
    __poVesselPoint->ApparentCourse.setSpeedVertical( __fdApparentSpeedVertical );
    __poVesselPoint->ApparentCourseValidity.setValiditySpeedVertical( __fdSystemTime, __fdErrorSpeedVertical,
                                                                      fabs( __fdApparentSpeedVertical ) < __poSettings->getMinValueSpeedVertical() );
    __bVesselSynchronized = true;
  }

  // ... position
  if( __fdLongitude != CDataPosition::UNDEFINED_LONGITUDE
      && __fdLatitude != CDataPosition::UNDEFINED_LATITUDE )
  {
    __bVesselSynchronized = true;
    __poVesselPoint->setPosition( __fdLongitude, __fdLatitude );
    __poVesselPoint->setValidityPosition( __fdSystemTime, __fdErrorHorizontal );
    __poVesselPoint->setFixType( CDeviceDataFix::getType() );
    __poVesselPoint->setSatelliteCount( CDeviceDataFix::getSourcesUsed() );
    __poVesselPoint->setDopHorizontal( CDeviceDataDop::getDopHorizontal() );
  }

  // ... elevation
  if( __fdElevation != CDataPosition::UNDEFINED_ELEVATION )
  {
    __poVesselPoint->setElevation( __fdElevation );
    __poVesselPoint->setValidityElevation( __fdSystemTime, __fdErrorVertical );
    __poVesselPoint->setDopVertical( CDeviceDataDop::getDopVertical() );
    __bVesselSynchronized = true;
  }

  // ... time
  if( __fdTime != CDataTime::UNDEFINED_TIME )
  {
    __poVesselPoint->setTime( __fdTime );
    __poVesselPoint->setValidityTime( __fdSystemTime, __fdErrorTime );
    __bVesselSynchronized = true;
  }

  // ... additional text data
  if( !__qsText.isEmpty() )
  {
    __poVesselPoint->setComment( __qsText );
    __bVesselSynchronized = true;
  }

  // ... [end]
  if( __bVesselSynchronized )
  {
    __poVesselPoint->onDeviceDataFix();
    QVCTRuntime::useChartTable()->setProjectModified();
  }
  emit signalRefreshContent();
}

void CVesselPointDevice::slotDataSkyView( const CDeviceDataSkyView& _roDeviceDataSkyView )
{
  if( !CDeviceDataFix::getSourceName().isEmpty()
      && CDeviceDataFix::getSourceName() != _roDeviceDataSkyView.getSourceName() ) return;
  if( _roDeviceDataSkyView.CDeviceDataDop::operator==( CDeviceDataDop::UNDEFINED ) ) return;
  CDeviceDataFix::setDop( _roDeviceDataSkyView );
  emit signalRefreshContent();
}

//
// SETTERS
//

void CVesselPointDevice::setSynchronized( bool _bSynchronizePosition, bool _bSynchronizeElevation, bool _bSynchronizeTime,
                                          bool _bSynchronizeGroundBearing, bool _bSynchronizeGroundSpeed, bool _bSynchronizeGroundSpeedVertical,
                                          bool _bSynchronizeApparentBearing, bool _bSynchronizeApparentSpeed, bool _bSynchronizeApparentSpeedVertical,
                                          bool _bSynchronizeText )
{
  bSynchronizePosition = _bSynchronizePosition;
  bSynchronizeElevation = _bSynchronizeElevation;
  bSynchronizeTime = _bSynchronizeTime;
  bSynchronizeGroundBearing = _bSynchronizeGroundBearing;
  bSynchronizeGroundSpeed = _bSynchronizeGroundSpeed;
  bSynchronizeGroundSpeedVertical = _bSynchronizeGroundSpeedVertical;
  bSynchronizeApparentBearing = _bSynchronizeApparentBearing;
  bSynchronizeApparentSpeed = _bSynchronizeApparentSpeed;
  bSynchronizeApparentSpeedVertical = _bSynchronizeApparentSpeedVertical;
  bSynchronizeText = _bSynchronizeText;
}

//
// OTHER
//

bool CVesselPointDevice::connectDevice()
{
  if( poDevice ) return true;
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  CDevice* __poDevice = __poDeviceOverlay->pickDevice( COverlayObject::getName() );
  if( !__poDevice ) return false;
  poDevice = __poDevice;
  QObject::connect( poDevice, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
  QObject::connect( poDevice, SIGNAL( signalDataFix(const CDeviceDataFix&) ), this, SLOT( slotDataFix(const CDeviceDataFix&) ) );
  QObject::connect( poDevice, SIGNAL( signalDataSkyView(const CDeviceDataSkyView&) ), this, SLOT( slotDataSkyView(const CDeviceDataSkyView&) ) );
  return true;
}

void CVesselPointDevice::disconnectDevice()
{
  if( !poDevice ) return;
  QObject::disconnect( poDevice, 0, this, 0 );
  poDevice = 0;
}

void CVesselPointDevice::parseQVCT( const QDomElement& _rqDomElement )
{
  bSynchronizePosition = _rqDomElement.attribute( "synch_position", "0" ).toInt();
  bSynchronizeElevation = _rqDomElement.attribute( "synch_elevation", "0" ).toInt();
  bSynchronizeTime = _rqDomElement.attribute( "synch_time", "0" ).toInt();
  bSynchronizeGroundBearing = _rqDomElement.attribute( "synch_ground_bearing", "0" ).toInt();
  bSynchronizeGroundSpeed = _rqDomElement.attribute( "synch_ground_speed", "0" ).toInt();
  bSynchronizeGroundSpeedVertical = _rqDomElement.attribute( "synch_ground_speed_vertical", "0" ).toInt();
  bSynchronizeApparentBearing = _rqDomElement.attribute( "synch_apparent_bearing", "0" ).toInt();
  bSynchronizeApparentSpeed = _rqDomElement.attribute( "synch_apparent_speed", "0" ).toInt();
  bSynchronizeApparentSpeedVertical = _rqDomElement.attribute( "synch_apparent_speed_vertical", "0" ).toInt();
}

void CVesselPointDevice::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{

  // Data
  _rqXmlStreamWriter.writeStartElement( "Device" );
  // ... name
  if( !getName().isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", getName() );
  // ... source name
  if( !getSourceName().isEmpty() ) _rqXmlStreamWriter.writeAttribute( "source", getSourceName() );
  // ... synchronization flags
  _rqXmlStreamWriter.writeAttribute( "synch_position", bSynchronizePosition ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_elevation", bSynchronizeElevation ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_time", bSynchronizeTime ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_ground_bearing", bSynchronizeGroundBearing ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_ground_speed", bSynchronizeGroundSpeed ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_ground_speed_vertical", bSynchronizeGroundSpeedVertical ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_apparent_bearing", bSynchronizeApparentBearing ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_apparent_speed", bSynchronizeApparentSpeed ? "1" : "0" );
  _rqXmlStreamWriter.writeAttribute( "synch_apparent_speed_vertical", bSynchronizeApparentSpeedVertical ? "1" : "0" );
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Device
}
