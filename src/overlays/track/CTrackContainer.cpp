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
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/track/CTrackContainer.hpp"
#include "overlays/track/CTrackContainerEditView.hpp"
#include "overlays/track/CTrackSubContainer.hpp"
#include "overlays/track/CTrackPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CTrackContainer::CTrackContainer( const QString& _rqsName )
  : COverlayContainer( COverlayObject::CONTAINER, _rqsName )
  , qsType()
  , qsDescription()
  , qsComment()
  , qsUrl()
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CTrackOverlay::VISIBLE, Qt::Checked );
}

CTrackContainer::~CTrackContainer()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CTrackSubContainer*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CTrackContainer::serialize( QDataStream& _rqDataStream ) const
{
  COverlayObject::serialize( _rqDataStream );
  _rqDataStream << qsType << qsDescription << qsComment << qsUrl;
}

void CTrackContainer::unserialize( QDataStream& _rqDataStream )
{
  COverlayObject::unserialize( _rqDataStream );
  _rqDataStream >> qsType >> qsDescription >> qsComment >> qsUrl;
  QTreeWidgetItem::setText( CTrackOverlay::NAME, qsName );
}

void CTrackContainer::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CTrackContainer::showDetail()
{
  QVCTRuntime::useTrackContainerDetailView()->setOverlayObject( this );
  QVCTRuntime::useTrackContainerDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::TRACK_CONTAINER );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::TRACK );
  QVCTRuntime::useChartTable()->setOverlayObjectSelected( this );
}

void CTrackContainer::showEdit()
{
  CTrackContainerEditView* __poTrackContainerEditView = new CTrackContainerEditView( this );
  if( __poTrackContainerEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poTrackContainerEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayContainer (implement/override)
//------------------------------------------------------------------------------

COverlayPoint* CTrackContainer::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CTrackOverlay::draw() to pick the correct (overlapping) item
  {
    CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)QTreeWidgetItem::child( __i );
    COverlayPoint* __poOverlayPoint = __poTrackSubContainer->matchScrPosition( _poChart, _rqPointFScrPosition );
    if( __poOverlayPoint ) return __poOverlayPoint;
  }
  return 0;
}


//------------------------------------------------------------------------------
// METHODS: COverlayVisibility (override)
//------------------------------------------------------------------------------

void CTrackContainer::setVisibleName( bool _bVisibleName )
{
  bVisibleName = _bVisibleName;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->setVisibleName( bVisibleName );
}

void CTrackContainer::setVisiblePosition( bool _bVisiblePosition )
{
  bVisiblePosition = _bVisiblePosition;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->setVisiblePosition( bVisiblePosition );
}

void CTrackContainer::setVisibleRouting( bool _bVisibleRouting )
{
  bVisibleRouting = _bVisibleRouting;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->setVisibleRouting( bVisibleRouting );
}

void CTrackContainer::toggleVisible()
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
      CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)QTreeWidgetItem::child( __i );
      int __iCount_aux = __poTrackSubContainer->childCount();
      for( int __j = 0; __j < __iCount_aux; __j++ )
      {
        CTrackPoint* __poTrackPoint = (CTrackPoint*)__poTrackSubContainer->child( __j );
        bVisibleName |= __poTrackPoint->isVisibleName();
        bVisiblePosition |= __poTrackPoint->isVisiblePosition();
        bVisibleRouting |= __poTrackPoint->isVisibleRouting();
      }
    }
    // Toggle global visibility
    COverlayVisibility::toggleVisible( false, true );
    // Set points visibility
    for( int __i = 0; __i < __iCount; __i++ )
    {
      CTrackSubContainer* __poTrackSubContainer = (CTrackSubContainer*)QTreeWidgetItem::child( __i );
      __poTrackSubContainer->setVisibleName( bVisibleName );
      __poTrackSubContainer->setVisiblePosition( bVisiblePosition );
      __poTrackSubContainer->setVisibleRouting( bVisibleRouting );
    }
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CTrackSubContainer* CTrackContainer::pickSubContainer()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount ) return (CTrackSubContainer*)QTreeWidgetItem::child( __iCount-1 );
  return addSubContainer();
}

