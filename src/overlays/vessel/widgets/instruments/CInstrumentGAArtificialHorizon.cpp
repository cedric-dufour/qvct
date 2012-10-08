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
#include <QDateTime>
#include <QFont>
#include <QPainter>
#include <QPointF>
#include <QPen>
#include <QPixmap>
#include <QSizeF>
#include <QWidget>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/pointer/CPointerPoint.hpp"
#include "overlays/vessel/widgets/instruments/CInstrumentGAArtificialHorizon.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrumentGAArtificialHorizon::CInstrumentGAArtificialHorizon( CVesselCockpit* _poVesselCockpit )
  : CInstrument( _poVesselCockpit )
  , bForceRedraw( true )
  , fdPitch( CDataCourse::UNDEFINED_BEARING )
  , fdRoll( CDataCourse::UNDEFINED_BEARING )
  , fdBearing( CDataCourse::UNDEFINED_BEARING )
  , fdTime( CDataTime::UNDEFINED_TIME )
{
}


//------------------------------------------------------------------------------
// METHODS: CInstrument (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGAArtificialHorizon::draw()
{
  // Retrieve parameters and values
  CVesselPoint* __poVesselPoint = CInstrument::useVesselPoint();
  double __fdSpeed = __poVesselPoint ? __poVesselPoint->GroundCourse.getSpeed() : CDataCourse::UNDEFINED_SPEED;
  double __fdSpeedVertical = __poVesselPoint ? __poVesselPoint->GroundCourse.getSpeedVertical() : CDataCourse::UNDEFINED_SPEED;
  double __fdBearing = __poVesselPoint ? __poVesselPoint->GroundCourse.getBearing() : CDataCourse::UNDEFINED_BEARING;
  double __fdTime = __poVesselPoint ? __poVesselPoint->getTime() : CDataTime::UNDEFINED_TIME;

  // Compute pitch and roll angles
  double __fdPitch = CDataCourse::UNDEFINED_BEARING, __fdRoll = CDataCourse::UNDEFINED_BEARING;
  if( __poVesselPoint )
  {
    if( __fdSpeed != CDataCourse::UNDEFINED_SPEED
        && __fdSpeedVertical != CDataCourse::UNDEFINED_SPEED
        && fabs( __fdSpeed ) > 5.0 ) // under 5m/s (10kt), aircraft is most likely still grounded
    {
      __fdPitch = atan( __fdSpeedVertical / __fdSpeed ) * QVCT::RAD2DEG;
    }
    if( __fdBearing != CDataCourse::UNDEFINED_BEARING
        && __fdTime != CDataTime::UNDEFINED_TIME )
    {
      if( fdBearing != CDataCourse::UNDEFINED_BEARING
          && fdTime != CDataTime::UNDEFINED_TIME )
      {
        if( __fdTime - fdTime > 0.0 )
        {
          double __fdSpeedAngular = ( __fdBearing - fdBearing ) / ( __fdTime - fdTime );
          __fdRoll = atan( __fdSpeed * __fdSpeedAngular*QVCT::DEG2RAD / 9.81 ) * QVCT::RAD2DEG;
        }
        else
          __fdRoll = fdRoll;
      }
      fdBearing = __fdBearing;
      fdTime = __fdTime;
    }
  }

  // Redraw pixmap only if required
  if( bForceRedraw
      || qPixmapBuffer.size() != QWidget::size()
      || fdPitch != __fdPitch
      || fdRoll != __fdRoll )
  {
    // Draw
    if( bForceRedraw
        || qPixmapBufferBackground.size() != QWidget::size() )
    {
      drawStatic();
    }
    drawDynamic( __poVesselPoint, __fdPitch, __fdRoll );

    // Update cache parameters and values
    bForceRedraw = false;
    fdPitch = __fdPitch;
    fdRoll = __fdRoll;
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

void CInstrumentGAArtificialHorizon::drawStatic()
{
  // Widget geometry
  QPointF __qPointFCenter = QPointF( QRectF( QWidget::rect() ).center() );
  double __fdSizeFactor = ( QWidget::width() < QWidget::height() ? (double)QWidget::width() : (double)QWidget::height() ) / 1200.0;

  // BACKGROUND
  {
    // Widget geometry (cont'd)
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
    __qPainter.setWorldTransform( __qTransform );

    // Draw

    // ... delimiting corona
    __qPen.setColor( Qt::white );
    __qPen.setWidth( 10.0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QColor( 32, 32, 32, 255 ) );
    __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 495.0, 495.0 );

    // ... small ticks
    for( int __i=-3; __i<=3; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 18.0;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 450.0*__qPointFAngle, 480.0*__qPointFAngle );
    }

    // ... large ticks
    for( int __i=-3; __i<=3; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 6.0;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 420.0*__qPointFAngle, 480.0*__qPointFAngle );
    }

    // ... horizon ticks
    __qPen.setWidth( 15.0 );
    __qPainter.setPen( __qPen );
    for( int __i=-1; __i<=1; __i++ )
    {
      if( !__i ) continue;
      double __fdAngle = (double)__i * QVCT::PI / 2.0;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 390.0*__qPointFAngle, 480.0*__qPointFAngle );
    }

    // ... 45-degrees ticks
    __qPen.setColor( Qt::white );
    __qPen.setWidth( 0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( Qt::white );
    __qPainter.rotate( -45.0 );
    for( int __i=0; __i<3; __i++ )
    {
      static const QPointF __pqPointFPolygon[3] =
        {
          QPointF( -30.0, -480.0 ),
          QPointF( 0.0, -410.0 ),
          QPointF( +30.0, -480.0 ),
        };
      __qPainter.drawConvexPolygon( __pqPointFPolygon, 3 );
      __qPainter.rotate( 45.0 );
    }

    // ... data
    __qPen.setColor( Qt::lightGray );
    __qPen.setWidth( 5.0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QColor( 32, 32, 32, 255 ) );
    __qFont.setPixelSize( 50.0 );
    __qFont.setBold( false );
    __qPainter.setFont( __qFont );
    __qPainter.setWorldTransform( __qTransform );
    {
      QRectF __qRectFFrame( QPointF( -150.0, -60.0 ), QSizeF( 300.0, 120.0 ) );
      QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( -425.0, +515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( -440.0, +540.0 ) ), Qt::AlignLeft, "PITCH" );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, +515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, +540.0 ) ), Qt::AlignRight, "ROLL" );
    }
  } // BACKGROUND

  // FOREGROUND
  {
    // Widget geometry (cont'd)
    QTransform __qTransform;
    __qTransform.translate( __qPointFCenter.x()*2.0, __qPointFCenter.y()*2.0 );
    __qTransform.scale( __fdSizeFactor, __fdSizeFactor );

    // Painter and resources
    if( qPixmapBufferForeground.size() != QWidget::size()*2.0 ) qPixmapBufferForeground = QPixmap( QWidget::size()*2.0 );
    qPixmapBufferForeground.fill( QColor( 224, 128, 0 ) );
    QPainter __qPainter( &qPixmapBufferForeground );
    QPen __qPen;
    QFont __qFont;
    __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    __qPainter.setWorldTransform( __qTransform );

    // Draw

    // ... sky
    __qPen.setColor( Qt::white );
    __qPen.setWidth( 0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QColor( 0, 192, 255 ) );
    __qPainter.drawRect( -1000.0, -1000.0, 2000.0, 1000.0 );

    // ... pitch marks
    __qPen.setWidth( 10.0 );
    __qPainter.setPen( __qPen );
    for( int __i=-6; __i<=6; __i++ )
    {
      __qPainter.drawLine( QPointF( -30.0, __i*30.0 ), QPointF( +30.0, __i*30.0 ) );
    }
    __qFont.setPixelSize( 40.0 );
    __qFont.setBold( true );
    __qPainter.setFont( __qFont );
    for( int __i=-3; __i<=3; __i++ )
    {
      int __j = fabs( __i );
      __qPainter.drawLine( QPointF( -75.0-__j*25.0, __i*60.0 ), QPointF( +75.0+__j*25.0, __i*60.0 ) );
      if( !__i ) continue;
      QRectF __qRectFText( QPointF( -40.0, -20.0 ), QSizeF( 80.0, 40.0 ) );
      __qPainter.drawText( __qRectFText.translated( QPointF( -125.0-__j*25.0, __i*60.0 ) ), Qt::AlignCenter, QString::number( __j*10 ) );
      __qPainter.drawText( __qRectFText.translated( QPointF( +125.0+__j*25.0, __i*60.0 ) ), Qt::AlignCenter, QString::number( __j*10 ) );
    }

    // ... horizon mark
    __qPen.setWidth( 15.0 );
    __qPainter.setPen( __qPen );
    __qPainter.drawLine( QPointF( -1000.0, 0.0 ), QPointF( +1000.0, 0.0 ) );
  } // FOREGROUND

}

