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
#include <QPainter>
#include <QPointF>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "overlays/route/CRoutePoint.hpp"
#include "overlays/route/CRoutePointEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRoutePoint::CRoutePoint( const QString& _rqsName )
  : COverlayPoint( _rqsName )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CRouteOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CRouteOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CRouteOverlay::SELECT, Qt::Unchecked );
}

CRoutePoint::CRoutePoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayPoint( _rqsName, _roDataPosition )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CRouteOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CRouteOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CRouteOverlay::SELECT, Qt::Unchecked );
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CRoutePoint::serialize( QDataStream& _rqDataStream ) const
{
  COverlayPoint::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsSymbol << qsUrl;
}

void CRoutePoint::unserialize( QDataStream& _rqDataStream )
{
  COverlayPoint::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsSymbol >> qsUrl;
  QTreeWidgetItem::setText( CRouteOverlay::NAME, qsName );
}

void CRoutePoint::draw( const CChart* _poChart, QPainter* _pqPainter )
{
   // Constant drawing resources
  static const QPointF __qPointFSymbol(-32,-32);
  static const QSize __qSizeSymbol(32,32);

  // Draw
  if( CDataPosition::operator==( CDataPosition::UNDEFINED ) || !bVisible
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return;
  COverlayPoint::drawMarker( _poChart, _pqPainter );
  if( QVCTRuntime::useSettings()->isVisibleSymbols() ) COverlayPoint::drawSymbol( _poChart, _pqPainter, qsSymbol );
  COverlayPoint::drawTag( _poChart, _pqPainter );
}

void CRoutePoint::showDetail()
{
  QVCTRuntime::useRoutePointDetailView()->setOverlayObject( this );
  QVCTRuntime::useRoutePointDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::ROUTE_POINT );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::ROUTE );
}

void CRoutePoint::showEdit()
{
  CRoutePointEditView* __poRoutePointEditView = new CRoutePointEditView( this );
  if( __poRoutePointEditView->exec() == QDialog::Accepted )
  {
    showDetail();
    QVCTRuntime::useRouteOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
  delete __poRoutePointEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayPoint (implement/override)
//------------------------------------------------------------------------------

bool CRoutePoint::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( CDataPosition::operator==( CDataPosition::UNDEFINED ) || !bVisible
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return false;
  return COverlayPoint::matchScrPosition( _poChart, _rqPointFScrPosition );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CRoutePoint::toggleMultiSelected()
{
  QTreeWidgetItem::setCheckState( CRouteOverlay::SELECT, QTreeWidgetItem::checkState( CRouteOverlay::SELECT ) == Qt::Unchecked ? Qt::Checked : Qt::Unchecked );
}

//
// OTHER
//

void CRoutePoint::parseQVCT( const QDomElement& _rqDomElement )
{
  if( _rqDomElement.hasAttribute( "longitude" ) && _rqDomElement.hasAttribute( "longitude" ) )
  {
    COverlayPoint::setPosition( _rqDomElement.attribute( "longitude" ).toDouble(),
                                _rqDomElement.attribute( "latitude" ).toDouble(),
                                _rqDomElement.attribute( "elevation", QString::number( CDataPosition::UNDEFINED_ELEVATION, 'f' ) ).toDouble() );
  }
  qsType = _rqDomElement.attribute( "type" );
  qsSymbol = _rqDomElement.attribute( "symbol" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();
}

void CRoutePoint::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  if( _rqDomElement.hasAttribute( "lon" ) && _rqDomElement.hasAttribute( "lat" ) )
  {
    QString __qsElevation = _rqDomElement.firstChildElement( "ele" ).text();
    if( __qsElevation.isEmpty() ) __qsElevation = QString::number( CDataPosition::UNDEFINED_ELEVATION, 'f' );
    COverlayPoint::setPosition( _rqDomElement.attribute( "lon" ).toDouble(),
                                _rqDomElement.attribute( "lat" ).toDouble(),
                                __qsElevation.toDouble() );
  }
  qsType = _rqDomElement.firstChildElement( "type" ).text();
  qsDescription = _rqDomElement.firstChildElement( "desc" ).text();
  qsComment = _rqDomElement.firstChildElement( "cmt" ).text();
  qsSymbol = _rqDomElement.firstChildElement( "sym" ).text();
  qsUrl = _rqDomElement.firstChildElement( "link" ).attribute( "href" );
}

void CRoutePoint::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{

  // Data
  _rqXmlStreamWriter.writeStartElement( "Point" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... position
  _rqXmlStreamWriter.writeAttribute( "longitude", QString::number( CDataPosition::getLongitude() ) );
  _rqXmlStreamWriter.writeAttribute( "latitude", QString::number( CDataPosition::getLatitude() ) );
  // ... elevation
  if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    _rqXmlStreamWriter.writeAttribute( "elevation", QString::number( CDataPosition::getElevation() ) );
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
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Point
}

void CRoutePoint::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "rtept" );
  // ... position
  _rqXmlStreamWriter.writeAttribute( "lon", QString::number( CDataPosition::getLongitude() ) );
  _rqXmlStreamWriter.writeAttribute( "lat", QString::number( CDataPosition::getLatitude() ) );
  // ... name
  if( !qsName.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "name" );
    _rqXmlStreamWriter.writeCharacters( qsName );
    _rqXmlStreamWriter.writeEndElement(); // name
  }
  // ... elevation
  if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
  {
    _rqXmlStreamWriter.writeStartElement( "ele" );
    _rqXmlStreamWriter.writeCharacters( QString::number( CDataPosition::getElevation() ) );
    _rqXmlStreamWriter.writeEndElement(); // ele
  }
  // ... type
  if( !qsType.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "type" );
    _rqXmlStreamWriter.writeCharacters( qsType );
    _rqXmlStreamWriter.writeEndElement(); // type
  }
  // ... description
  if( !qsDescription.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "desc" );
    _rqXmlStreamWriter.writeCDATA( qsDescription );
    _rqXmlStreamWriter.writeEndElement(); // desc
  }
  // ... comment
  if( !qsComment.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "cmt" );
    _rqXmlStreamWriter.writeCDATA( qsComment );
    _rqXmlStreamWriter.writeEndElement(); // cmt
  }
  // ... symbol
  if( !qsSymbol.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "sym" );
    _rqXmlStreamWriter.writeCharacters( qsSymbol );
    _rqXmlStreamWriter.writeEndElement(); // sym
  }
  // ... url
  if( !qsUrl.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "link" );
    _rqXmlStreamWriter.writeAttribute( "href", qsUrl );
    _rqXmlStreamWriter.writeEndElement(); // link
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // rtept
}
