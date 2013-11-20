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
#include <QPen>
#include <QPointF>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "overlays/track/CTrackPoint.hpp"
#include "overlays/track/CTrackSubContainer.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackSubContainer::CTrackSubContainer( const QString& _rqsName )
  : COverlayContainer( COverlayObject::SUBCONTAINER, _rqsName )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CTrackOverlay::VISIBLE, Qt::Checked );
}

CTrackSubContainer::~CTrackSubContainer()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CTrackPoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CTrackSubContainer::unserialize( QDataStream& _rqDataStream )
{
  COverlayObject::unserialize( _rqDataStream );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
}

                                       
//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CTrackSubContainer::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  // Exit if we're not visible
  if( !bVisible ) return;

  // Retrieve and adjust drawing parameters
  double __fdZoom = _poChart->getZoom();
  QPen __qPenMultiSelect = QVCTRuntime::useTrackOverlay()->getPenLine();
  __qPenMultiSelect.setColor( QColor( 0, 192, 0, 192 ) );
  __qPenMultiSelect.setWidth( __qPenMultiSelect.width() * __fdZoom );

  // Draw
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 2 ) return;
  CTrackPoint* __poTrackPointFrom = (CTrackPoint*)QTreeWidgetItem::child( 0 );
  CTrackPoint* __poTrackPointTo = 0;
  for( int __i = 1; __i < __iCount; __i++ )
  {
    __poTrackPointTo = (CTrackPoint*)QTreeWidgetItem::child( __i );
    __poTrackPointFrom->drawLine( _poChart, _pqPainter, __poTrackPointTo );
    if( __poTrackPointFrom->isMultiSelected() )
    {
      _pqPainter->setPen( __qPenMultiSelect );
      _pqPainter->drawPoint( _poChart->toDrawPosition( *__poTrackPointFrom ) );
    }
    __poTrackPointFrom = __poTrackPointTo;
  }
  if( __poTrackPointTo && __poTrackPointFrom->isMultiSelected() )
  {
    _pqPainter->setPen( __qPenMultiSelect );
    _pqPainter->drawPoint( _poChart->toDrawPosition( *__poTrackPointTo ) );
  }
  ((CTrackPoint*)QTreeWidgetItem::child( 0 ))->draw( _poChart, _pqPainter );
  ((CTrackPoint*)QTreeWidgetItem::child( __iCount-1 ))->draw( _poChart, _pqPainter );
}

void CTrackSubContainer::showDetail()
{
  QVCTRuntime::useTrackSubContainerDetailView()->setOverlayObject( this );
  QVCTRuntime::useTrackSubContainerDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::TRACK_SUBCONTAINER );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::TRACK );
}


//------------------------------------------------------------------------------
// METHODS: COverlayContainer (implement/override)
//------------------------------------------------------------------------------

COverlayPoint* CTrackSubContainer::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CTrackSubContainer::draw() to pick the correct (overlapping) item
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

void CTrackSubContainer::setVisibleName( bool _bVisibleName )
{
  bVisibleName = _bVisibleName;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackPoint*)QTreeWidgetItem::child( __i ))->setVisibleName( bVisibleName );
}

void CTrackSubContainer::setVisiblePosition( bool _bVisiblePosition )
{
  bVisiblePosition = _bVisiblePosition;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackPoint*)QTreeWidgetItem::child( __i ))->setVisiblePosition( bVisiblePosition );
}

void CTrackSubContainer::setVisibleRouting( bool _bVisibleRouting )
{
  bVisibleRouting = _bVisibleRouting;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackPoint*)QTreeWidgetItem::child( __i ))->setVisibleRouting( bVisibleRouting );
}

void CTrackSubContainer::toggleVisible()
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
      CTrackPoint* __poTrackPoint = (CTrackPoint*)QTreeWidgetItem::child( __i );
      bVisibleName |= __poTrackPoint->isVisibleName();
      bVisiblePosition |= __poTrackPoint->isVisiblePosition();
      bVisibleRouting |= __poTrackPoint->isVisibleRouting();
    }
    // Toggle global visibility
    COverlayVisibility::toggleVisible( false, true );
    // Set points visibility
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CTrackPoint* __poTrackPoint = (CTrackPoint*)QTreeWidgetItem::child( __i );
      __poTrackPoint->setVisibleName( bVisibleName );
      __poTrackPoint->setVisiblePosition( bVisiblePosition );
      __poTrackPoint->setVisibleRouting( bVisibleRouting );
    }
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CTrackPoint* CTrackSubContainer::getLastPoint()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return 0;
  return (CTrackPoint*)QTreeWidgetItem::child( __iCount-1 );
}

