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
#include <QDataStream>
#include <QPainter>

// QVCT
#include "data/CDataCourseValidity.hpp"
#include "data/CDataPositionValidity.hpp"
#include "charts/CChart.hpp"
#include "overlays/COverlay.hpp"
#include "overlays/COverlayCourse.hpp"
#include "units/CUnitBearing.hpp"
#include "units/CUnitSpeed.hpp"
#include "units/CUnitSpeedVertical.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayCourse::COverlayCourse( const QString& _rqsName )
  : COverlayPoint( _rqsName )
  , CDataCourseGA()
{}

COverlayCourse::COverlayCourse( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayPoint( _rqsName, _roDataPosition )
  , CDataCourseGA()
{}

COverlayCourse::COverlayCourse( const QString& _rqsName, const CDataPosition& _roDataPosition,
                                const CDataCourse& _roGroundGeoCourse, const CDataCourse& _roApparentGeoCourse )
  : COverlayPoint( _rqsName, _roDataPosition )
  , CDataCourseGA( _roGroundGeoCourse, _roApparentGeoCourse )
{}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void COverlayCourse::serialize( QDataStream& _rqDataStream ) const
{
  COverlayPoint::serialize( _rqDataStream );
  CDataCourseGA::serialize( _rqDataStream );
}

void COverlayCourse::unserialize( QDataStream& _rqDataStream )
{
  COverlayItem::unserialize( _rqDataStream );
  CDataCourseGA::unserialize( _rqDataStream );
}


//------------------------------------------------------------------------------
// METHODS: COverlayPoint (implement/override)
//------------------------------------------------------------------------------


void COverlayCourse::drawMarker( const CChart* _poChart, QPainter* _pqPainter, const CDataPositionValidity* _poDataPositionValidity )
{
  // Constant drawing resources
  static const QSize __qSizeSelect(32,32);
  static const QPixmap __qPixmapSelect( ":icons/32x32/select.png" );

  // Exit if we're not visible
  if( CDataPosition::operator==( CDataPosition::UNDEFINED ) || !bVisible ) return;

  // Retrieve and adjust drawing parameters
  double __fdZoom = _poChart->getZoom();
  if( __fdZoom < 0.2 ) __fdZoom = 0.2;
  QPointF __qPointF = _poChart->toDrawPosition( *this );

  // Draw
  COverlay* __poOverlay = useOverlay();
  QPen __qPen = __poOverlay->getPenMarker();
  __qPen.setWidth( __qPen.width() * __fdZoom );
  if( _poDataPositionValidity && !_poDataPositionValidity->isValidPosition() ) __qPen.setStyle( Qt::DotLine );
  // ... marker
  double __fdRadius = 11.0 * __fdZoom;
  _pqPainter->setBrush( __poOverlay->getBrushMarker() );
  _pqPainter->setPen( __qPen );
  _pqPainter->drawEllipse( __qPointF, __fdRadius, __fdRadius );
  _pqPainter->drawPoint( __qPointF );
  double __fdGroundBearing = CDataCourseGA::GroundCourse.getBearing();
  if( __fdGroundBearing == CDataCourse::UNDEFINED_BEARING ) __fdGroundBearing = 0;
  else __fdGroundBearing *= 0.01745329; // degree -> radian
  __fdRadius = 16.0 * __fdZoom;
  QPointF __qPointFTip = __qPointF + __fdRadius * QPointF( sin( __fdGroundBearing ), -cos( __fdGroundBearing ) );
  _pqPainter->drawLine( __qPointFTip, __qPointF + __fdRadius * QPointF( sin( __fdGroundBearing+2.5 ), -cos( __fdGroundBearing+2.5 ) ) );
  _pqPainter->drawLine( __qPointFTip, __qPointF + __fdRadius * QPointF( sin( __fdGroundBearing-2.5 ), -cos( __fdGroundBearing-2.5 ) ) );
  // ... multi-select
  if( !isMultiSelected() || __fdZoom < 0.5 ) return;
  _pqPainter->drawPixmap( __qPointF, __qPixmapSelect.scaled( __qSizeSelect*(__fdZoom/2.0), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
}

//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void COverlayCourse::drawVector( const CChart* _poChart, QPainter* _pqPainter, const CDataCourseValidityGA* _poDataCourseValidityGA )
{
  // Exit if we're not visible
  if( CDataPosition::operator==( CDataPosition::UNDEFINED ) || !bVisible || !bVisibleCourse ) return;

  // Retrieve drawing parameters
  double __fdZoom = _poChart->getZoom();
  if( __fdZoom < 0.75 ) __fdZoom = 0.75;
  QPointF __qPointF = _poChart->toDrawPosition( *this );

  // Draw
  COverlay* __poOverlay = useOverlay();
  QPen __qPen = __poOverlay->getPenVector();
  double __fdLength = _poChart->getDrawArea().width() + _poChart->getDrawArea().height();

  // ... ground course
  double __fdGroundBearing = CDataCourseGA::GroundCourse.getBearing();
  if( __fdGroundBearing != CDataCourse::UNDEFINED_BEARING )
  {
    // ... bearing line
    QString __qsGroundBearing = CUnitBearing::toString( __fdGroundBearing );
    __fdGroundBearing *= 0.01745329; // degree -> radian
    QPointF __qPointFBearing = QPointF( sin( __fdGroundBearing ), -cos( __fdGroundBearing ) );
    QPointF __qPointFGround = __qPointF + __fdLength * __qPointFBearing;
    bool __bInvalid = false;
    if( _poDataCourseValidityGA && !_poDataCourseValidityGA->GroundCourseValidity.isValidBearing() )
    {
      __bInvalid = true;
      __qPen.setStyle( Qt::DotLine );
    }
    __qPen.setWidth( 3.0 * __fdZoom );
    _pqPainter->setPen( __qPen );
    _pqPainter->drawLine( __qPointF, __qPointFGround );

    // ... speed tick
    double __fdGroundSpeed = CDataCourseGA::GroundCourse.getSpeed();
    QString __qsGroundSpeed = "-";
    if( __fdGroundSpeed != CDataCourse::UNDEFINED_SPEED )
    {
      __qsGroundSpeed = CUnitSpeed::toString( __fdGroundSpeed );
      QBrush __qBrush = __poOverlay->getBrushMarker();
      __qBrush.setStyle( Qt::SolidPattern );
      if( _poDataCourseValidityGA && !_poDataCourseValidityGA->GroundCourseValidity.isValidSpeed() )
      {
        __bInvalid = true;
        __qBrush.setStyle( Qt::Dense4Pattern );
      }
      _pqPainter->setBrush( __qBrush );
      _pqPainter->setPen( QPen( Qt::transparent ) );
      double __fdLengthMinute = __fdGroundSpeed * 60.0 * _poChart->getZoom() / _poChart->getResolution();
      if( __fdLengthMinute >= 10 )
      {
        for( int __i = 0; __i < 5; __i++ )
        {
          double __fdLengthTick = (__i+1)*__fdLengthMinute;
          if( __fdLengthTick > __fdLength ) break;
          double __fdRadius = ( 10.0 - __i ) * __fdZoom;
          _pqPainter->drawEllipse( __qPointF + __fdLengthTick * __qPointFBearing, __fdRadius, __fdRadius );
        }
      }
    }

    // ... course data
    double __fdGroundSpeedVertical = CDataCourseGA::GroundCourse.getSpeedVertical();
    QString __qsGroundSpeedVertical = "-";
    if( __fdGroundSpeedVertical != CDataCourse::UNDEFINED_SPEED )
    {
      __qsGroundSpeedVertical = CUnitSpeedVertical::toString( __fdGroundSpeedVertical );
      if( _poDataCourseValidityGA && !_poDataCourseValidityGA->GroundCourseValidity.isValidSpeedVertical() ) __bInvalid = true;
    }
    double __fdBearingOffset = ( 35.0 + 25.0*fabs( pow( sin( __fdGroundBearing ), 3 ) ) ) * __fdZoom;
    drawText( _poChart, _pqPainter, __qsGroundBearing+QString::fromUtf8("·")+__qsGroundSpeed+"\n"+__qsGroundSpeedVertical, __qPointF+QPointF( __fdBearingOffset*sin( __fdGroundBearing ), -__fdBearingOffset*cos( __fdGroundBearing ) ), __bInvalid );
  }

  // ... apparent course
  double __fdApparentBearing = CDataCourseGA::ApparentCourse.getBearing();
  if( __fdApparentBearing != CDataCourse::UNDEFINED_BEARING )
  {
    __fdApparentBearing *= 0.01745329; // degree -> radian
    QPointF __qPointFApparent = __qPointF + __fdLength * QPointF( sin( __fdApparentBearing ), -cos( __fdApparentBearing ) );
    if( _poDataCourseValidityGA && !_poDataCourseValidityGA->ApparentCourseValidity.isValidBearing() ) __qPen.setStyle( Qt::DotLine );
    __qPen.setWidth( 2.0 * __fdZoom );
    _pqPainter->setPen( __qPen );
    _pqPainter->drawLine( __qPointF, __qPointFApparent );
  }
}
