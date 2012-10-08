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
#include "overlays/vessel/widgets/instruments/CInstrumentGAChronograph.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrumentGAChronograph::CInstrumentGAChronograph( CVesselCockpit* _poVesselCockpit )
  : CInstrument( _poVesselCockpit )
  , bForceRedraw( true )
  , eUnit( CUnitTimeZone::UTC )
  , fdTime( 0.0 )
  , fdSW1TimeStarted( 0.0 )
  , fdSW1TimeAccumulated( 0.0 )
  , fdSW2TimeStarted( 0.0 )
  , fdSW2TimeAccumulated( 0.0 )
{
}


//------------------------------------------------------------------------------
// METHODS: QWidget (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGAChronograph::mouseReleaseEvent( QMouseEvent* _pqMouseEvent )
{
  QPointF __qPointFMouse = _pqMouseEvent->posF();
  if( __qPointFMouse.x() < QWidget::size().width()/2 )
  { // left-hand side
    if( fdSW1TimeStarted )
    {
      fdSW1TimeAccumulated += microtime() - fdSW1TimeStarted;
      fdSW1TimeStarted = 0;
    }
    else
      fdSW1TimeStarted = microtime();
  }
  else
  { // right-hand side
    if( fdSW2TimeStarted )
    {
      fdSW2TimeAccumulated += microtime() - fdSW2TimeStarted;
      fdSW2TimeStarted = 0;
    }
    else
      fdSW2TimeStarted = microtime();
  }
  bForceRedraw = true;
  QWidget::update();
}

void CInstrumentGAChronograph::mouseDoubleClickEvent( QMouseEvent* _pqMouseEvent )
{
  // NOTE: don't forget that the widget also receives the two corresponding mouseReleaseEvents
  QPointF __qPointFMouse = _pqMouseEvent->posF();
  if( __qPointFMouse.x() < QWidget::size().width()/2 )
  { // left-hand side
    if( fdSW1TimeStarted ) fdSW1TimeAccumulated = 0;
  }
  else
  { // right-hand side
    if( fdSW2TimeStarted ) fdSW2TimeAccumulated = 0;
  }
  bForceRedraw = true;
  QWidget::update();
}


