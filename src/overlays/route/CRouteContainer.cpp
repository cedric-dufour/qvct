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
#include <QFileInfo>
#include <QFileDialog>
#include <QPainter>
#include <QPointF>
#include <QTreeWidgetItem>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/route/CRouteContainer.hpp"
#include "overlays/route/CRouteContainerEditView.hpp"
#include "overlays/route/CRoutePoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CRouteContainer::CRouteContainer( const QString& _rqsName )
  : COverlayContainer( COverlayObject::CONTAINER, _rqsName )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled );
  QTreeWidgetItem::setText( CRouteOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CRouteOverlay::VISIBLE, Qt::Checked );
  // QObject::connect( this, SIGNAL( signalDelete(COverlayObject*) ), QVCTRuntime::useRouteContainerDetailView(), SLOT( slotDelete(COverlayObject*) ) );
}

CRouteContainer::~CRouteContainer()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CRoutePoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CRouteContainer::serialize( QDataStream& _rqDataStream ) const
{
  COverlayObject::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsUrl;
}

void CRouteContainer::unserialize( QDataStream& _rqDataStream )
{
  COverlayObject::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsUrl;
  QTreeWidgetItem::setText( CRouteOverlay::NAME, qsName );
}

void CRouteContainer::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return;
  CRoutePoint* __poRoutePointFrom = (CRoutePoint*)QTreeWidgetItem::child( 0 );
  if( __iCount == 1 )
  {
    __poRoutePointFrom->draw( _poChart, _pqPainter );
    return;
  }
  CRoutePoint* __poRoutePointTo = 0;
  for( int __i = 1; __i < __iCount; __i++ )
  {
    __poRoutePointTo = (CRoutePoint*)QTreeWidgetItem::child( __i );
    __poRoutePointFrom->drawLine( _poChart, _pqPainter, __poRoutePointTo );
    __poRoutePointFrom->draw( _poChart, _pqPainter );
    __poRoutePointFrom = __poRoutePointTo;
  }
  __poRoutePointTo->draw( _poChart, _pqPainter );
}

void CRouteContainer::showDetail()
{
  QVCTRuntime::useRouteContainerDetailView()->setOverlayObject( this );
  QVCTRuntime::useRouteContainerDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::ROUTE_CONTAINER );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::ROUTE );
  QVCTRuntime::useChartTable()->setOverlayObjectSelected( this );
}

void CRouteContainer::showEdit()
{
  CRouteContainerEditView* __poRouteContainerEditView = new CRouteContainerEditView( this );
  if( __poRouteContainerEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poRouteContainerEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayContainer (implement/override)
//------------------------------------------------------------------------------

COverlayPoint* CRouteContainer::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CRouteContainer::draw() to pick the correct (overlapping) item
  {
    COverlayPoint* __poOverlayPoint = (COverlayPoint*)QTreeWidgetItem::child( __i );
    if( __poOverlayPoint->matchScrPosition( _poChart, _rqPointFScrPosition ) )
      return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayVisibility (override)
//------------------------------------------------------------------------------

void CRouteContainer::setVisibleName( bool _bVisibleName )
{
  bVisibleName = _bVisibleName;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CRoutePoint*)QTreeWidgetItem::child( __i ))->setVisibleName( bVisibleName );
}

void CRouteContainer::setVisiblePosition( bool _bVisiblePosition )
{
  bVisiblePosition = _bVisiblePosition;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CRoutePoint*)QTreeWidgetItem::child( __i ))->setVisiblePosition( bVisiblePosition );
}

void CRouteContainer::setVisibleRouting( bool _bVisibleRouting )
{
  bVisibleRouting = _bVisibleRouting;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CRoutePoint*)QTreeWidgetItem::child( __i ))->setVisibleRouting( bVisibleRouting );
}

