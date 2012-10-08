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
#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QPointF>
#include <QPen>
#include <QPixmap>
#include <QSizeF>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGASpeedVertical.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrumentGASpeedVertical::CInstrumentGASpeedVertical( CVesselCockpit* _poVesselCockpit )
  : CInstrument( _poVesselCockpit )
  , bForceRedraw( true )
  , eUnit( CUnitSpeedVertical::M_S )
  , fdSpeedVertical( CDataCourse::UNDEFINED_SPEED )
{
}


//------------------------------------------------------------------------------
// METHODS: CInstrument (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGASpeedVertical::draw()
{
  // Retrieve parameters and values
  CVesselPoint* __poVesselPoint = CInstrument::useVesselPoint();
  CUnitSpeedVertical::EUnit __eUnit = QVCTRuntime::useSettings()->getUnitSpeedVertical();
  double __fdSpeedVertical = __poVesselPoint ? __poVesselPoint->GroundCourse.getSpeedVertical() : 0.0;

  // Redraw pixmap only if required
  if( bForceRedraw
      || qPixmapBuffer.size() != QWidget::size()
      || eUnit != __eUnit
      || fdSpeedVertical != __fdSpeedVertical )
  {
    // Draw resources
    if( bForceRedraw
        || qPixmapBufferBackground.size() != QWidget::size()
        || eUnit != __eUnit )
    {
      drawStatic( __eUnit );
    }
    drawDynamic( __eUnit, __poVesselPoint );

    // Update cache parameters and values
    bForceRedraw = false;
    eUnit = __eUnit;
    fdSpeedVertical = __fdSpeedVertical;
  }

  // Draw pixmap
  QPainter __qPainter;
  __qPainter.begin( this );
  __qPainter.setRenderHints( QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing );
  __qPainter.drawPixmap( 0, 0, qPixmapBuffer );
  __qPainter.end();
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void CInstrumentGASpeedVertical::drawStatic( CUnitSpeedVertical::EUnit _eUnit )
{
  // Widget geometry
  QPointF __qPointFCenter = QPointF( QRectF( QWidget::rect() ).center() );
  double __fdSizeFactor = ( QWidget::width() < QWidget::height() ? (double)QWidget::width() : (double)QWidget::height() ) / 1200.0;
  QTransform __qTransform;
  __qTransform.translate( __qPointFCenter.x(), __qPointFCenter.y() );
  __qTransform.scale( __fdSizeFactor, __fdSizeFactor );

  // Painter and resources
  if( qPixmapBufferBackground.size() != QWidget::size() ) qPixmapBufferBackground = QPixmap( QWidget::size() );
  qPixmapBufferBackground.fill( Qt::transparent );
  QPainter __qPainter( &qPixmapBufferBackground );
  QPen __qPen;
  QFont __qFont;
  __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

  // Draw
  __qPainter.setWorldTransform( __qTransform );
  int __iTicksLarge = 0, __iTicksSmall = 0, __iDigitsMax = 0;
  switch( _eUnit )
  {
  case CUnitSpeedVertical::M_S: __iTicksLarge = 5; __iTicksSmall = 10; __iDigitsMax = 10; break;
  case CUnitSpeedVertical::FT_MIN: __iTicksLarge = 4; __iTicksSmall = 20; __iDigitsMax = 20; break;
  default: return;
  }

  // ... delimiting corona
  __qPen.setColor( Qt::white );
  __qPen.setWidth( 10.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( QColor( 32, 32, 32, 255 ) ) );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 495.0, 495.0 );

  // ... small ticks
  for( int __i=-__iTicksSmall; __i<=__iTicksSmall; __i++ )
  {
    double __fdAngle = (double)__i * QVCT::PI * 17.0 / 18.0 / __iTicksSmall;
    QPointF __qPointFAngle( -cos( __fdAngle ), sin( __fdAngle ) );
    __qPainter.drawLine( 450.0*__qPointFAngle, 480.0*__qPointFAngle );
  }

  // ... large ticks and digits
  __qFont.setPixelSize( 90.0 );
  __qFont.setBold( true );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -135.0, -45.0 ), QSizeF( 180.0, 90.0 ) );
    for( int __i=-__iTicksLarge; __i<=__iTicksLarge; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI * 17.0 / 18.0 / __iTicksLarge;
      QPointF __qPointFAngle( -cos( __fdAngle ), -sin( __fdAngle ) );
      __qPainter.drawLine( 420.0*__qPointFAngle, 480.0*__qPointFAngle );
      __qPainter.drawText( __qRectFText.translated( 350.0*__qPointFAngle ), Qt::AlignVCenter|Qt::AlignRight, QString::number( __i*__iDigitsMax/__iTicksLarge ) );
    }
  }

  // ... name and units
  __qPen.setColor( Qt::lightGray );
  __qPen.setWidth( 5.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( QColor( 32, 32, 32, 255 ) ) );
  __qFont.setPixelSize( 50.0 );
  __qFont.setBold( false );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -200.0, -30.0 ), QSizeF( 400.0, 60.0 ) );
    switch( _eUnit )
    {
    case CUnitSpeedVertical::M_S: __qPainter.drawText( __qRectFText.translated( QPointF( 0, -150.0 ) ), Qt::AlignCenter, CUnitSpeedVertical::toSymbol( _eUnit ) ); break;
    case CUnitSpeedVertical::FT_MIN: __qPainter.drawText( __qRectFText.translated( QPointF( 0, -150.0 ) ), Qt::AlignCenter, "100"+CUnitSpeedVertical::toSymbol( _eUnit ) ); break;
    default:;
    }
    __qPainter.drawText( __qRectFText.translated( QPointF( 0, +150.0 ) ), Qt::AlignCenter, "VSI" );
  }

  // ... data
  {
    QRectF __qRectFFrame( QPointF( -150.0, -60.0 ), QSizeF( 300.0, 120.0 ) );
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, +515.0 ) ), 10.0, 10.0 );
    __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, +540.0 ) ), Qt::AlignRight, CUnitSpeedVertical::toSymbol( _eUnit ) );
  }
}

