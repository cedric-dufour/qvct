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
#include "overlays/landmark/CLandmarkPoint.hpp"
#include "overlays/landmark/CLandmarkPointEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkPoint::CLandmarkPoint( const QString& _rqsName )
  : COverlayPoint( _rqsName )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CLandmarkOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::SELECT, Qt::Unchecked );
}

CLandmarkPoint::CLandmarkPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayPoint( _rqsName, _roDataPosition )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsSymbol()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled );
  QTreeWidgetItem::setText( CLandmarkOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::VISIBLE, Qt::Checked );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::SELECT, Qt::Unchecked );
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CLandmarkPoint::serialize( QDataStream& _rqDataStream ) const
{
  COverlayPoint::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsSymbol << qsUrl;
}

void CLandmarkPoint::unserialize( QDataStream& _rqDataStream )
{
  COverlayPoint::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsSymbol >> qsUrl;
  QTreeWidgetItem::setText( CLandmarkOverlay::NAME, qsName );
}

void CLandmarkPoint::draw( const CChart* _poChart, QPainter* _pqPainter )
{
   // Constant drawing resources
  static const QPointF __qPointFSymbol(-32,-32);
  static const QSize __qSizeSymbol(32,32);

  // Draw
  CMainWindow* __poMainWindow = QVCTRuntime::useMainWindow();
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return;
  COverlayPoint::drawMarker( _poChart, _pqPainter );
  if( QVCTRuntime::useSettings()->isVisibleSymbols() && __poMainWindow->symbolExists( qsSymbol ) )
  {
    double __fdZoom = _poChart->getZoom();
    if( __fdZoom > 5.0 ) __fdZoom = 5.0;
    if( __fdZoom > 0.5 ) _pqPainter->drawPixmap( _poChart->toDrawPosition( *this )+__qPointFSymbol*(__fdZoom/2.0), __poMainWindow->symbolPixmap( qsSymbol ).scaled( __qSizeSymbol*(__fdZoom/2.0), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
  }
  COverlayPoint::drawTag( _poChart, _pqPainter );
}

void CLandmarkPoint::showDetail()
{
  QVCTRuntime::useLandmarkPointDetailView()->setOverlayObject( this );
  QVCTRuntime::useLandmarkPointDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::LANDMARK_POINT );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::LANDMARK );
}

void CLandmarkPoint::showEdit()
{
  CLandmarkPointEditView* __poLandmarkPointEditView = new CLandmarkPointEditView( this );
  if( __poLandmarkPointEditView->exec() == QDialog::Accepted )
  {
    showDetail();
    QVCTRuntime::useLandmarkOverlay()->forceRedraw();
    QVCTRuntime::useChartTable()->updateChart();
  }
  delete __poLandmarkPointEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayPoint (implement/override)
//------------------------------------------------------------------------------

bool CLandmarkPoint::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return false;
  return COverlayPoint::matchScrPosition( _poChart, _rqPointFScrPosition );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CLandmarkPoint::toggleMultiSelected()
{
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::SELECT, QTreeWidgetItem::checkState( CLandmarkOverlay::SELECT ) == Qt::Unchecked ? Qt::Checked : Qt::Unchecked );
}

//
// OTHER
//

void CLandmarkPoint::parseQVCT( const QDomElement& _rqDomElement )
{
  if( _rqDomElement.hasAttribute( "longitude" ) && _rqDomElement.hasAttribute( "longitude" ) )
  {
    COverlayPoint::setPosition( _rqDomElement.attribute( "longitude" ).toDouble(),
                                _rqDomElement.attribute( "latitude" ).toDouble(),
                                _rqDomElement.attribute( "elevation", QString::number( CDataPosition::UNDEFINED_ELEVATION ) ).toDouble() );
  }
  qsType = _rqDomElement.attribute( "type" );
  qsSymbol = _rqDomElement.attribute( "symbol" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();
}

void CLandmarkPoint::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  if( _rqDomElement.hasAttribute( "lon" ) && _rqDomElement.hasAttribute( "lat" ) )
  {
    QString __qsElevation = _rqDomElement.firstChildElement( "ele" ).text();
    if( __qsElevation.isEmpty() ) __qsElevation = QString::number( CDataPosition::UNDEFINED_ELEVATION );
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

void CLandmarkPoint::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
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

void CLandmarkPoint::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "wpt" );
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
  _rqXmlStreamWriter.writeEndElement(); // wpt
}
