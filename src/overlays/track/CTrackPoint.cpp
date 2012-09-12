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
#include <QDateTime>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "devices/data/CDeviceDataDop.hpp"
#include "overlays/track/CTrackPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackPoint::CTrackPoint( const QString& _rqsName )
  : COverlayPoint( _rqsName )
  , qsFixType()
  , iSatelliteCount( 0 )
  , fdDopHorizontal( CDeviceDataDop::UNDEFINED_VALUE )
  , fdDopVertical( CDeviceDataDop::UNDEFINED_VALUE )
  , fdDopPositional( CDeviceDataDop::UNDEFINED_VALUE )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CTrackOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CTrackOverlay::SELECT, Qt::Unchecked );
}

CTrackPoint::CTrackPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayPoint( _rqsName, _roDataPosition )
  , qsFixType()
  , iSatelliteCount( 0 )
  , fdDopHorizontal( 0.0 )
  , fdDopVertical( 0.0 )
  , fdDopPositional( 0.0 )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CTrackOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CTrackOverlay::SELECT, Qt::Unchecked );
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CTrackPoint::unserialize( QDataStream& _rqDataStream )
{
  COverlayPoint::unserialize( _rqDataStream );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
}

void CTrackPoint::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  // Draw
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return;
  COverlayPoint::drawTag( _poChart, _pqPainter );
}

void CTrackPoint::showDetail()
{
  QVCTRuntime::useTrackPointDetailView()->setOverlayObject( this );
  QVCTRuntime::useTrackPointDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::TRACK_POINT );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::TRACK );
}


//------------------------------------------------------------------------------
// METHODS: COverlayPoint (implement/override)
//------------------------------------------------------------------------------

bool CTrackPoint::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return false;
  QPointF __qPointF = _poChart->toDrawPosition( *this );
  __qPointF -= _rqPointFScrPosition;
  return( __qPointF.x()*__qPointF.x() + __qPointF.y()*__qPointF.y() <= 25 );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CTrackPoint::toggleMultiSelected()
{
  QTreeWidgetItem::setCheckState( CTrackOverlay::SELECT, QTreeWidgetItem::checkState( CTrackOverlay::SELECT ) == Qt::Unchecked ? Qt::Checked : Qt::Unchecked );
}

//
// OTHER
//

void CTrackPoint::parseQVCT( const QDomElement& _rqDomElement )
{
  if( _rqDomElement.hasAttribute( "longitude" ) && _rqDomElement.hasAttribute( "longitude" ) )
  {
    CDataPosition::setPosition( _rqDomElement.attribute( "longitude" ).toDouble(),
                               _rqDomElement.attribute( "latitude" ).toDouble(),
                                _rqDomElement.attribute( "elevation", QString::number( CDataPosition::UNDEFINED_ELEVATION, 'f' ) ).toDouble() );
  }
  QString __qsTime = _rqDomElement.attribute( "time" );
  if( !__qsTime.isEmpty() )
    CDataTime::setTime( QDateTime::fromString( __qsTime, Qt::ISODate ).toTime_t() );
  qsFixType = _rqDomElement.attribute( "fixtype" );
  iSatelliteCount = _rqDomElement.attribute( "satcount", "0" ).toInt();
  fdDopHorizontal = _rqDomElement.attribute( "hdop", QString::number( CDeviceDataDop::UNDEFINED_VALUE, 'f' ) ).toDouble();
  fdDopVertical = _rqDomElement.attribute( "vdop", QString::number( CDeviceDataDop::UNDEFINED_VALUE, 'f' ) ).toDouble();
  fdDopPositional = _rqDomElement.attribute( "pdop", QString::number( CDeviceDataDop::UNDEFINED_VALUE, 'f' ) ).toDouble();
}

void CTrackPoint::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  if( _rqDomElement.hasAttribute( "lon" ) && _rqDomElement.hasAttribute( "lat" ) )
  {
    QString __qsElevation = _rqDomElement.firstChildElement( "ele" ).text();
    if( __qsElevation.isEmpty() ) __qsElevation = QString::number( CDataPosition::UNDEFINED_ELEVATION, 'f' );
    CDataPosition::setPosition( _rqDomElement.attribute( "lon" ).toDouble(),
                               _rqDomElement.attribute( "lat" ).toDouble(),
                               __qsElevation.toDouble() );
  }
  QString __qsValue = _rqDomElement.firstChildElement( "time" ).text();
  if( !__qsValue.isEmpty() )
    CDataTime::setTime( QDateTime::fromString( __qsValue, Qt::ISODate ).toTime_t() );
  qsFixType = _rqDomElement.firstChildElement( "fix" ).text();
  __qsValue = _rqDomElement.firstChildElement( "sat" ).text();
  if( !__qsValue.isEmpty() ) iSatelliteCount = __qsValue.toInt();
  __qsValue = _rqDomElement.firstChildElement( "hdop" ).text();
  if( !__qsValue.isEmpty() ) fdDopHorizontal = __qsValue.toDouble();
  __qsValue = _rqDomElement.firstChildElement( "vdop" ).text();
  if( !__qsValue.isEmpty() ) fdDopVertical = __qsValue.toDouble();
  __qsValue = _rqDomElement.firstChildElement( "pdop" ).text();
  if( !__qsValue.isEmpty() ) fdDopPositional = __qsValue.toDouble();
}