//------------------------------------------------------------------------------
// METHODS: CInstrument (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGAChronograph::draw()
{
  // Retrieve parameters and values
  CUnitTimeZone::EUnit __eUnit = QVCTRuntime::useSettings()->getUnitTimeZone();
  double __fdTime = floor( microtime() + 0.5 ); // we don't use milli-seconds

  // Redraw pixmap only if required
  if( bForceRedraw
      || qPixmapBuffer.size() != QWidget::size()
      || eUnit != __eUnit
      || fdTime != __fdTime )
  {
    // Draw resources
    if( bForceRedraw
        || qPixmapBufferBackground.size() != QWidget::size()
        || eUnit != __eUnit )
    {
      drawStatic( __eUnit );
    }
    drawDynamic( __eUnit, __fdTime );

    // Update cache parameters and values
    bForceRedraw = false;
    eUnit = __eUnit;
    fdTime = __fdTime;
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

void CInstrumentGAChronograph::drawStatic( CUnitTimeZone::EUnit _eUnit )
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

  // ... delimiting corona
  __qPen.setColor( Qt::white );
  __qPen.setWidth( 10.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( QColor( 32, 32, 32, 255 ) ) );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 495.0, 495.0 );

  // ... small (1-minute) ticks
  for( int __i=0; __i<60; __i++ )
  {
    double __fdAngle = (double)__i * QVCT::PI / 30.0;
    QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
    __qPainter.drawLine( 450.0*__qPointFAngle, 480.0*__qPointFAngle );
  }

  // ... large (10-minutes) ticks
  {
    for( int __i=1; __i<=12; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 6.0;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 420.0*__qPointFAngle, 480.0*__qPointFAngle );
    }
  }

  // ... digits (hours)
  __qFont.setPixelSize( 90.0 );
  __qFont.setBold( true );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -90.0, -45.0 ), QSizeF( 180.0, 90.0 ) );
    for( int __i=1; __i<=12; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 6.0;
      QPointF __qPointFAngle( sin( __fdAngle )*( __i<10 ? 1.05 : 1.0 ), -cos( __fdAngle )*1.05 );
      __qPainter.drawText( __qRectFText.translated( 340.0*__qPointFAngle ), Qt::AlignCenter, QString::number( __i ) );
    }
  }

  // ... digits (minutes)
  __qFont.setPixelSize( 50.0 );
  __qFont.setBold( false );
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -50.0, -25.0 ), QSizeF( 100.0, 50.0 ) );
    for( int __i=1; __i<=12; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 6.0;
      QPointF __qPointFAngle( sin( __fdAngle )*( __i<2 ? 1.05 : 1.0 ), -cos( __fdAngle )*1.05 );
      __qPainter.drawText( __qRectFText.translated( 240.0*__qPointFAngle ), Qt::AlignCenter, QString::number( __i*5 ) );
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
    __qPainter.drawText( __qRectFText.translated( QPointF( 0, -150.0 ) ), Qt::AlignCenter, CUnitTimeZone::toSymbol( _eUnit ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( 0, +150.0 ) ), Qt::AlignCenter, "TIME" );
  }

  // ... data
  {
    QRectF __qRectFFrame( QPointF( -150.0, -60.0 ), QSizeF( 300.0, 120.0 ) );
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( -425.0, -515.0 ) ), 10.0, 10.0 );
    __qPainter.drawText( __qRectFText.translated( QPointF( -440.0, -540.0 ) ), Qt::AlignLeft, "SW1" );
    __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, -515.0 ) ), 10.0, 10.0 );
    __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, -540.0 ) ), Qt::AlignRight, "SW2" );
    __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, +515.0 ) ), 10.0, 10.0 );
    __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, +540.0 ) ), Qt::AlignRight, CUnitTimeZone::toSymbol( _eUnit ) );
  }
}

void CInstrumentGAChronograph::drawDynamic( CUnitTimeZone::EUnit _eUnit, double _fdTime )
{
  double __fdTimeConverted = CUnitTime::toValue( _fdTime, _eUnit );

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

  // ... hours
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QBrush( Qt::white ) );
  __qPainter.rotate( __fdTimeConverted/120.0 );
  {
    static const QPointF __pqPointFPolygon[5] =
      {
        QPointF( -25.0, -10.0 ),
        QPointF( -25.0, -250.0 ),
        QPointF( 0.0, -300.0 ),
        QPointF( +25.0, -250.0 ),
        QPointF( +25.0, -10.0 ),
      };
    __qPainter.drawConvexPolygon( __pqPointFPolygon, 5 );
  }

  // ... minutes
  __qPainter.setWorldTransform( __qTransform );
  __qPainter.rotate( __fdTimeConverted/10.0 );
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

  // ... seconds
  __qPen.setColor( QColor( 255, 128, 0 ) );
  __qPen.setWidth( 10 );
  __qPainter.setPen( __qPen );
  __qPainter.setWorldTransform( __qTransform );
  __qPainter.rotate( __fdTimeConverted*6.0 );
  __qPainter.drawLine( QPointF( 0, 0), QPointF( 0, -460 ) );

  // ... center mask
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
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
  __qPainter.setFont( __qFont );
  {
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( -425.0, -490.0 ) ), Qt::AlignCenter, CUnitTimeDelta::toString( fdSW1TimeAccumulated + ( fdSW1TimeStarted ? microtime()-fdSW1TimeStarted : 0.0 ), CUnitTimeDelta::HMS, 0 ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, -490.0 ) ), Qt::AlignCenter, CUnitTimeDelta::toString( fdSW2TimeAccumulated + ( fdSW2TimeStarted ? microtime()-fdSW2TimeStarted : 0.0 ), CUnitTimeDelta::HMS, 0 ) );
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, +490.0 ) ), Qt::AlignCenter, CUnitTime::toString( _fdTime, _eUnit, CUnitTime::HMS, 0 ) );
  }
}
