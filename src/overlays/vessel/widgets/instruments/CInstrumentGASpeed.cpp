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
#include "overlays/vessel/widgets/instruments/CInstrumentGASpeed.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrumentGASpeed::CInstrumentGASpeed( CVesselCockpit* _poVesselCockpit )
  : CInstrument( _poVesselCockpit )
  , bForceRedraw( true )
  , eUnit( CUnitSpeed::M_S )
  , fdSpeed( CDataCourse::UNDEFINED_SPEED )
{
}


//------------------------------------------------------------------------------
// METHODS: CInstrument (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGASpeed::draw()
{
  // Retrieve parameters and values
  CVesselPoint* __poVesselPoint = CInstrument::useVesselPoint();
  CUnitSpeed::EUnit __eUnit = QVCTRuntime::useSettings()->getUnitSpeed();
  double __fdSpeed = __poVesselPoint ? __poVesselPoint->GroundCourse.getSpeed() : 0.0;

  // Redraw pixmap only if required
  if( bForceRedraw
      || qPixmapBuffer.size() != QWidget::size()
      || eUnit != __eUnit
      || fdSpeed != __fdSpeed )
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
    fdSpeed = __fdSpeed;
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

void CInstrumentGASpeed::drawStatic( CUnitSpeed::EUnit _eUnit )
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
  case CUnitSpeed::M_S: __iTicksLarge = 7; __iTicksSmall = 35; __iDigitsMax = 150; break;
  case CUnitSpeed::KM_H: __iTicksLarge = 6; __iTicksSmall = 30; __iDigitsMax = 600; break;
  case CUnitSpeed::MI_H: __iTicksLarge = 7; __iTicksSmall = 35; __iDigitsMax = 350; break;
  case CUnitSpeed::KT: __iTicksLarge = 6; __iTicksSmall = 30; __iDigitsMax = 300; break;
  default: return;
  }

  // ... delimiting corona
  __qPen.setColor( Qt::white );
  __qPen.setWidth( 10.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( QColor( 32, 32, 32, 255 ) ) );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 495.0, 495.0 );

  // ... small ticks
  for( int __i=0; __i<=__iTicksSmall; __i++ )
  {
    double __fdAngle = (double)__i * QVCT::PI * 34.0 / 18.0 / __iTicksSmall;
    QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
    __qPainter.drawLine( 450.0*__qPointFAngle, 480.0*__qPointFAngle );
  }

  // ... large ticks and digits
  __qFont.setPixelSize( 90.0 );
  __qFont.setBold( true );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -135.0, -45.0 ), QSizeF( 270.0, 90.0 ) );
    for( int __i=0; __i<=__iTicksLarge; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI * 34.0 / 18.0 / __iTicksLarge;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 420.0*__qPointFAngle, 480.0*__qPointFAngle );
      if( !__i ) continue;
      QPointF __qPointFAngleText( __qPointFAngle.x(), __qPointFAngle.y()*1.15 );
      __qPainter.drawText( __qRectFText.translated( 300.0*__qPointFAngleText ), Qt::AlignCenter, QString::number( __i*__iDigitsMax/__iTicksLarge ) );
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
    __qPainter.drawText( __qRectFText.translated( QPointF( 0, -150.0 ) ), Qt::AlignCenter, CUnitSpeed::toSymbol( _eUnit ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( 0, +150.0 ) ), Qt::AlignCenter, "GS" );
  }

  // ... data
  {
    QRectF __qRectFFrame( QPointF( -150.0, -60.0 ), QSizeF( 300.0, 120.0 ) );
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, +515.0 ) ), 10.0, 10.0 );
    __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, +540.0 ) ), Qt::AlignRight, CUnitSpeed::toSymbol( _eUnit ) );
  }
}

void CInstrumentGASpeed::drawDynamic( CUnitSpeed::EUnit _eUnit, const CVesselPoint* _poVesselPoint )
{
  double __fdSpeed = _poVesselPoint ? _poVesselPoint->GroundCourse.getSpeed() : 0.0;
  double __fdSpeedConverted = __fdSpeed != CDataCourse::UNDEFINED_SPEED ? CUnitSpeed::toValue( __fdSpeed, _eUnit ) : 0.0;

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
  case CUnitSpeed::M_S: __fdValueMax = 150.0; break;
  case CUnitSpeed::KM_H: __fdValueMax = 600.0; break;
  case CUnitSpeed::MI_H: __fdValueMax = 350.0; break;
  case CUnitSpeed::KT: __fdValueMax = 300.0; break;
  default: return;
  }

  // ... speed
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( Qt::white ) );
  double __fdSpeedLimited = __fdSpeedConverted;
  if( __fdSpeedLimited > __fdValueMax ) __fdSpeedLimited = __fdValueMax;
  __qPainter.rotate( __fdSpeedLimited/__fdValueMax * 340.0 );
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
  __qFont.setItalic( !_poVesselPoint || __fdSpeed == CDataCourse::UNDEFINED_SPEED || !_poVesselPoint->GroundCourseValidity.isValidSpeed() );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    QString __qsGS( "n/a" );
    if( _poVesselPoint && __fdSpeed != CDataCourse::UNDEFINED_SPEED )
    {
      int __iPrecision;
      switch( _eUnit )
      {
      case CUnitSpeed::M_S: __iPrecision = 1; break;
      default: __iPrecision = 0;
      }
      __qsGS = QString::number( __fdSpeedConverted, 'f', __iPrecision );
    }
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, +490.0 ) ), Qt::AlignCenter, __qsGS );
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
