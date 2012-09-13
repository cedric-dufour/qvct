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
#include "charts/CChart.hpp"
#include "devices/data/CDeviceDataFix.hpp"
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/track/CTrackSubContainer.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/CVesselPointEditView.hpp"
#include "overlays/vessel/device/CVesselPointDevice.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPoint::CVesselPoint( const QString& _rqsName, bool _bDynamic )
  : COverlayCourse( _rqsName )
  , CDataPositionValidity()
  , CDataCourseValidityGA()
  , bDynamic( _bDynamic )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
  , eFixType( CDeviceDataFix::FIX_UNDEFINED )
  , iSatelliteCount( 0 )
  , fdDopHorizontal( CDeviceDataDop::UNDEFINED_VALUE )
  , fdDopVertical( CDeviceDataDop::UNDEFINED_VALUE )
  , iTrackRecordRate( 1 )
  , bTrackRecord (false )
  , tLastTrackRecord ( 0 )
  , fdLastTrackBearing ( CDataCourse::UNDEFINED_BEARING )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CVesselOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CVesselOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CVesselOverlay::SELECT, Qt::Unchecked );
}

CVesselPoint::CVesselPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayCourse( _rqsName, _roDataPosition )
  , CDataPositionValidity()
  , CDataCourseValidityGA()
  , bDynamic( false )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CVesselOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CVesselOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CVesselOverlay::SELECT, Qt::Unchecked );
}

CVesselPoint::~CVesselPoint()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CVesselPointDevice*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}

//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CVesselPoint::serialize( QDataStream& _rqDataStream ) const
{
  COverlayCourse::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsSymbol << qsUrl;
}

void CVesselPoint::unserialize( QDataStream& _rqDataStream )
{
  COverlayCourse::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsSymbol >> qsUrl;
  QTreeWidgetItem::setText( CVesselOverlay::NAME, qsName );
}

void CVesselPoint::draw( const CChart* _poChart, QPainter* _pqPainter )
{
   // Constant drawing resources
  static const QPointF __qPointFSymbol(-32,-32);
  static const QSize __qSizeSymbol(32,32);

  // Draw
  CMainWindow* __poMainWindow = QVCTRuntime::useMainWindow();
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return;
  double __fdGroundSpeed = CDataCourseGA::GroundCourse.getSpeed();
  if( __fdGroundSpeed != CDataCourse::UNDEFINED_SPEED && __fdGroundSpeed >= QVCTRuntime::useSettings()->getMinValueSpeed() )
    COverlayCourse::drawVector( _poChart, _pqPainter, this );
  COverlayCourse::drawMarker( _poChart, _pqPainter, this );
  if( QVCTRuntime::useSettings()->isVisibleSymbols() && __poMainWindow->symbolExists( qsSymbol ) )
  {
    double __fdZoom = _poChart->getZoom();
    if( __fdZoom > 0.5 ) _pqPainter->drawPixmap( _poChart->toDrawPosition( *this )+__qPointFSymbol*(__fdZoom/2.0), __poMainWindow->symbolPixmap( qsSymbol ).scaled( __qSizeSymbol*(__fdZoom/2.0), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
  }
  double __fdGroundBearing = CDataCourseGA::GroundCourse.getBearing();
  if( __fdGroundBearing == CDataCourse::UNDEFINED_SPEED ) __fdGroundBearing = 0;
  COverlayCourse::drawTag( _poChart, _pqPainter, __fdGroundBearing < 180 ? COverlayPoint::TAG_LEFT : COverlayPoint::TAG_RIGHT, this, this );
}

void CVesselPoint::showDetail()
{
  QVCTRuntime::useVesselPointDetailView()->setOverlayObject( this );
  QVCTRuntime::useVesselPointDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::VESSEL_POINT );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::VESSEL );
}

