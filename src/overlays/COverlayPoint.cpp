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
#include <QPointF>

// QVCT
#include "data/CDataPositionValidity.hpp"
#include "data/CDataTimeValidity.hpp"
#include "charts/CChart.hpp"
#include "overlays/COverlay.hpp"
#include "overlays/COverlayPoint.hpp"
#include "units/CUnitBearing.hpp"
#include "units/CUnitDistance.hpp"
#include "units/CUnitElevation.hpp"
#include "units/CUnitPosition.hpp"
#include "units/CUnitTime.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayPoint::COverlayPoint( const QString& _rqsName )
  : COverlayItem( COverlayObject::ITEM, _rqsName )
  , COverlayVisibility()
  , CDataPosition()
{}

COverlayPoint::COverlayPoint( const QString& _rqsName, const CDataPosition& _roDataPosition )
  : COverlayItem( COverlayObject::ITEM, _rqsName )
  , COverlayVisibility()
  , CDataPosition( _roDataPosition )
{}


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

int COverlayPoint::getPositionBox( const QTreeWidgetItem* _pqTreeWidgetItem,
                                   CDataPosition* _poDataPositionLower, CDataPosition* _poDataPositionUpper,
                                   int _iIndexMin, int _iIndexMax )
{

  // Input bounding box
  // ... longitude
  double __fdLongitudeLower = _poDataPositionLower->getLongitude();
  if( __fdLongitudeLower == CDataPosition::UNDEFINED_LONGITUDE ) __fdLongitudeLower = -180;
  double __fdLongitudeUpper = _poDataPositionUpper->getLongitude();
  if( __fdLongitudeUpper == CDataPosition::UNDEFINED_LONGITUDE ) __fdLongitudeUpper = 180;
  if( __fdLongitudeLower > __fdLongitudeUpper )
  {
    double __fd = __fdLongitudeLower;
    __fdLongitudeUpper = __fdLongitudeLower;
    __fdLongitudeLower = __fd;
  }
  // ... latitude
  double __fdLatitudeLower = _poDataPositionLower->getLatitude();
  if( __fdLatitudeLower == CDataPosition::UNDEFINED_LATITUDE ) __fdLatitudeLower = -90;
  double __fdLatitudeUpper = _poDataPositionUpper->getLatitude();
  if( __fdLatitudeUpper == CDataPosition::UNDEFINED_LATITUDE ) __fdLatitudeUpper = 90;
  if( __fdLatitudeLower > __fdLatitudeUpper )
  {
    double __fd = __fdLatitudeLower;
    __fdLatitudeUpper = __fdLatitudeLower;
    __fdLatitudeLower = __fd;
  }
  // ... elevation
  double __fdElevationLower = _poDataPositionLower->getElevation();
  if( __fdElevationLower == CDataPosition::UNDEFINED_ELEVATION ) __fdElevationLower = -999999;
  double __fdElevationUpper = _poDataPositionUpper->getElevation();
  if( __fdElevationUpper == CDataPosition::UNDEFINED_ELEVATION ) __fdElevationUpper = 999999;
  if( __fdElevationLower > __fdElevationUpper )
  {
    double __fd = __fdElevationLower;
    __fdElevationUpper = __fdElevationLower;
    __fdElevationLower = __fd;
  }
  // ... indices
  int __iIndexMax = _iIndexMax >= 0 ? _iIndexMax : _pqTreeWidgetItem->childCount()-1;

  // Output bounding box
  int __iCountValid = 0;
  double __fdLongitudeMin = 180, __fdLongitudeMax = -180;
  double __fdLatitudeMin = 90, __fdLatitudeMax = -90;
  double __fdElevationMin = 9999999, __fdElevationMax = -9999999;
  for( int __i = _iIndexMin; __i <= __iIndexMax; __i++ )
  {
    CDataPosition* __poDataPosition = (CDataPosition*)(COverlayPoint*)_pqTreeWidgetItem->child( __i );
    if( *__poDataPosition == CDataPosition::UNDEFINED ) continue;
    double __fdLongitude = __poDataPosition->getLongitude();
    if( __fdLongitude < __fdLongitudeLower || __fdLongitude > __fdLongitudeUpper ) continue;
    double __fdLatitude = __poDataPosition->getLatitude();
    if( __fdLatitude < __fdLatitudeLower || __fdLatitude > __fdLatitudeUpper ) continue;
    double __fdElevation = __poDataPosition->getElevation();
    if( __fdElevation != CDataPosition::UNDEFINED_ELEVATION
        && ( __fdElevation < __fdElevationLower || __fdElevation > __fdElevationUpper ) ) continue;
    __iCountValid++;
    if( __fdLongitude < __fdLongitudeMin ) __fdLongitudeMin = __fdLongitude;
    if( __fdLongitude > __fdLongitudeMax ) __fdLongitudeMax = __fdLongitude;
    if( __fdLatitude < __fdLatitudeMin ) __fdLatitudeMin = __fdLatitude;
    if( __fdLatitude > __fdLatitudeMax ) __fdLatitudeMax = __fdLatitude;
    if( __fdElevation != CDataPosition::UNDEFINED_ELEVATION )
    {
      if( __fdElevation < __fdElevationMin ) __fdElevationMin = __fdElevation;
      if( __fdElevation > __fdElevationMax ) __fdElevationMax = __fdElevation;
    }
  }
  _poDataPositionLower->resetPosition();
  _poDataPositionUpper->resetPosition();
  if( __iCountValid )
  {
    _poDataPositionLower->setPosition( __fdLongitudeMin, __fdLatitudeMin, __fdElevationMin > -9999999 ? __fdElevationMin : CDataPosition::UNDEFINED_ELEVATION );
    _poDataPositionUpper->setPosition( __fdLongitudeMax, __fdLatitudeMax, __fdElevationMax < 9999999 ? __fdElevationMax : CDataPosition::UNDEFINED_ELEVATION );
  }
  return __iCountValid;
}