void CRouteContainer::toggleVisible()
{
  if( !bVisible ) bVisible = true;
  else
  {
    int __iCount = QTreeWidgetItem::childCount();
    // Retrieve points visibility
    bVisibleName = false;
    bVisiblePosition = false;
    bVisibleRouting = false;
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CRoutePoint* __poRoutePoint = (CRoutePoint*)QTreeWidgetItem::child( __i );
      bVisibleName |= __poRoutePoint->isVisibleName();
      bVisiblePosition |= __poRoutePoint->isVisiblePosition();
      bVisibleRouting |= __poRoutePoint->isVisibleRouting();
    }
    // Toggle global visibility
    COverlayVisibility::toggleVisible( false, true );
    // Set points visibility
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CRoutePoint* __poRoutePoint = (CRoutePoint*)QTreeWidgetItem::child( __i );
      __poRoutePoint->setVisibleName( bVisibleName );
      __poRoutePoint->setVisiblePosition( bVisiblePosition );
      __poRoutePoint->setVisibleRouting( bVisibleRouting );
    }
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CRoutePoint* CRouteContainer::addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
{
  CRoutePoint* __poRoutePoint = new CRoutePoint( _rqsName, _roDataPosition );
  QTreeWidgetItem::addChild( __poRoutePoint );
  return __poRoutePoint;
}

double CRouteContainer::getLengthRL()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 2 ) return 0.0;
  CRoutePoint* __poRoutePathFrom = (CRoutePoint*)QTreeWidgetItem::child( 0 );
  CRoutePoint* __poRoutePathTo = 0;
  double __fdLength = 0;
  for( int __i = 1; __i < __iCount; __i++ )
  {
    __poRoutePathTo = (CRoutePoint*)QTreeWidgetItem::child( __i );
    __fdLength += CDataPosition::distanceRL( *__poRoutePathFrom, *__poRoutePathTo );
    __poRoutePathFrom = __poRoutePathTo;
  }
  return __fdLength;
}

int CRouteContainer::parseQVCT( const QDomElement& _rqDomElement )
{
  qsType = _rqDomElement.attribute( "type" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Point" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Point" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( __qsName.isEmpty() ) COverlayObject::newChildName( tr("Waypoint"), 1, __iCount );
    CRoutePoint* __poRoutePoint = new CRoutePoint( __qsName );
    __poRoutePoint->parseQVCT( __qDomElement );
    addChild( __poRoutePoint );
  }
  return __iCount;
}

int CRouteContainer::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  qsType = _rqDomElement.firstChildElement( "type" ).text();
  qsDescription = _rqDomElement.firstChildElement( "desc" ).text();
  qsComment = _rqDomElement.firstChildElement( "cmt" ).text();
  qsUrl = _rqDomElement.firstChildElement( "link" ).attribute( "href" );
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "rtept" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "rtept" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.firstChildElement( "name" ).text();
    if( __qsName.isEmpty() ) COverlayObject::newChildName( tr("Waypoint"), 1, __iCount );
    CRoutePoint* __poRoutePoint = new CRoutePoint( __qsName );
    __poRoutePoint->parseGPX( __qDomElement );
    addChild( __poRoutePoint );
  }
  return __iCount;
}

void CRouteContainer::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // Data
  _rqXmlStreamWriter.writeStartElement( "Route" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... type
  if( !qsType.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "type", qsType );
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
  // ... points
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CRoutePoint* __poRoutePoint = (CRoutePoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poRoutePoint->isMultiSelected() ) continue;
    __poRoutePoint->dumpQVCT( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Route
}

void CRouteContainer::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "rte" );
  // ... name
  if( !qsName.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "name" );
    _rqXmlStreamWriter.writeCharacters( qsName );
    _rqXmlStreamWriter.writeEndElement(); // name
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
  // ... url
  if( !qsUrl.isEmpty() )
  {
    _rqXmlStreamWriter.writeStartElement( "link" );
    _rqXmlStreamWriter.writeAttribute( "href", qsUrl );
    _rqXmlStreamWriter.writeEndElement(); // link
  }
  // ... points
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CRoutePoint* __poRoutePoint = (CRoutePoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poRoutePoint->isMultiSelected() ) continue;
    __poRoutePoint->dumpGPX( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // rte
}

int CRouteContainer::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
  {
    CRoutePoint* __poRoutePoint = (CRoutePoint*)QTreeWidgetItem::child( __i );
    if( __poRoutePoint->isMultiSelected() )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poRoutePoint );
      delete __poRoutePoint;
    }
  }
  return __iCount;
}