CTrackPoint* CTrackSubContainer::addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
{
  CTrackPoint* __poTrackPoint = new CTrackPoint( _rqsName, _roDataPosition );
  QTreeWidgetItem::addChild( __poTrackPoint );
  return __poTrackPoint;
}

double CTrackSubContainer::getLengthRL()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 2 ) return 0.0;
  CTrackPoint* __poTrackPointFrom = (CTrackPoint*)QTreeWidgetItem::child( 0 );
  CTrackPoint* __poTrackPointTo = 0;
  double __fdLength = 0;
  for( int __i = 1; __i < __iCount; __i++ )
  {
    __poTrackPointTo = (CTrackPoint*)QTreeWidgetItem::child( __i );
    __fdLength += CDataPosition::distanceRL( *__poTrackPointFrom, *__poTrackPointTo );
    __poTrackPointFrom = __poTrackPointTo;
  }
  return __fdLength;
}

double CTrackSubContainer::getTimeElapsed()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 2 ) return 0.0;
  CTrackPoint* __poTrackPointFirst = (CTrackPoint*)QTreeWidgetItem::child( 0 );
  CTrackPoint* __poTrackPointLast = (CTrackPoint*)QTreeWidgetItem::child( __iCount-1 );
  if( __poTrackPointFirst->getTime() < 0 || __poTrackPointLast->getTime() < 0 ) return 0.0;
  return( __poTrackPointLast->getTime() - __poTrackPointFirst->getTime() );
}

int CTrackSubContainer::parseQVCT( const QDomElement& _rqDomElement )
{
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Point" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Point" ) )
  {
    __iCount++;
    QString __qsName;
    QString __qsName_aux = __qDomElement.attribute( "time" );
    if( !__qsName_aux.isEmpty() ) __qsName = CUnitTime::toString( QDateTime::fromString( __qsName_aux, Qt::ISODate ).toTime_t() );
    if( __qsName.isEmpty() ) __qsName = __qDomElement.firstChildElement( "name" ).text();
    if( __qsName.isEmpty() ) __qsName = COverlayObject::newChildName( tr("Point"), 3, __iCount );
    CTrackPoint* __poTrackPoint = new CTrackPoint( __qsName );
    __poTrackPoint->parseQVCT( __qDomElement );
    addChild( __poTrackPoint );
  }
  return __iCount;
}

int CTrackSubContainer::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "trkpt" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "trkpt" ) )
  {
    __iCount++;
    QString __qsName;
    QString __qsName_aux = __qDomElement.firstChildElement( "time" ).text();
    if( !__qsName_aux.isEmpty() ) __qsName = CUnitTime::toString( QDateTime::fromString( __qsName_aux, Qt::ISODate ).toTime_t() );
    if( __qsName.isEmpty() ) __qsName = __qDomElement.firstChildElement( "name" ).text();
    if( __qsName.isEmpty() ) __qsName = COverlayObject::newChildName( tr("Point"), 3, __iCount );
    CTrackPoint* __poTrackPoint = new CTrackPoint( __qsName );
    __poTrackPoint->parseGPX( __qDomElement );
    addChild( __poTrackPoint );
  }
  return __iCount;
}

void CTrackSubContainer::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return;

  // Data
  _rqXmlStreamWriter.writeStartElement( "Segment" );
  // ... points
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CTrackPoint* __poTrackPoint = (CTrackPoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poTrackPoint->isMultiSelected() ) continue;
    __poTrackPoint->dumpQVCT( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Segment
}

void CTrackSubContainer::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount ) return;

  // Data
  _rqXmlStreamWriter.writeStartElement( "trkseg" );
  // ... points
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CTrackPoint* __poTrackPoint = (CTrackPoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poTrackPoint->isMultiSelected() ) continue;
    __poTrackPoint->dumpGPX( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // trkseg
}

int CTrackSubContainer::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
  {
    CTrackPoint* __poTrackPoint = (CTrackPoint*)QTreeWidgetItem::child( __i );
    if( __poTrackPoint->isMultiSelected() )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poTrackPoint );
      delete __poTrackPoint;
    }
  }
  return __iCount;
}