CTrackSubContainer* CTrackContainer::addSubContainer()
{
  CTrackSubContainer* __poTrackSubContainer = 0;
  int __iCount = QTreeWidgetItem::childCount();
  if( !__iCount || QTreeWidgetItem::child( __iCount-1 )->childCount() )
  {
    __poTrackSubContainer = new CTrackSubContainer( COverlayObject::newChildName( tr("Segment"), 1, true ) );
    QTreeWidgetItem::addChild( __poTrackSubContainer );
  }
  else __poTrackSubContainer = (CTrackSubContainer*)QTreeWidgetItem::child( __iCount-1 );
  return __poTrackSubContainer;
}

double CTrackContainer::getLengthRL()
{
  double __fdLength = 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    __fdLength += ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->getLengthRL();
  return __fdLength;
}

double CTrackContainer::getTimeElapsed()
{
  double __fdTimeElapsed = 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    __fdTimeElapsed += ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->getTimeElapsed();
  return __fdTimeElapsed;
}

int CTrackContainer::parseQVCT( const QDomElement& _rqDomElement )
{
  COverlayVisibility::setVisibility( _rqDomElement.attribute( "visibility", "3" ).toInt() );
  QTreeWidgetItem::setCheckState( CTrackOverlay::VISIBLE, bVisible ? Qt::Checked : Qt::Unchecked );
  qsType = _rqDomElement.attribute( "type" );
  qsUrl = _rqDomElement.attribute( "url" );
  qsDescription = _rqDomElement.firstChildElement( "Description" ).text();
  qsComment = _rqDomElement.firstChildElement( "Comment" ).text();
  int __iCount = 0, __iCountSegment = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Segment" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Segment" ) )
  {
    __iCountSegment++;
    CTrackSubContainer* __poTrackSubContainer = new CTrackSubContainer( COverlayObject::newChildName( tr("Segment"), 1, __iCountSegment ) );
    __iCount += __poTrackSubContainer->parseQVCT( __qDomElement );
    addChild( __poTrackSubContainer );
  }
  return __iCount;
}

int CTrackContainer::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  qsType = _rqDomElement.firstChildElement( "type" ).text();
  qsDescription = _rqDomElement.firstChildElement( "desc" ).text();
  qsComment = _rqDomElement.firstChildElement( "cmt" ).text();
  qsUrl = _rqDomElement.firstChildElement( "link" ).attribute( "href" );
  int __iCount = 0, __iCountSegment = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "trkseg" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "trkseg" ) )
  {
    __iCountSegment++;
    CTrackSubContainer* __poTrackSubContainer = new CTrackSubContainer( COverlayObject::newChildName( tr("Segment"), 1, __iCountSegment ) );
    __iCount += __poTrackSubContainer->parseGPX( __qDomElement );
    addChild( __poTrackSubContainer );
  }
  return __iCount;
}

void CTrackContainer::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // Data
  _rqXmlStreamWriter.writeStartElement( "Track" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... visibility
  _rqXmlStreamWriter.writeAttribute( "visibility", QString::number( COverlayVisibility::getVisibility() ) );
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
  // ... segments
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->dumpQVCT( _rqXmlStreamWriter, bOnlySelected );
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Track
}

void CTrackContainer::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  _rqXmlStreamWriter.writeStartElement( "trk" );
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
  // ... segments
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->dumpGPX( _rqXmlStreamWriter, bOnlySelected );
  _rqXmlStreamWriter.writeEndElement(); // trk
}

int CTrackContainer::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    __iCount += ((CTrackSubContainer*)QTreeWidgetItem::child( __i ))->deleteSelection();
  return __iCount;
}