//------------------------------------------------------------------------------
// METHODS: COverlayObject (implement/override)
//------------------------------------------------------------------------------

void COverlayPoint::serialize( QDataStream& _rqDataStream ) const
{
  COverlayItem::serialize( _rqDataStream );
  CDataPosition::serialize( _rqDataStream );
}

void COverlayPoint::unserialize( QDataStream& _rqDataStream )
{
  COverlayItem::unserialize( _rqDataStream );
  CDataPosition::unserialize( _rqDataStream );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void COverlayPoint::drawText( const CChart* _poChart, QPainter* _pqPainter,
                              const QString& _rqsText, const QPointF& _rqPointFScrPosition,
                              bool _bItalic )
{
  // Constant drawing resources
  static const QSizeF __qSizeFRect(200,14), __qSizeRBackgroundGrow( 4, 0 );
  static const QPointF __qPointFBackOffset(2,0);

  // Retrieve drawing parameters
  double __fdZoom = _poChart->getZoom();
  if( __fdZoom < 0.5 ) return;
  QString __qsText = _rqsText;

  // Draw
  COverlay* __poOverlay = useOverlay();
  QFont __qFont = __poOverlay->getFont();
  __qFont.setPixelSize( __qFont.pixelSize() * ( __fdZoom >= 0.75 ? __fdZoom : 0.75 ) );
  __qFont.setBold( false );
  __qFont.setItalic( _bItalic );
  _pqPainter->setFont( __qFont );

  // Draw: first pass (find out bounding rect)
  QRectF __qRectFBackground;
  _pqPainter->setPen( QPen( Qt::transparent ) );
  _pqPainter->drawText( QRectF( _rqPointFScrPosition, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, __qsText, &__qRectFBackground );
  // Draw: second pass (actually draw)
  _pqPainter->setBrush( __poOverlay->getBrushText() );
  __qRectFBackground.setSize( __qRectFBackground.size() + __qSizeRBackgroundGrow );
  __qRectFBackground.moveCenter( _rqPointFScrPosition );
  // ... background
  _pqPainter->drawRect( __qRectFBackground );
  _pqPainter->setPen( __poOverlay->getPenText() );
  // ... text
  _pqPainter->drawText( QRectF( __qRectFBackground.topLeft()+__qPointFBackOffset, __qRectFBackground.size() ), Qt::AlignHCenter|Qt::AlignTop, __qsText );
}

void COverlayPoint::drawMarker( const CChart* _poChart, QPainter* _pqPainter, const CDataPositionValidity* _poDataPositionValidity )
{
  // Constant drawing resources
  static const QPointF __qPointFCrosshairA1(6,0), __qPointFCrosshairA2(12,0);
  static const QPointF __qPointFCrosshairB1(0,6), __qPointFCrosshairB2(0,12);
  static const QPointF __qPointFCrosshairC1(-6,0), __qPointFCrosshairC2(-12,0);
  static const QPointF __qPointFCrosshairD1(0,-6), __qPointFCrosshairD2(0,-12);
  static const QSize __qSizeSelect(32,32);
  static const QPixmap __qPixmapSelect( ":icons/32x32/select.png" );

  // Exit if we're not visible
  if( !bVisible ) return;

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
  _pqPainter->setBrush( __poOverlay->getBrushMarker() );
  _pqPainter->setPen( __qPen );
  _pqPainter->drawEllipse( __qPointF, 9*__fdZoom, 9*__fdZoom );
  _pqPainter->drawPoint( __qPointF );
  _pqPainter->drawLine( __qPointF + __qPointFCrosshairA1*__fdZoom, __qPointF + __qPointFCrosshairA2*__fdZoom );
  _pqPainter->drawLine( __qPointF + __qPointFCrosshairB1*__fdZoom, __qPointF + __qPointFCrosshairB2*__fdZoom );
  _pqPainter->drawLine( __qPointF + __qPointFCrosshairC1*__fdZoom, __qPointF + __qPointFCrosshairC2*__fdZoom );
  _pqPainter->drawLine( __qPointF + __qPointFCrosshairD1*__fdZoom, __qPointF + __qPointFCrosshairD2*__fdZoom );
  // ... multi-select
  if( !isMultiSelected() || __fdZoom < 0.5 ) return;
  _pqPainter->drawPixmap( __qPointF, __qPixmapSelect.scaled( __qSizeSelect*(__fdZoom/2.0), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
}

void COverlayPoint::drawTag( const CChart* _poChart, QPainter* _pqPainter, ETagPosition _eTagPosition,
                             const CDataTimeValidity* _poDataTimeValidity, const CDataPositionValidity* _poDataPositionValidity )
{
  // Exit if we're not visible
  if( !bVisible || !bVisibleName ) return;

  // Retrieve drawing parameters
  double __fdZoom = _poChart->getZoom();
  if( __fdZoom < 0.5 ) return;
  QPointF __qPointF = _poChart->toDrawPosition( *this );
  QString __qsName = getName();
  if( __qsName.length() > 20 ) { __qsName.truncate( 19 ); __qsName += QString::fromUtf8("…"); }

  // Draw
  COverlay* __poOverlay = useOverlay();
  QFont __qFont = __poOverlay->getFont();
  int __iFontSize = __qFont.pixelSize() * ( __fdZoom >= 0.75 ? __fdZoom : 0.75 );
  __qFont.setPixelSize( __iFontSize );

  // Draw: first pass (find out bounding rect)
  QPointF __qPointFaux = __qPointF;
  QSizeF __qSizeFRect( __iFontSize*20, __iFontSize+4 ), __qSizeRBackgroundGrow( __iFontSize/2, 0 );
  QPointF __qPointFBackOffset( __iFontSize/4, 0 ), __qPointFTextOffset( 0, __iFontSize );
  QRectF __qRectFBounding, __qRectFBackground;
  _pqPainter->setPen( QPen( Qt::transparent ) );
  // ... name
  __qFont.setBold( true ); __qFont.setItalic( false ); _pqPainter->setFont( __qFont );
  _pqPainter->drawText( QRectF( __qPointFaux, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, __qsName, &__qRectFBounding );
  __qRectFBackground = __qRectFBounding;
  if( bVisiblePosition && __fdZoom >= 0.75 )
  {
    __qFont.setBold( false );
    // ... time
    if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME )
    {
      __qFont.setItalic( !_poDataTimeValidity || _poDataTimeValidity->isValidTime() ? false : true );
      _pqPainter->setFont( __qFont );
      __qPointFaux += __qPointFTextOffset;
      _pqPainter->drawText( QRectF( __qPointFaux, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitTime::toString( CDataTime::getTime() ), &__qRectFBounding );
      __qRectFBackground = __qRectFBackground.united( __qRectFBounding );
    }
    // ... position
    __qFont.setItalic( !_poDataPositionValidity || _poDataPositionValidity->isValidPosition() ? false : true );
    _pqPainter->setFont( __qFont );
    __qPointFaux += __qPointFTextOffset;
    _pqPainter->drawText( QRectF( __qPointFaux, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitPosition::toString( CDataPosition::getLongitude(), CUnitPosition::LONGITUDE ), &__qRectFBounding );
    __qRectFBackground = __qRectFBackground.united( __qRectFBounding );
    __qPointFaux += __qPointFTextOffset;
    _pqPainter->drawText( QRectF( __qPointFaux, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitPosition::toString( CDataPosition::getLatitude(), CUnitPosition::LATITUDE ), &__qRectFBounding );
    __qRectFBackground = __qRectFBackground.united( __qRectFBounding );
    // ... elevation
    if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    {
      __qFont.setItalic( !_poDataPositionValidity || _poDataPositionValidity->isValidElevation() ? false : true );
      _pqPainter->setFont( __qFont );
      __qPointFaux += __qPointFTextOffset;
      _pqPainter->drawText( QRectF( __qPointFaux, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitElevation::toString( CDataPosition::getElevation() ), &__qRectFBounding );
      __qRectFBackground = __qRectFBackground.united( __qRectFBounding );
    }
  }

  // Position
  QPointF __qPointFPositionOffset = QPointF( 15*__fdZoom, -20*__fdZoom );
  __qRectFBackground.setSize( __qRectFBackground.size() + __qSizeRBackgroundGrow );
  if( _eTagPosition == TAG_AUTO )
    _eTagPosition = __qPointF.x() + __qPointFPositionOffset.x() + __qRectFBackground.width() < _poChart->getDrawArea().width()
                    ? TAG_RIGHT
                    : TAG_LEFT;
  if( _eTagPosition == TAG_RIGHT )
  {
    __qPointF += __qPointFPositionOffset;
    __qRectFBackground.moveTopLeft( __qPointF );
  }
  else if( _eTagPosition == TAG_LEFT )
  {
    __qPointFPositionOffset += QPointF( __qRectFBackground.width(), 0 );
    __qPointF += QPointF( -__qPointFPositionOffset.x(), __qPointFPositionOffset.y() );
    __qRectFBackground.moveTopLeft( __qPointF );
  }
  __qPointF += __qPointFBackOffset;

  // Draw: second pass (actually draw)
  _pqPainter->setBrush( __poOverlay->getBrushText() );
  // ... background
  _pqPainter->drawRect( __qRectFBackground );
  _pqPainter->setPen( __poOverlay->getPenText() );
  // ... name
  __qFont.setBold( true ); __qFont.setItalic( false ); _pqPainter->setFont( __qFont );
  _pqPainter->drawText( QRectF( __qPointF, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, __qsName );
  if( bVisiblePosition && __fdZoom >= 0.75 )
  {
    __qFont.setBold( false );
    // ... time
    if( CDataTime::getTime() != CDataTime::UNDEFINED_TIME )
    {
      __qFont.setItalic( !_poDataTimeValidity || _poDataTimeValidity->isValidTime() ? false : true );
      _pqPainter->setFont( __qFont );
      __qPointF += __qPointFTextOffset;
      _pqPainter->drawText( QRectF( __qPointF, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitTime::toString( CDataTime::getTime() ) );
    }
    // ... position
    __qFont.setItalic( !_poDataPositionValidity || _poDataPositionValidity->isValidPosition() ? false : true );
    _pqPainter->setFont( __qFont );
    __qPointF += __qPointFTextOffset;
    _pqPainter->drawText( QRectF( __qPointF, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitPosition::toString( CDataPosition::getLongitude(), CUnitPosition::LONGITUDE ) );
    __qPointF += __qPointFTextOffset;
    _pqPainter->drawText( QRectF( __qPointF, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitPosition::toString( CDataPosition::getLatitude(), CUnitPosition::LATITUDE ) );
    // ... elevation
    if( CDataPosition::getElevation() != CDataPosition::UNDEFINED_ELEVATION )
    {
      __qFont.setItalic( !_poDataPositionValidity || _poDataPositionValidity->isValidElevation() ? false : true );
      _pqPainter->setFont( __qFont );
      __qPointF += __qPointFTextOffset;
      _pqPainter->drawText( QRectF( __qPointF, __qSizeFRect ), Qt::AlignLeft|Qt::AlignTop, CUnitElevation::toString( CDataPosition::getElevation() ) );
    }
  }
}

void COverlayPoint::drawLine( const CChart* _poChart, QPainter* _pqPainter, const COverlayPoint* _poOverlayPoint )
{
  // Exit if we're not visible
  if( !bVisible || !_poOverlayPoint->bVisible ) return;

  // Retrieve drawing parameters
  double __fdZoom = _poChart->getZoom();
  QPointF __qPointFFrom = _poChart->toDrawPosition( *this );
  QPointF __qPointFTo = _poChart->toDrawPosition( *_poOverlayPoint );
  QPointF __qPointFDelta = __qPointFTo - __qPointFFrom;

  // Draw
  COverlay* __poOverlay = useOverlay();
  QPen __qPen = __poOverlay->getPenMarker();
  __qPen.setWidth( __qPen.width() * ( __fdZoom >= 1.0 ? __fdZoom : 1.0 ) );
  // ... line
  _pqPainter->setPen( __qPen );
  _pqPainter->drawLine( __qPointFFrom, __qPointFTo );
  // ... course data
  double __fdBearingTo = CDataPosition::bearingRL( *this, *_poOverlayPoint );
  double __fdDeltaLimit = 75.0 + 100.0*fabs( pow( sin( __fdBearingTo*0.01745329 ), 3 ) );
  if( ( !bVisibleRouting && !_poOverlayPoint->bVisibleRouting )
      || __qPointFDelta.x()*__qPointFDelta.x() + __qPointFDelta.y()*__qPointFDelta.y() < __fdDeltaLimit*__fdDeltaLimit
      || __fdZoom < 0.5 ) return;
  double __fdDistance = CDataPosition::distanceRL( *this, *_poOverlayPoint );
  QString __qsDistance = CUnitDistance::toString( __fdDistance );
  drawText( _poChart, _pqPainter, __qsDistance, 0.5 * __qPointFFrom + 0.5 * __qPointFTo );
  if( bVisibleRouting || _poOverlayPoint->bVisibleRouting )
  {
    double __fdBearingFrom = __fdBearingTo + 180.0;
    if( __fdBearingFrom > 360.0 ) __fdBearingFrom -= 360.0;
    QString __qsBearingTo = CUnitBearing::toString( __fdBearingTo );
    QString __qsBearingFrom = CUnitBearing::toString( __fdBearingFrom );
    __fdBearingFrom *= 0.01745329; // degree -> radian
    __fdBearingTo *= 0.01745329; // degree -> radian
    double __fdBearingOffset = ( 20.0 + 20.0*fabs( pow( sin( __fdBearingTo ), 3 ) ) ) * __fdZoom;
    if( bVisibleRouting ) drawText( _poChart, _pqPainter, __qsBearingTo+QString::fromUtf8("·")+__qsBearingFrom, __qPointFFrom+QPointF( __fdBearingOffset*sin( __fdBearingTo ), -__fdBearingOffset*cos( __fdBearingTo ) ) );
    if( _poOverlayPoint->bVisibleRouting ) drawText( _poChart, _pqPainter, __qsBearingFrom+QString::fromUtf8("·")+__qsBearingTo, __qPointFTo+QPointF( __fdBearingOffset*sin( __fdBearingFrom ), -__fdBearingOffset*cos( __fdBearingFrom ) ) );
  }
}

bool COverlayPoint::matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const
{
  // Retrieve and adjust drawing parameters
  double __fdZoom = _poChart->getZoom();
  if( __fdZoom < 0.2 ) __fdZoom = 0.2;
  QPointF __qPointF = _poChart->toDrawPosition( *this );

  // Match
  __qPointF -= _rqPointFScrPosition;
  return( __qPointF.x()*__qPointF.x() + __qPointF.y()*__qPointF.y() <= 100*__fdZoom*__fdZoom );
}
