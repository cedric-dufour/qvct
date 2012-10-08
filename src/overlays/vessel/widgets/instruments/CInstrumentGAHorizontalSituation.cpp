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
#include "overlays/vessel/widgets/instruments/CInstrumentGAHorizontalSituation.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CInstrumentGAHorizontalSituation::CInstrumentGAHorizontalSituation( CVesselCockpit* _poVesselCockpit )
  : CInstrument( _poVesselCockpit )
  , bForceRedraw( true )
  , fdBearingVessel( CDataCourse::UNDEFINED_BEARING )
  , oPositionTarget( CDataPosition::UNDEFINED )
  , fdBearingRadial( 0.0 )
{
}


//------------------------------------------------------------------------------
// METHODS: QWidget (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGAHorizontalSituation::mouseReleaseEvent( QMouseEvent* _pqMouseEvent )
{
  QPointF __qPointFMouse = _pqMouseEvent->posF();
  if( __qPointFMouse.x() < QWidget::size().width()/2 )
  { // left-hand side
    fdBearingRadial -= 1.0;
    if( fdBearingRadial < 0.0 ) fdBearingRadial += 360.0;
  }
  else
  { // right-hand side
    fdBearingRadial += 1.0;
    if( fdBearingRadial >= 360.0 ) fdBearingRadial -= 360.0;
  }
  bForceRedraw = true;
  QWidget::update();
}

void CInstrumentGAHorizontalSituation::mouseDoubleClickEvent( QMouseEvent* _pqMouseEvent )
{
  // NOTE: don't forget that the widget also receives the two corresponding mouseReleaseEvents
  QPointF __qPointFMouse = _pqMouseEvent->posF();
  if( __qPointFMouse.x() < QWidget::size().width()/2 )
  { // left-hand side
    fdBearingRadial -= 8.0;
    if( fdBearingRadial < 0.0 ) fdBearingRadial += 360.0;
  }
  else
  { // right-hand side
    fdBearingRadial += 8.0;
    if( fdBearingRadial >= 360.0 ) fdBearingRadial -= 360.0;
  }
  bForceRedraw = true;
  QWidget::update();
}


//------------------------------------------------------------------------------
// METHODS: CInstrument (implement/override)
//------------------------------------------------------------------------------