void CVesselPoint::showEdit()
{
  CVesselPointEditView* __poVesselPointEditView = new CVesselPointEditView( this );
  if( __poVesselPointEditView->exec() == QDialog::Accepted )
  {
    showDetail();
    QVCTRuntime::useVesselOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
  delete __poVesselPointEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayPoint (implement/override)
//------------------------------------------------------------------------------

bool CVesselPoint::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return false;
  return COverlayCourse::matchScrPosition( _poChart, _rqPointFScrPosition );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTERS
//

bool CVesselPoint::isDeviceConnected() const
{
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return false;
  for( int __i = 0; __i < __iCount; __i++ )
    if( !((CVesselPointDevice*)QTreeWidgetItem::child( __i ))->isDeviceConnected() ) return false;
  return true;
}

//
// SETTERS
//

void CVesselPoint::toggleMultiSelected()
{
  QTreeWidgetItem::setCheckState( CVesselOverlay::SELECT, QTreeWidgetItem::checkState( CVesselOverlay::SELECT ) == Qt::Unchecked ? Qt::Checked : Qt::Unchecked );
}

bool CVesselPoint::setTrackRecord( bool _bTrackRecord )
{
  if( _bTrackRecord == bTrackRecord ) return bTrackRecord;
  if( _bTrackRecord )
  {
    CTrackContainer* __poTrackContainer = QVCTRuntime::useTrackOverlay()->pickContainer( COverlayObject::getName() );
    if( !__poTrackContainer )
    {
      qCritical( "ERROR[%s]: Failed to create new track for recording", Q_FUNC_INFO );
      return false;
    }
    CTrackSubContainer* __poTrackSubContainer = __poTrackContainer->addSubContainer();
    if( !__poTrackSubContainer )
    {
      qCritical( "ERROR[%s]: Failed to create new segment for recording", Q_FUNC_INFO );
      return false;
    }
  }
  bTrackRecord = _bTrackRecord;
  fdLastTrackBearing = CDataCourse::UNDEFINED_BEARING;
  return bTrackRecord;
}

//
// OTHER
//

void CVesselPoint::connectDevice()
{
  if( !bDynamic ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CVesselPointDevice*)QTreeWidgetItem::child( __i ))->connectDevice();
}

void CVesselPoint::disconnectDevice()
{
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CVesselPointDevice*)QTreeWidgetItem::child( __i ))->disconnectDevice();
}

void CVesselPoint::onDeviceDataFix()
{
  // Synchronize chart center position
  if( this->CDataPosition::operator!=( CDataPosition::UNDEFINED )
      && QVCTRuntime::useChartTable()->getVesselPointSynchronize() == this )
    QVCTRuntime::useChartTable()->synchronizeVesselPoint();

  // Track recording
  time_t __tCurrentTime = time( 0 );
  if( bTrackRecord
      && this->CDataPosition::operator!=( CDataPosition::UNDEFINED )
      && this->CDataTime::operator!=( CDataTime::UNDEFINED )
      && difftime( __tCurrentTime, tLastTrackRecord ) >= (double)iTrackRecordRate )
  {
    CSettings* __poSettings = QVCTRuntime::useSettings();

    do // error-catching loop
    {
      // ... retrieve current track
      CTrackSubContainer* __poTrackSubContainer = QVCTRuntime::useTrackOverlay()->pickSubContainer( COverlayObject::getName() );

      // ... check distance and bearing delta
      bool __bSkipBearing = false;
      if( CDataCourseGA::GroundCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
      {
        if( fdLastTrackBearing != CDataCourse::UNDEFINED_BEARING )
        {
          double __fdDeltaBearing = fabs( CDataCourseGA::GroundCourse.getBearing() - fdLastTrackBearing );
          if( __fdDeltaBearing > 180.0 ) __fdDeltaBearing = 360 - __fdDeltaBearing;
          if( __fdDeltaBearing < __poSettings->getMinValueBearing() ) __bSkipBearing = true;
        }
        fdLastTrackBearing = CDataCourseGA::GroundCourse.getBearing();
      }
      bool __bSkipDistance = false;
      CTrackPoint* __poTrackPointLast = __poTrackSubContainer->getLastPoint();
      if( __poTrackPointLast
          && CDataPosition::distanceRL( *__poTrackPointLast, *this ) < __poSettings->getMinValuePosition() ) __bSkipDistance = true;
      if( __bSkipBearing && __bSkipDistance ) break;

      // ... add new point
      QString __qsName;
      if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME ) __qsName = CUnitTime::toString( CDataTime::getTime() );
      if( __qsName.isEmpty() ) __qsName = __poTrackSubContainer->newChildName( tr("Point"), 3, true );
      CTrackPoint* __poTrackPoint = __poTrackSubContainer->addPoint( __qsName, *this );
      __poTrackPoint->setTime( CDataTime::getTime() );
      if( eFixType != CDeviceDataFix::FIX_UNDEFINED ) __poTrackPoint->setFixType( CDeviceDataFix::getTypeString( eFixType ) );
      __poTrackPoint->setSatelliteCount( iSatelliteCount );
      __poTrackPoint->setDopHorizontal( fdDopHorizontal );
      __poTrackPoint->setDopVertical( fdDopVertical );
      if( fdDopHorizontal != CDeviceDataDop::UNDEFINED_VALUE && fdDopVertical != CDeviceDataDop::UNDEFINED_VALUE )
        __poTrackPoint->setDopPositional( sqrt( fdDopHorizontal*fdDopHorizontal + fdDopVertical*fdDopVertical ) );
      QVCTRuntime::useTrackOverlay()->forceRedraw();

      // ... update track recording time
      tLastTrackRecord = __tCurrentTime;
    }
    while( false ); // error-catching loop
  }

  // Redraw
  QVCTRuntime::useVesselOverlay()->forceRedraw();
  // NOTE: Vessel detail view and chart overlays are automatically refreshed by CMainWindow::slotTimerRefreshContent()
}

