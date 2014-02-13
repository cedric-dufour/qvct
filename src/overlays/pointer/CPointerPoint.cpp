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

// QVCT
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "overlays/pointer/CPointerPoint.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CPointerPoint::CPointerPoint( const QString& _rqsName )
  : COverlayPoint( _rqsName )
{}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void CPointerPoint::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  if( !bVisible
      || this->CDataPosition::operator==( CDataPosition::UNDEFINED )
      || !_poChart->getDrawArea().contains( _poChart->toDrawPosition( *this ).toPoint() ) ) return;
  COverlayPoint::drawMarker( _poChart, _pqPainter );
  COverlayPoint::drawTag( _poChart, _pqPainter );
}

void CPointerPoint::showDetail()
{
  QVCTRuntime::usePointerPointDetailView()->setOverlayObject( this );
  QVCTRuntime::usePointerPointDetailView()->refreshContent();
  QVCTRuntime::useOverlayDetailView()->switchView( COverlayDetailView::POINTER_POINT );
  QVCTRuntime::useChartTable()->setOverlayObjectSelected( 0 );
}