void CInstrumentGAArtificialHorizon::drawDynamic( const CVesselPoint* _poVesselPoint, double _fdPitch, double _fdRoll )
{
  double __fdPitchConverted = _fdPitch != CDataCourse::UNDEFINED_BEARING ? _fdPitch : 0.0;
  double __fdRollConverted = _fdRoll != CDataCourse::UNDEFINED_BEARING ? _fdRoll : 0.0;

  // Widget geometry
  QPointF __qPointFCenter = QPointF( QRectF( QWidget::rect() ).center() );
  double __fdSizeFactor = ( QWidget::width() < QWidget::height() ? (double)QWidget::width() : (double)QWidget::height() ) / 1200.0;
  QTransform __qTransformNonScaled;
  __qTransformNonScaled.translate( __qPointFCenter.x(), __qPointFCenter.y() );
  QTransform __qTransformScaled;
  __qTransformScaled.translate( __qPointFCenter.x(), __qPointFCenter.y() );
  __qTransformScaled.scale( __fdSizeFactor, __fdSizeFactor );

  // Painter and resources
  qPixmapBuffer = QPixmap( qPixmapBufferBackground );
  QPainter __qPainter( &qPixmapBuffer );
  QPen __qPen;
  QFont __qFont;
  __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );

  // Draw

  // ... foreground
  QPainterPath __qPainterPath;
  __qPainterPath.addEllipse( __qPointFCenter, 390.0*__fdSizeFactor, 390.0*__fdSizeFactor );
  __qPainter.setClipPath( __qPainterPath );
  __qPainter.setWorldTransform( __qTransformNonScaled );
  __qPainter.translate( 0.0, __fdPitchConverted*6.0*__fdSizeFactor );
  __qPainter.rotate( -__fdRollConverted );
  __qPainter.drawPixmap( QPointF( -(double)qPixmapBufferForeground.width()/2.0, -(double)qPixmapBufferForeground.height()/2.0 ), qPixmapBufferForeground );
  __qPainter.setClipping( false );
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 10.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setWorldTransform( __qTransformScaled );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 390.0, 390.0 );

  // ... attitude marks
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPen.setCapStyle( Qt::SquareCap );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QColor( 255, 224, 0 ) );
  __qPainter.setPen( __qPen );
  __qPainter.drawRoundedRect( QRectF( QPointF( -175.0, -10.0 ), QPointF( -50.0, +10.0 ) ), 10.0, 10.0 );
  __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 10.0, 10.0 );
  __qPainter.drawRoundedRect( QRectF( QPointF( +50.0, -10.0 ), QPointF( +175.0, +10.0 ) ), 10.0, 10.0 );
  __qPainter.rotate( -__fdRollConverted );
  {
    static const QPointF __pqPointFPolygon[3] =
      {
        QPointF( -30.0, -310.0 ),
        QPointF( 0.0, -380.0 ),
        QPointF( +30.0, -310.0 ),
      };
    __qPainter.drawConvexPolygon( __pqPointFPolygon, 3 );
  }

  // ... data
  __qPen.setColor( Qt::white );
  __qPainter.setPen( __qPen );
  __qFont.setPixelSize( 50.0 );
  __qFont.setBold( false );
  __qPainter.setFont( __qFont );
  __qPainter.setWorldTransform( __qTransformScaled );
  {
    QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
    QString __qsPITCH( "n/a" ), __qsROLL( "n/a" );
    if( _poVesselPoint )
    {
      if( _fdPitch != CDataCourse::UNDEFINED_BEARING )
        __qsPITCH = CUnitBearing::toString( _fdPitch, CUnitBearing::DEG, 0 );
      if( _fdRoll != CDataCourse::UNDEFINED_BEARING )
      __qsROLL = CUnitBearing::toString( _fdRoll, CUnitBearing::DEG, 0 );
    }
    __qFont.setItalic( !_poVesselPoint || _fdPitch == CDataCourse::UNDEFINED_BEARING || !_poVesselPoint->GroundCourseValidity.isValidSpeed() || !_poVesselPoint->GroundCourseValidity.isValidSpeedVertical() );
    __qPainter.setFont( __qFont );
    __qPainter.drawText( __qRectFText.translated( QPointF( -425.0, +490.0 ) ), Qt::AlignCenter, __qsPITCH );
    __qFont.setItalic( !_poVesselPoint || _fdRoll == CDataCourse::UNDEFINED_BEARING || !_poVesselPoint->GroundCourseValidity.isValidBearing() );
    __qPainter.setFont( __qFont );
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, +490.0 ) ), Qt::AlignCenter, __qsROLL );
  }

  // ... OFF flag
  if( !_poVesselPoint )
  {
    __qPen.setColor( Qt::black );
    __qPen.setWidth( 0 );
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