void CTrackPoint::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{

  // Data
  _rqXmlStreamWriter.writeStartElement( "Point" );
  // ... position
  _rqXmlStreamWriter.writeAttribute( "longitude", QString::number( CDataPosition::getLongitude() ) );
  _rqXmlStreamWriter.writeAttribute( "latitude", QString::number( CDataPosition::getLatitude() ) );
  // ... elevation
  if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    _rqXmlStreamWriter.writeAttribute( "elevation", QString::number( CDataPosition::getElevation() ) );
  // ... time
  if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME )
    _rqXmlStreamWriter.writeAttribute( "time", QDateTime::fromTime_t( CDataTime::getTime() ).toUTC().toString( Qt::ISODate )+"Z" );
  // ... fix type
  if( !qsFixType.isEmpty() )
    _rqXmlStreamWriter.writeAttribute( "fixtype", qsFixType );
  // ... satellite count
  if( iSatelliteCount > 0 )
    _rqXmlStreamWriter.writeAttribute( "satcount", QString::number( iSatelliteCount ) );
  // ... horizontal Dilution-of-Precision (HDOP)
  if( fdDopHorizontal != CDeviceDataDop::UNDEFINED_VALUE )
    _rqXmlStreamWriter.writeAttribute( "hdop", QString::number( fdDopHorizontal, 'f', 3 ) );
  // ... vertical Dilution-of-Precision (VDOP)
  if( fdDopVertical != CDeviceDataDop::UNDEFINED_VALUE )
    _rqXmlStreamWriter.writeAttribute( "vdop", QString::number( fdDopVertical, 'f', 3 ) );
  // ... positional Dilution-of-Precision (PDOP)
  if( fdDopPositional != CDeviceDataDop::UNDEFINED_VALUE )
    _rqXmlStreamWriter.writeAttribute( "pdop", QString::number( fdDopPositional, 'f', 3 ) );
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Point
}

void CTrackPoint::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "trkpt" );
  // ... position
  _rqXmlStreamWriter.writeAttribute( "lon", QString::number( CDataPosition::getLongitude() ) );
  _rqXmlStreamWriter.writeAttribute( "lat", QString::number( CDataPosition::getLatitude() ) );
  // ... elevation
  if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
  {
    _rqXmlStreamWriter.writeStartElement( "ele" );
    _rqXmlStreamWriter.writeCharacters( QString::number( CDataPosition::getElevation() ) );
    _rqXmlStreamWriter.writeEndElement(); // ele
  }
  // ... time
  if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME )
  {
    _rqXmlStreamWriter.writeStartElement( "time" );
    _rqXmlStreamWriter.writeCharacters( QDateTime::fromTime_t( CDataTime::getTime() ).toUTC().toString( Qt::ISODate )+"Z" );
    _rqXmlStreamWriter.writeEndElement(); // time
  }
  // ... fix type
  if( !qsFixType.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "fix" );
    _rqXmlStreamWriter.writeCharacters( qsFixType );
    _rqXmlStreamWriter.writeEndElement(); // fix
  }
  // ... satellite count
  if( iSatelliteCount > 0 )
  {
    _rqXmlStreamWriter.writeStartElement( "sat" );
    _rqXmlStreamWriter.writeCharacters( QString::number( iSatelliteCount ) );
    _rqXmlStreamWriter.writeEndElement(); // sat
  }
  // ... horizontal Dilution-of-Precision (HDOP)
  if( fdDopHorizontal != CDeviceDataDop::UNDEFINED_VALUE )
  {
    _rqXmlStreamWriter.writeStartElement( "hdop" );
    _rqXmlStreamWriter.writeCharacters( QString::number( fdDopHorizontal, 'f', 3 ) );
    _rqXmlStreamWriter.writeEndElement(); // hdop
  }
  // ... vertical Dilution-of-Precision (VDOP)
  if( fdDopVertical != CDeviceDataDop::UNDEFINED_VALUE )
  {
    _rqXmlStreamWriter.writeStartElement( "vdop" );
    _rqXmlStreamWriter.writeCharacters( QString::number( fdDopVertical, 'f', 3 ) );
    _rqXmlStreamWriter.writeEndElement(); // vdop
  }
  // ... positional Dilution-of-Precision (PDOP)
  if( fdDopPositional != CDeviceDataDop::UNDEFINED_VALUE )
  {
    _rqXmlStreamWriter.writeStartElement( "pdop" );
    _rqXmlStreamWriter.writeCharacters( QString::number( fdDopPositional, 'f', 3 ) );
    _rqXmlStreamWriter.writeEndElement(); // pdop
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // trkpt
}
