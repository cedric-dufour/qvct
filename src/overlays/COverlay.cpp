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
#include <QTreeWidgetItem>
#include <QWidget>

// QVCT
#include "charts/CChart.hpp"
#include "overlays/COverlay.hpp"
#include "overlays/COverlayObject.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlay::COverlay( const QString& _rqsName )
  : QTreeWidgetItem( COverlayObject::OVERLAY )
  , qsName( _rqsName )
  , bVisible( true )
  , poChartPixmap( 0 )
  , bForceRedraw( true )
{
  // Painter dependencies
  qFont.setPixelSize( 12 );
  qBrushText.setStyle( Qt::SolidPattern );
  qBrushText.setColor( QColor( 255, 255, 255, 160 ) );
  qPenText.setColor( QColor( 0, 0, 0, 192 ) );
  qPenText.setStyle( Qt::SolidLine );
  qPenText.setWidth( 1 );
  qPenText.setCapStyle( Qt::RoundCap );
  qBrushMarker.setStyle( Qt::SolidPattern );
  qBrushMarker.setColor( QColor( 0, 0, 0, 128 ) );
  qPenMarker.setColor( QColor( 0, 0, 0, 192 ) );
  qPenMarker.setStyle( Qt::SolidLine );
  qPenMarker.setWidth( 4 );
  qPenMarker.setCapStyle( Qt::RoundCap );
  qPenLine.setColor( QColor( 0, 0, 0, 192 ) );
  qPenLine.setStyle( Qt::SolidLine );
  qPenLine.setWidth( 5 );
  qPenLine.setCapStyle( Qt::RoundCap );
  qPenVector.setColor( QColor( 0, 0, 0, 192 ) );
  qPenVector.setStyle( Qt::SolidLine );
  qPenVector.setWidth( 3 );
  qPenVector.setCapStyle( Qt::RoundCap );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void COverlay::draw( const CChart* _poChart, QPainter* _pqPainter )
{
  // Exit if we're not visible
  if( !bVisible ) return;

  // Redraw pixmap only if required
  QSize __qSizePixmap = _poChart->getDrawArea().toRect().size();
  QPointF __qPointFDatPositionPixmap = _poChart->getDatPosition();
  double __fdZoomPixmap = _poChart->getZoom();
  if( poChartPixmap != _poChart
      || qPixmapBuffer.size() != __qSizePixmap
      || qPointFDatPositionPixmap != __qPointFDatPositionPixmap
      || fdZoomPixmap != __fdZoomPixmap
      || bForceRedraw )
  {
    //qDebug( "DEBUG[%s] Rendering", Q_FUNC_INFO );

    // Reset pixmap buffer
    poChartPixmap = _poChart;
    if( qPixmapBuffer.size() != __qSizePixmap ) qPixmapBuffer = QPixmap( __qSizePixmap );
    qPixmapBuffer.fill( Qt::transparent );
    qPointFDatPositionPixmap = __qPointFDatPositionPixmap;
    fdZoomPixmap = __fdZoomPixmap;
    bForceRedraw = false;

    // Create buffer painter
    QPainter __qPainter( &qPixmapBuffer );
    __qPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    __qPainter.setFont( qFont );

    // Draw
    drawContent( _poChart, &__qPainter );
  }
  _pqPainter->drawPixmap( 0, 0, qPixmapBuffer );

}

QString COverlay::newChildName( const QString& _rqsName, int __iZeroPrefix, bool __bForceSuffix ) const
{
  QString __qsName;
  int __iSuffix = 0;
  bool __bValid;
  do
  {
    __qsName = _rqsName;
    if( __bForceSuffix || __iSuffix ) __qsName += "("+QString::number( __iSuffix+1 ).prepend( QString( __iZeroPrefix, '0' ) ).right( __iZeroPrefix+1 )+")";
    __bValid = true;
    for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    {
      if( __qsName == ((COverlayObject*)QTreeWidgetItem::child( __i ))->getName() )
      {
        __bValid = false;
        __iSuffix++;
        break;
      }
    }
  }
  while( !__bValid );
  return __qsName;
}
