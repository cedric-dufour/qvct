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
#include "overlays/landmark/CLandmarkContainer.hpp"
#include "overlays/landmark/CLandmarkContainerEditView.hpp"
#include "overlays/landmark/CLandmarkPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CLandmarkContainer::CLandmarkContainer( const QString& _rqsName )
  : COverlayContainer( COverlayObject::CONTAINER, _rqsName )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled );
  QTreeWidgetItem::setText( CLandmarkOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::VISIBLE, Qt::Checked );
}

CLandmarkContainer::~CLandmarkContainer()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CLandmarkPoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CLandmarkContainer::unserialize( QDataStream& _rqDataStream )
{
  COverlayObject::unserialize( _rqDataStream );
  QTreeWidgetItem::setText( CLandmarkOverlay::NAME, qsName );
}

void CLandmarkContainer::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  if( !bVisible ) return;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
    ((CLandmarkPoint*)QTreeWidgetItem::child( __i ))->draw( _poChart, _pqPainter );
}

void CLandmarkContainer::showDetail()
{
  QVCTRuntime::useLandmarkContainerDetailView()->setOverlayObject( this );
  QVCTRuntime::useLandmarkContainerDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::LANDMARK_CONTAINER );
  QVCTRuntime::useOverlayListView()->switchView( COverlayListView::LANDMARK );
  QVCTRuntime::useChartTable()->setOverlayObjectSelected( this );
}

void CLandmarkContainer::showEdit()
{
  CLandmarkContainerEditView* __poLandmarkContainerEditView = new CLandmarkContainerEditView( this );
  if( __poLandmarkContainerEditView->exec() == QDialog::Accepted ) showDetail();
  delete __poLandmarkContainerEditView;
}


//------------------------------------------------------------------------------
// METHODS: COverlayContainer (implement/override)
//------------------------------------------------------------------------------

COverlayPoint* CLandmarkContainer::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  if( !bVisible ) return 0;
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = __iCount-1 ; __i >= 0; __i-- ) // we must go in the reverse order of CLandmarkContainer::draw() to pick the correct (overlapping) item
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

void CLandmarkContainer::toggleVisibility()
{
  // Toggle global visibility
  COverlayVisibility::toggleVisibility( false, false );
  // Set points visibility
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)QTreeWidgetItem::child( __i );
    __poLandmarkPoint->setVisibleName( bVisibleName );
    __poLandmarkPoint->setVisiblePosition( bVisiblePosition );
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CLandmarkPoint* CLandmarkContainer::addPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
{
  CLandmarkPoint* __poLandmarkPoint = new CLandmarkPoint( _rqsName, _roDataPosition );
  QTreeWidgetItem::addChild( __poLandmarkPoint );
  return __poLandmarkPoint;
}

int CLandmarkContainer::parseQVCT( const QDomElement& _rqDomElement )
{
  COverlayVisibility::setVisibility( _rqDomElement.attribute( "visibility", "3" ).toInt() );
  QTreeWidgetItem::setCheckState( CLandmarkOverlay::VISIBLE, bVisible ? Qt::Checked : Qt::Unchecked );
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "Point" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "Point" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.attribute( "name" );
    if( __qsName.isEmpty() ) __qsName = COverlayObject::newChildName( tr("Landmark"), 1, __iCount );
    CLandmarkPoint* __poLandmarkPoint = new CLandmarkPoint( __qsName );
    __poLandmarkPoint->parseQVCT( __qDomElement );
    addChild( __poLandmarkPoint );
  }
  return __iCount;
}

int CLandmarkContainer::parseGPX( const QDomElement& _rqDomElement )
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/
  int __iCount = 0;
  for( QDomElement __qDomElement = _rqDomElement.firstChildElement( "wpt" );
       !__qDomElement.isNull();
       __qDomElement = __qDomElement.nextSiblingElement( "wpt" ) )
  {
    __iCount++;
    QString __qsName = __qDomElement.firstChildElement( "name" ).text();
    if( __qsName.isEmpty() ) __qsName = COverlayObject::newChildName( tr("Landmark"), 1, __iCount );
    CLandmarkPoint* __poLandmarkPoint = new CLandmarkPoint( __qsName );
    __poLandmarkPoint->parseGPX( __qDomElement );
    addChild( __poLandmarkPoint );
  }
  return __iCount;
}

void CLandmarkContainer::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // Data
  _rqXmlStreamWriter.writeStartElement( "Landmarks" );
  // ... name
  if( !qsName.isEmpty() ) _rqXmlStreamWriter.writeAttribute( "name", qsName );
  // ... visibility
  _rqXmlStreamWriter.writeAttribute( "visibility", QString::number( COverlayVisibility::getVisibility() ) );
  // ... points
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poLandmarkPoint->isMultiSelected() ) continue;
    __poLandmarkPoint->dumpQVCT( _rqXmlStreamWriter );
  }
  // ... [end]
  _rqXmlStreamWriter.writeEndElement(); // Landmarks
}

void CLandmarkContainer::dumpGPX( QXmlStreamWriter & _rqXmlStreamWriter, bool bOnlySelected ) const
{
  // GPX format reference: see http://www.topografix.com/GPX/1/1/

  // Data
  // ... points
  int __iCount = QTreeWidgetItem::childCount();
  for( int __i = 0; __i < __iCount; __i++ )
  {
    CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)QTreeWidgetItem::child( __i );
    if( bOnlySelected && !__poLandmarkPoint->isMultiSelected() ) continue;
    __poLandmarkPoint->dumpGPX( _rqXmlStreamWriter );
  }
}

int CLandmarkContainer::deleteSelection()
{
  int __iCount = 0;
  for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
  {
    CLandmarkPoint* __poLandmarkPoint = (CLandmarkPoint*)QTreeWidgetItem::child( __i );
    if( __poLandmarkPoint->isMultiSelected() )
    {
      __iCount++;
      QTreeWidgetItem::removeChild( __poLandmarkPoint );
      delete __poLandmarkPoint;
    }
  }
  return __iCount;
}