void CInstrumentGASpeedVertical::drawDynamic( CUnitSpeedVertical::EUnit _eUnit, const CVesselPoint* _poVesselPoint )
{
  double __fdSpeedVertical = _poVesselPoint ? _poVesselPoint->GroundCourse.getSpeedVertical() : 0.0;
  double __fdSpeedVerticalConverted = __fdSpeedVertical != CDataCourse::UNDEFINED_SPEED ? CUnitSpeedVertical::toValue( __fdSpeedVertical, _eUnit ) : 0.0;

  // Widget geometry
  QPointF __qPointFCenter = QPointF( QRectF( QWidget::rect() ).center() );
  double __fdSizeFactor = ( QWidget::width() < QWidget::height() ? (double)QWidget::width() : (double)QWidget::height() ) / 1200.0;
  QTransform __qTransform;
  __qTransform.translate( __qPointFCenter.x(), __qPointFCenter.y() );
  __qTransform.scale( __fdSizeFactor, __fdSizeFactor );

  // Painter and resources
  qPixmapBuffer = QPixmap( qPixmapBufferBackground );
  QPainter __qPainter( &qPixmapBuffer );
  QPen __qPen;
  QFont __qFont;
  __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

  // Draw
  __qPainter.setWorldTransform( __qTransform );
  double __fdValueMax = 0;
  switch( _eUnit )
  {
  case CUnitSpeedVertical::M_S: __fdValueMax = 10.0; break;
  case CUnitSpeedVertical::FT_MIN: __fdValueMax = 2000.0; break;
  default: return;
  }

  // ... speed
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( Qt::white ) );
  __qPainter.rotate( -90.0 );
  double __fdSpeedVerticalLimited = __fdSpeedVerticalConverted;
  if( __fdSpeedVerticalLimited > __fdValueMax ) __fdSpeedVerticalLimited = __fdValueMax;
  else if( __fdSpeedVerticalLimited < -__fdValueMax ) __fdSpeedVerticalLimited = -__fdValueMax;
  __qPainter.rotate( __fdSpeedVerticalLimited/__fdValueMax * 170.0 );
  {
    static const QPointF __pqPointFPolygon[5] =
      {
        QPointF( -15.0, -10.0 ),
        QPointF( -15.0, -370.0 ),
        QPointF( 0.0, -400.0 ),
        QPointF( +15.0, -370.0 ),
        QPointF( +15.0, -10.0 ),
      };
    __qPainter.drawConvexPolygon( __pqPointFPolygon, 5 );
  }

  // ... center mask
  __qPainter.setWorldTransform( __qTransform );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 40.0, 40.0 );
  __qPainter.setBrush( QBrush( Qt::black ) );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 30.0, 30.0 );

  // ... data
  __qPen.setColor( Qt::white );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
  __qFont.setPixelSize( 50.0 );
  __qFont.setBold( false );
  __qFont.setItalic( !_poVesselPoint || __fdSpeedVertical == CDataCourse::UNDEFINED_SPEED || !_poVesselPoint->GroundCourseValidity.isValidSpeedVertical() );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    QString __qsVS( "n/a" );
    if( _poVesselPoint && __fdSpeedVertical != CDataCourse::UNDEFINED_SPEED )
    {
      int __iPrecision;
      switch( _eUnit )
      {
      case CUnitSpeedVertical::M_S: __iPrecision = 1; break;
      default: __iPrecision = 0;
      }
      __qsVS = QString::number( __fdSpeedVerticalConverted, 'f', __iPrecision );
    }
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, +490.0 ) ), Qt::AlignCenter, __qsVS );
  }

  // ... OFF flag
  if( !_poVesselPoint )
  {
    __qPen.setColor( Qt::black );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QBrush( Qt::red ) );
    __qFont.setBold( true );
    __qFont.setItalic( false );
    __qPainter.setFont( __qFont );
    QRectF __qRectFText( QPointF( -75.0, -30.0 ), QSizeF( 150.0, 60.0 ) );
    __qPainter.drawRect( __qRectFText.translated( QPointF( +150.0, -50.0 ) ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( +150.0, -50.0 ) ), Qt::AlignCenter, "OFF" );
  }
}