void CInstrumentGAHorizontalSituation::draw()
{
  // Retrieve parameters and values
  CVesselPoint* __poVesselPoint = CInstrument::useVesselPoint();
  double __fdBearingVessel = __poVesselPoint ? __poVesselPoint->GroundCourse.getBearing() : 0.0;
  CPointerPoint* __poPointerPoint = QVCTRuntime::usePointerOverlay()->usePointerTarget();

  // Redraw pixmap only if required
  if( bForceRedraw
      || qPixmapBuffer.size() != QWidget::size()
      || fdBearingVessel != __fdBearingVessel
      || oPositionTarget != *__poPointerPoint )
  {
    // Draw resources
    if( bForceRedraw
        || qPixmapBufferBackground.size() != QWidget::size() )
    {
      drawStatic();
    }
    drawDynamic( __poVesselPoint, __poPointerPoint );

    // Update cache parameters and values
    bForceRedraw = false;
    fdBearingVessel = __fdBearingVessel;
    oPositionTarget = *__poPointerPoint;
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

void CInstrumentGAHorizontalSituation::drawStatic()
{
  // Widget geometry
  QPointF __qPointFCenter = QPointF( QRectF( QWidget::rect() ).center() );
  double __fdSizeFactor = ( QWidget::width() < QWidget::height() ? (double)QWidget::width() : (double)QWidget::height() ) / 1200.0;
  QTransform __qTransform;
  __qTransform.translate( __qPointFCenter.x(), __qPointFCenter.y() );
  __qTransform.scale( __fdSizeFactor, __fdSizeFactor );

  // BACKGROUND
  {
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
    __qPen.setColor( Qt::black );
    __qPainter.setPen( __qPen );
    __qPainter.drawEllipse( QPointF( 0.0, 0.0 ), 360.0, 360.0 );

    // ... data
    __qPen.setColor( Qt::lightGray );
    __qPen.setWidth( 5.0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QColor( 32, 32, 32, 255 ) );
    __qFont.setPixelSize( 50.0 );
    __qFont.setBold( false );
    __qPainter.setFont( __qFont );
    {
      QRectF __qRectFFrame( QPointF( -150.0, -60.0 ), QSizeF( 300.0, 120.0 ) );
      QRectF __qRectFText( QPointF( -125.0, -30.0 ), QSizeF( 250.0, 60.0 ) );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( -425.0, -515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( -440.0, -540.0 ) ), Qt::AlignLeft, "QDM/QDR" );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, -515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, -540.0 ) ), Qt::AlignRight, "TH" );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( -425.0, +515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( -440.0, +540.0 ) ), Qt::AlignLeft, "DIST" );
      __qPainter.drawRoundedRect( __qRectFFrame.translated( QPointF( +425.0, +515.0 ) ), 10.0, 10.0 );
      __qPainter.drawText( __qRectFText.translated( QPointF( +440.0, +540.0 ) ), Qt::AlignRight, "ETE" );
    }
  } // BACKGROUND

  // BEARING
  {
    // Painter and resources
    if( qPixmapBufferBearing.size() != QWidget::size() ) qPixmapBufferBearing = QPixmap( QWidget::size() );
    qPixmapBufferBearing.fill( Qt::transparent );
    QPainter __qPainter( &qPixmapBufferBearing );
    QPen __qPen;
    QFont __qFont;
    __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    __qPainter.setWorldTransform( __qTransform );

    // Draw

    // ... small (5-degrees) ticks
    __qPen.setColor( Qt::white );
    __qPen.setWidth( 10.0 );
    __qPainter.setPen( __qPen );
    for( int __i=0; __i<72; __i++ )
    {
      double __fdAngle = (double)__i * QVCT::PI / 36.0;
      QPointF __qPointFAngle( sin( __fdAngle ), -cos( __fdAngle ) );
      __qPainter.drawLine( 460.0*__qPointFAngle, 480.0*__qPointFAngle );
    }

    // ... large (10-degrees) ticks and digits
    __qFont.setPixelSize( 60.0 );
    __qFont.setBold( true );
    __qPainter.setFont( __qFont );
    {
      QRectF __qRectFText( QPointF( -60.0, -30.0 ), QSizeF( 120.0, 60.0 ) );
      for( int __i=0; __i<36; __i++ )
      {
        __qPainter.drawLine( QPointF( 0.0, -440.0 ), QPointF( 0.0, -480.0 ) );
        if( __i % 3 == 0 )
        {
          __qPainter.drawText( __qRectFText.translated( QPointF( 0.0, -400.0 ) ), Qt::AlignCenter, QString::number( __i ) );
        }
        __qPainter.rotate( 10.0 );
      }
    }
  } // BEARING

  // TARGET
  {
    // Painter and resources
    if( qPixmapBufferTarget.size() != QWidget::size() ) qPixmapBufferTarget = QPixmap( QWidget::size() );
    qPixmapBufferTarget.fill( Qt::transparent );
    QPainter __qPainter( &qPixmapBufferTarget );
    QPen __qPen;
    __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    __qPainter.setWorldTransform( __qTransform );

    // Draw

    // ... 2-degrees ticks
    __qPen.setColor( Qt::white );
    __qPen.setWidth( 10.0 );
    __qPainter.setPen( __qPen );
    for( int __i=-5; __i<=5; __i++ )
    {
      double __fdOffset = (double)__i*50.0;
      __qPainter.drawEllipse( QPointF( __fdOffset, 0.0 ), 10.0, 10.0 );
      __qPainter.drawLine( QPointF( __fdOffset, -20.0 ), QPointF( __fdOffset, +20.0 ) );
    }

    // ... direction
    __qPen.setColor( Qt::black );
    __qPen.setWidth( 0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( QColor( 255, 224, 0 ) );
    {
      static const QPointF __pqPointFPolygon[7] =
        {
          QPointF( -10.0, -200.0 ),
          QPointF( -10.0, -300.0 ),
          QPointF( -40.0, -290.0 ),
          QPointF( 0.0, -350.0 ),
          QPointF( +40.0, -290.0 ),
          QPointF( +10.0, -300.0 ),
          QPointF( +10.0, -200.0 ),
        };
      __qPainter.drawConvexPolygon( __pqPointFPolygon, 7 );
    }
    __qPainter.drawRect( QRectF( QPointF( -10.0, +200.0 ), QPointF( +10.0, +350.0 ) ) );
  } // TARGET

  // FOREGROUND
  {
    // Painter and resources
    if( qPixmapBufferForeground.size() != QWidget::size() ) qPixmapBufferForeground = QPixmap( QWidget::size() );
    qPixmapBufferForeground.fill( Qt::transparent );
    QPainter __qPainter( &qPixmapBufferForeground );
    QPen __qPen;
    __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

    // Draw
    __qPainter.setWorldTransform( __qTransform );

    // ... center mark
    __qPen.setColor( QColor( 255, 128, 0 ) );
    __qPen.setWidth( 15.0 );
    __qPainter.setPen( __qPen );
    __qPainter.drawLine( QPointF( 0.0, -50.0 ), QPointF( 0.0, 100.0 ) );
    __qPainter.drawLine( QPointF( -75.0, 0.0 ), QPointF( +75.0, 0.0 ) );
    __qPainter.drawLine( QPointF( -25.0, 75.0 ), QPointF( +25.0, 75.0 ) );

    // ...   corona marks
    __qPen.setColor( Qt::black );
    __qPen.setWidth( 0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush(QColor( 255, 128, 0 ) );
    for( int __i=0; __i<4; __i++ )
    {
      static const QPointF __pqPointFPolygon[3] =
        {
          QPointF( -50.0, -480.0 ),
          QPointF( 0.0, -430.0 ),
          QPointF( +50.0, -480.0 ),
        };
      __qPainter.drawConvexPolygon( __pqPointFPolygon, 3 );
      __qPainter.rotate( 90.0 );
    }
    __qPainter.rotate( 45.0 );
    for( int __i=0; __i<4; __i++ )
    {
      static const QPointF __pqPointFPolygon[3] =
        {
          QPointF( -30.0, -480.0 ),
          QPointF( 0.0, -450.0 ),
          QPointF( +30.0, -480.0 ),
        };
      __qPainter.drawConvexPolygon( __pqPointFPolygon, 3 );
      __qPainter.rotate( 90.0 );
    }
  } // FOREGROUND

}

void CInstrumentGAHorizontalSituation::drawDynamic( const CVesselPoint* _poVesselPoint, const CPointerPoint* _poPointerPoint )
{
  double __fdBearingVessel = _poVesselPoint ? _poVesselPoint->GroundCourse.getBearing() : 0.0;
  bool __bTarget = _poPointerPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED );
  double __fdBearingTarget = __bTarget ? CDataPosition::bearingRL( *_poVesselPoint, *_poPointerPoint ) : __fdBearingVessel;

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

  // ... bearing corona
  __qPainter.setWorldTransform( __qTransformNonScaled );
  __qPainter.rotate( -__fdBearingVessel );
  __qPainter.drawPixmap( QPointF( -(double)qPixmapBufferBearing.width()/2.0, -(double)qPixmapBufferBearing.height()/2.0 ), qPixmapBufferBearing );

  // ... target indicator
  __qPainter.setWorldTransform( __qTransformNonScaled );
  __qPainter.rotate( __fdBearingTarget - __fdBearingVessel );
  __qPainter.drawPixmap( QPointF( -(double)qPixmapBufferTarget.width()/2.0, -(double)qPixmapBufferTarget.height()/2.0 ), qPixmapBufferTarget );

  // ... radial deviation
  __qPainter.setWorldTransform( __qTransformScaled );
  __qPainter.rotate( __fdBearingTarget - __fdBearingVessel );
  double __fdBearingDeviation = 0.0;
  if( _poVesselPoint && __bTarget )
  {
    __fdBearingDeviation = __fdBearingTarget - fdBearingRadial;
    if( __fdBearingDeviation <= -180.0 ) __fdBearingDeviation += 360;
    else if( __fdBearingDeviation > 180.0 ) __fdBearingDeviation -= 360;
    bool _bTargetTo = true;
    if( __fdBearingDeviation < -90.0 )
    {
      _bTargetTo = false;
      __fdBearingDeviation = -180.0 - __fdBearingDeviation;
    }
    if( __fdBearingDeviation > 90.0 )
    {
      _bTargetTo = false;
      __fdBearingDeviation = 180.0 - __fdBearingDeviation;
    }
    __qPen.setColor( Qt::black );
    __qPen.setWidth( 0 );
    __qPainter.setPen( __qPen );
    __qPainter.setBrush( Qt::white );
    QPointF __pqPointFPolygon[3] =
      {
        QPointF( 50.0, _bTargetTo ? -50.0 : +50.0 ),
        QPointF( 100.0, _bTargetTo ? -150.0 : +150.0 ),
        QPointF( 150.0, _bTargetTo ? -50.0 : +50.0 ),
      };
    __qPainter.drawConvexPolygon( __pqPointFPolygon, 3 );
  }
  if( __fdBearingDeviation > 13.0 ) __fdBearingDeviation = 13.0;
  else if( __fdBearingDeviation < -13.0 ) __fdBearingDeviation = -13.0;
  __qPen.setColor( Qt::black );
  __qPen.setWidth( 0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( QColor( 255, 224, 0 ) );
  __qPainter.drawRect( QRectF( QPointF( __fdBearingDeviation*25.0-10.0, -200.0 ), QPointF( __fdBearingDeviation*25.0+10.0, +200.0 ) ) );

  // ... foreground
  __qPainter.resetTransform();
  __qPainter.drawPixmap( 0, 0, qPixmapBufferForeground );

  // ... radial
  __qPen.setColor( QColor( 255, 128, 0 ) );
  __qPen.setWidth( 10.0 );
  __qPainter.setPen( __qPen );
  __qPainter.setBrush( Qt::transparent );
  __qPainter.setWorldTransform( __qTransformScaled );
  __qPainter.rotate( fdBearingRadial - __fdBearingVessel );
  {
    static const QPointF __pqPointFPolygon[7] =
      {
        QPointF( -75.0, -490.0 ),
        QPointF( -75.0, -455.0 ),
        QPointF( -35.0, -455.0 ),
        QPointF( 0.0, -420.0 ),
        QPointF( +35.0, -455.0 ),
        QPointF( +75.0, -455.0 ),
        QPointF( +75.0, -490.0 ),
      };
    __qPainter.drawConvexPolygon( __pqPointFPolygon, 7 );
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
    QString __qsTH( "n/a" ), __qsDIST( "n/a" ), __qsETE( "n/a" );
    if( _poVesselPoint && __bTarget )
    {
      CSettings* __poSettings = QVCTRuntime::useSettings();
      double __fdDistance = CDataPosition::distanceRL( *_poVesselPoint, *_poPointerPoint );
      __qsTH = CUnitBearing::toString( __fdBearingTarget, CUnitBearing::DEG, 0 );
      __qsDIST = CUnitDistance::toString( __fdDistance, __poSettings->getUnitDistance(), 0 );
      do // processing-breaking loop
      {
        double __fdSpeed = _poVesselPoint->GroundCourse.getSpeed();
        if( __fdSpeed == CDataCourse::UNDEFINED_SPEED ) break;
        double __fdSpeedCosine = __fdSpeed * cos( ( __fdBearingVessel - __fdBearingTarget ) * QVCT::DEG2RAD );
        if( __fdSpeedCosine <= 0 ) break; // we can't get any ETE/ETA if we're moving away from the target (or not moving at all)
        double __fdDuration = __fdDistance / __fdSpeedCosine;
        if( __fdDuration > 86400 ) break; // if the ETE is more than 24 hours, let's assume it's not worth displaying it
        __qsETE = CUnitTimeDelta::toString( __fdDuration, CUnitTimeDelta::HM, 0 );
      }
      while( false ); // processing-breaking loop
    }
    __qPainter.drawText( __qRectFText.translated( QPointF( -425.0, -490.0 ) ), Qt::AlignCenter, CUnitBearing::toString( fdBearingRadial, CUnitBearing::DEG, 0) );
    __qFont.setItalic( !_poVesselPoint || !_poVesselPoint->isValidPosition() );
    __qPainter.setFont( __qFont );
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, -490.0 ) ), Qt::AlignCenter, __qsTH );
    __qPainter.drawText( __qRectFText.translated( QPointF( -425.0, +490.0 ) ), Qt::AlignCenter, __qsDIST );
    __qFont.setItalic( !_poVesselPoint || !_poVesselPoint->isValidPosition() || !_poVesselPoint->GroundCourseValidity.isValidBearing() || !_poVesselPoint->GroundCourseValidity.isValidSpeed() );
    __qPainter.setFont( __qFont );
    __qPainter.drawText( __qRectFText.translated( QPointF( +425.0, +490.0 ) ), Qt::AlignCenter, __qsETE );
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