void CVesselPoint::parseQVCT( const QDomElement& _rqDomElement )
{
  // ... position
  if( _rqDomElement.hasAttribute( "longitude" ) && _rqDomElement.hasAttribute( "longitude" ) )
  {
    CDataPosition::setPosition( _rqDomElement.attribute( "longitude" ).toDouble(),
                                _rqDomElement.attribute( "latitude" ).toDouble(),
                                _rqDomElement.attribute( "elevation", QString::number( CDataPosition::UNDEFINED_ELEVATION, 'f' ) ).toDouble() );
  }
  QString __qsTime = _rqDomElement.attribute( "time" );
  if( !__qsTime.isEmpty() )
  {
    CDataTime::setTime( QDateTime::fromString( __qsTime, Qt::ISODate ).toTime_t() );
  }

  // ... info
  qsType = _rqDomElement.attribute( "type" );
  qsSymbol = _rqDomElement.attribute( "symbol" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();

  // ... course
  QDomElement __qDomElementCourse = _rqDomElement.firstChildElement( "Course" );
  if( !__qDomElementCourse.isNull() )
  {
    QDomElement __qDomElement = __qDomElementCourse.firstChildElement( "Ground" );
    if( !__qDomElement.isNull() )
    {
      CDataCourseGA::GroundCourse.setBearing( __qDomElement.attribute( "bearing", QString::number( CDataCourse::UNDEFINED_BEARING, 'f' ) ).toDouble() );
      CDataCourseGA::GroundCourse.setSpeed( __qDomElement.attribute( "speed", QString::number( CDataCourse::UNDEFINED_SPEED, 'f' ) ).toDouble() );
      CDataCourseGA::GroundCourse.setSpeedVertical( __qDomElement.attribute( "speed_vertical", QString::number( CDataCourse::UNDEFINED_SPEED, 'f' ) ).toDouble() );
    }
    __qDomElement = __qDomElementCourse.firstChildElement( "Apparent" );
    if( !__qDomElement.isNull() )
    {
      CDataCourseGA::ApparentCourse.setBearing( __qDomElement.attribute( "bearing", QString::number( CDataCourse::UNDEFINED_BEARING, 'f' ) ).toDouble() );
      CDataCourseGA::ApparentCourse.setSpeed( __qDomElement.attribute( "speed", QString::number( CDataCourse::UNDEFINED_SPEED, 'f' ) ).toDouble() );
      CDataCourseGA::ApparentCourse.setSpeedVertical( __qDomElement.attribute( "speed_vertical", QString::number( CDataCourse::UNDEFINED_SPEED, 'f' ) ).toDouble() );
    }
  }

  // ... validity
  CDataPositionValidity::setValidityPosition( CDataPositionValidity::UNDEFINED_VALUE, CDataPositionValidity::UNDEFINED_VALUE, true );
  CDataPositionValidity::setValidityElevation( CDataPositionValidity::UNDEFINED_VALUE, CDataPositionValidity::UNDEFINED_VALUE, true );
  CDataTimeValidity::setValidityTime( CDataTimeValidity::UNDEFINED_VALUE, CDataTimeValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::GroundCourseValidity.setValidityBearing( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::GroundCourseValidity.setValiditySpeed( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::GroundCourseValidity.setValiditySpeedVertical( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::ApparentCourseValidity.setValidityBearing( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::ApparentCourseValidity.setValiditySpeed( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );
  CDataCourseValidityGA::ApparentCourseValidity.setValiditySpeedVertical( CDataCourseValidity::UNDEFINED_VALUE, CDataCourseValidity::UNDEFINED_VALUE, true );

  // ... devices
  for( QDomElement __qDomElementDevice = _rqDomElement.firstChildElement( "Device" );
       !__qDomElementDevice.isNull();
       __qDomElementDevice = __qDomElementDevice.nextSiblingElement( "Device" ) )
  {
    QString __qsName = __qDomElementDevice.attribute( "name" );
    QString __qsSourceName = __qDomElementDevice.attribute( "source" );
    CVesselPointDevice* __poVesselPointDevice = new CVesselPointDevice( __qsName, __qsSourceName );
    if( __poVesselPointDevice ) __poVesselPointDevice->parseQVCT( __qDomElementDevice );
    QTreeWidgetItem::addChild( __poVesselPointDevice );
    __poVesselPointDevice->connectDevice();
  }
}

void CVesselPoint::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{

  // Data
  _rqXmlStreamWriter.writeStartElement( "Vessel" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... position
  _rqXmlStreamWriter.writeAttribute( "longitude", QString::number( CDataPosition::getLongitude() ) );
  _rqXmlStreamWriter.writeAttribute( "latitude", QString::number( CDataPosition::getLatitude() ) );
  // ... elevation
  if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    _rqXmlStreamWriter.writeAttribute( "elevation", QString::number( CDataPosition::getElevation() ) );
  // ... time
  if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME )
    _rqXmlStreamWriter.writeAttribute( "time", QDateTime::fromTime_t( CDataTime::getTime() ).toUTC().toString( Qt::ISODate )+"Z" );
  // ... type
  if( !qsType.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "type", qsType );
  // ... symbol
  if( !qsSymbol.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "symbol", qsSymbol );
  // ... url
  if( !qsUrl.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "url", qsUrl );
  // ... description
  if( !qsDescription.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "Description" );
    _rqXmlStreamWriter.writeCDATA( qsDescription );
    _rqXmlStreamWriter.writeEndElement(); // Description
  }
  // ... comment
  if( !qsComment.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "Comment" );
    _rqXmlStreamWriter.writeCDATA( qsComment );
    _rqXmlStreamWriter.writeEndElement(); // Comment
  }
  // ... course
  if( CDataCourseGA::GroundCourse != CDataCourse::UNDEFINED || CDataCourseGA::ApparentCourse != CDataCourse::UNDEFINED )
  {
    _rqXmlStreamWriter.writeStartElement( "Course" );
    if( CDataCourseGA::GroundCourse != CDataCourse::UNDEFINED )
    {
      _rqXmlStreamWriter.writeStartElement( "Ground" );
      if( CDataCourseGA::GroundCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
        _rqXmlStreamWriter.writeAttribute( "bearing", QString::number( CDataCourseGA::GroundCourse.getBearing() ) );
      if( CDataCourseGA::GroundCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
        _rqXmlStreamWriter.writeAttribute( "speed", QString::number( CDataCourseGA::GroundCourse.getSpeed() ) );
      if( CDataCourseGA::GroundCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
        _rqXmlStreamWriter.writeAttribute( "speed_vertical", QString::number( CDataCourseGA::GroundCourse.getSpeedVertical() ) );
    _rqXmlStreamWriter.writeEndElement(); // Ground
    }
    if( CDataCourseGA::ApparentCourse != CDataCourse::UNDEFINED )
    {
      _rqXmlStreamWriter.writeStartElement( "Apparent" );
      if( CDataCourseGA::ApparentCourse.getBearing() != CDataCourse::UNDEFINED_BEARING )
        _rqXmlStreamWriter.writeAttribute( "bearing", QString::number( CDataCourseGA::ApparentCourse.getBearing() ) );
      if( CDataCourseGA::ApparentCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED )
        _rqXmlStreamWriter.writeAttribute( "speed", QString::number( CDataCourseGA::ApparentCourse.getSpeed() ) );
      if( CDataCourseGA::ApparentCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED )
        _rqXmlStreamWriter.writeAttribute( "speed_vertical", QString::number( CDataCourseGA::ApparentCourse.getSpeedVertical() ) );
      _rqXmlStreamWriter.writeEndElement(); // Apparent
    }
    _rqXmlStreamWriter.writeEndElement(); // Course
  }
  // ... devices
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CVesselPointDevice* __poVesselPointDevice = (CVesselPointDevice*)QTreeWidgetItem::child( __i );
    __poVesselPointDevice->dumpQVCT( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Vessel
}
