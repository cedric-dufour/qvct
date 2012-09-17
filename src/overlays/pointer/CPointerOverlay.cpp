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
#include <QPainter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "overlays/pointer/CPointerOverlay.hpp"
#include "overlays/pointer/CPointerPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CPointerOverlay::CPointerOverlay()
  : COverlayBase( tr("Pointer") )
{
  importSettings();
  QTreeWidgetItem::addChild( new CPointerPoint( tr("Mouse") ) );
  QTreeWidgetItem::addChild( new CPointerPoint( tr("Target") ) );
}

CPointerOverlay::~CPointerOverlay()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  while( __pqTreeWidgetItem )
  {
    delete (CPointerPoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 0 );
  }
}


//------------------------------------------------------------------------------
// METHODS: COverlay (implement/override)
//------------------------------------------------------------------------------

void CPointerOverlay::drawContent( const CChart* _poChart, QPainter* _pqPainter ) const
{
  if( !bVisible ) return;

  // Draw

  // ... mouse pointer
  CPointerPoint* __poPointerPoint = (CPointerPoint*)QTreeWidgetItem::child( 0 );
  __poPointerPoint->draw( _poChart, _pqPainter );

  // ... target
  __poPointerPoint = (CPointerPoint*)QTreeWidgetItem::child( 1 );
  __poPointerPoint->draw( _poChart, _pqPainter );
  CVesselPoint* __poVesselPoint = QVCTRuntime::useChartTable()->getVesselPointSynchronize();
  if( __poVesselPoint ) __poPointerPoint->drawLine( _poChart, _pqPainter, __poVesselPoint );

  // ... path
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 4 ) return;
  CPointerPoint* __poPointerPathFrom = (CPointerPoint*)QTreeWidgetItem::child( 2 );
  CPointerPoint* __poPointerPathTo = 0;
  for( int __i = 3; __i < __iCount; __i++ )
  {
    __poPointerPathTo = (CPointerPoint*)QTreeWidgetItem::child( __i );
    __poPointerPathFrom->drawLine( _poChart, _pqPainter, __poPointerPathTo );
    __poPointerPathFrom = __poPointerPathTo;
  }
}

void CPointerOverlay::showDetail( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( !_pqTreeWidgetItem ) return;
  switch( _pqTreeWidgetItem->type() )
  {

  case COverlayObject::ITEM:
    {
      CPointerPoint* __poPointerPoint = (CPointerPoint*)_pqTreeWidgetItem;
      __poPointerPoint->showDetail();
    }
    break;

  default:;

  }
}

void CPointerOverlay::setPosition( const QTreeWidgetItem* _pqTreeWidgetItem ) const
{
  if( _pqTreeWidgetItem->type() != COverlayObject::ITEM ) return;
  CPointerPoint* __poPointerPoint = (CPointerPoint*)_pqTreeWidgetItem;
  QVCTRuntime::useChartTable()->setGeoPosition( *__poPointerPoint );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// GETTERS
//

bool CPointerOverlay::hasPath() const
{
  return QTreeWidgetItem::childCount() > 2;
}

int CPointerOverlay::getPathSegments() const
{
  int __iCount = QTreeWidgetItem::childCount() - 3;
  if( __iCount < 0 ) __iCount = 0;
  return __iCount;
}

//
// USERS
//

CPointerPoint* CPointerOverlay::usePointerPoint( bool _bTarget )
{
  return (CPointerPoint*)QTreeWidgetItem::child( _bTarget ? 1 : 0 );
}

//
// OTHER
//

void CPointerOverlay::importSettings()
{
  QColor __qColor = QVCTRuntime::useSettings()->getColorPointer();
  __qColor.setAlpha( 128 );
  qBrushMarker.setColor( __qColor );
  __qColor.setAlpha( 192 );
  qPenText.setColor( __qColor );
  qPenMarker.setColor( __qColor );
  qPenLine.setColor( __qColor );
}

void CPointerOverlay::setPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition, bool _bTarget )
{
  CDataPosition __oDataPosition = _poChart->toGeoPosition( _rqPointFScrPosition );
  __oDataPosition.resetElevation();
  setPosition( __oDataPosition, _bTarget );
}

void CPointerOverlay::setPosition( const CDataPosition& _roDataPosition, bool _bTarget )
{
  CPointerPoint* __poPointerPoint = (CPointerPoint*)QTreeWidgetItem::child( _bTarget ? 1 : 0 );
  __poPointerPoint->setPosition( _roDataPosition );
}

void CPointerOverlay::clearPosition( bool _bTarget )
{
  CPointerPoint* __poPointerPoint = (CPointerPoint*)QTreeWidgetItem::child( _bTarget ? 1 : 0 );
  __poPointerPoint->setPosition( CDataPosition::UNDEFINED );
}

void CPointerOverlay::setPath( const CChart* _poChart, const QPointF& _rqPointFScrPosition )
{
  CDataPosition __oDataPosition = _poChart->toGeoPosition( _rqPointFScrPosition );
  __oDataPosition.resetElevation();
  setPath( __oDataPosition );
}

void CPointerOverlay::setPath( const CDataPosition& _roDataPosition )
{
  if( _roDataPosition == CDataPosition::UNDEFINED ) return;
  ((CPointerPoint*)QTreeWidgetItem::child( 0 ))->setPosition( _roDataPosition );
  CPointerPoint* __poPointerPoint = new CPointerPoint( "Path"+QString::number( QTreeWidgetItem::childCount() ) );
  __poPointerPoint->setPosition( _roDataPosition );
  QTreeWidgetItem::addChild( __poPointerPoint );
}

double CPointerOverlay::getPathLengthRL()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 4 ) return 0.0;
  CPointerPoint* __poPointerPathFrom = (CPointerPoint*)QTreeWidgetItem::child( 2 );
  CPointerPoint* __poPointerPathTo = 0;
  double __fdLength = 0;
  for( int __i = 3; __i < __iCount; __i++ )
  {
    __poPointerPathTo = (CPointerPoint*)QTreeWidgetItem::child( __i );
    __fdLength += CDataPosition::distanceRL( *__poPointerPathFrom, *__poPointerPathTo );
    __poPointerPathFrom = __poPointerPathTo;
  }
  return __fdLength;
}

double CPointerOverlay::getPathLengthGC()
{
  int __iCount = QTreeWidgetItem::childCount();
  if( __iCount < 4 ) return 0.0;
  CPointerPoint* __poPointerPathFrom = (CPointerPoint*)QTreeWidgetItem::child( 2 );
  CPointerPoint* __poPointerPathTo = (CPointerPoint*)QTreeWidgetItem::child( __iCount-1 );
  return CDataPosition::distanceGC( *__poPointerPathFrom, *__poPointerPathTo );
}

void CPointerOverlay::clearPath()
{
  QTreeWidgetItem* __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 2 );
  while( __pqTreeWidgetItem )
  {
    delete (CPointerPoint*)__pqTreeWidgetItem;
    __pqTreeWidgetItem = QTreeWidgetItem::takeChild( 2 );
  }
}
