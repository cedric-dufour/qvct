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
#include <QDomElement> // QtXml module
#include <QKeyEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QRectF>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "QVCT.hpp"
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "charts/CChartGDALRaster.hpp"
#include "charts/CChartGDALElevation.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChart::CChart( const QString& _rqsFileName, QWidget* _pqParent )
  : QWidget( _pqParent )
  , poChartGDALRaster( 0 )
  , poChartGDALElevation( 0 )
  , bShowElevation( false )
{
  poChartGDALRaster = new CChartGDALRaster( _rqsFileName );
  if( poChartGDALRaster->getStatus() != QVCT::OK ) return;
  QPalette __qPalette( QWidget::palette() );
  __qPalette.setColor( QPalette::Background, Qt::black );
  QWidget::setPalette( __qPalette );
  QWidget::setAutoFillBackground(true);
  QWidget::setCursor( QCursor( QPixmap( ":cursors/crosshair.png" ), 15, 15 ) );
  qPointFDatPosition = QRectF( poChartGDALRaster->getDatGeometry() ).center();
  bPositionLock = true;
  fdZoom = 1.0;
  bZoomLock = true;
}

CChart::~CChart()
{
  if( poChartGDALElevation ) delete poChartGDALElevation;
  if( poChartGDALRaster ) delete poChartGDALRaster;
}

void CChart::addElevation( const QString& _rqsFileName )
{
  poChartGDALElevation = new CChartGDALElevation( _rqsFileName );
  if( poChartGDALElevation->getStatus() != QVCT::OK )
  {
    delete poChartGDALElevation;
    poChartGDALElevation = 0;
  }
}


//------------------------------------------------------------------------------
// METHODS: QWidget (OVERRIDE)
//------------------------------------------------------------------------------

void CChart::paintEvent( QPaintEvent* _pqPaintEvent )
{
  //qDebug( "DEBUG[%s]", Q_FUNC_INFO );
  qRectFDrawArea = QRectF( 0, 0, QWidget::width(), QWidget::height() );
  draw();
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CChart::resetDrawArea()
{
  qRectFDrawArea = QRectF( 0, 0, QWidget::width(), QWidget::height() );
}

void CChart::setGeoPosition( const CDataPosition& _roGeoPosition )
{
  qPointFDatPosition = poChartGDALRaster->toDatPosition( _roGeoPosition );
}

void CChart::showElevation( bool _bShow )
{
  if( !poChartGDALElevation ) return;
  if( bShowElevation != _bShow )
  {
    QVCTRuntime::usePointerOverlay()->forceRedraw();
    QVCTRuntime::useLandmarkOverlay()->forceRedraw();
    QVCTRuntime::useRouteOverlay()->forceRedraw();
    QVCTRuntime::useTrackOverlay()->forceRedraw();
    QVCTRuntime::useVesselOverlay()->forceRedraw();
  }
  bShowElevation = _bShow;
}

//
// GETTERS
//

QVCT::EStatus CChart::getStatus() const
{
  return poChartGDALRaster->getStatus();
}

QString CChart::getFileName() const
{
  return poChartGDALRaster->getFileName();
}

CDataPosition CChart::getGeoPositionCenter() const
{
  return poChartGDALRaster->toGeoPosition( poChartGDALRaster->getDatGeometry().center() );
}

CDataPosition CChart::getGeoPosition() const
{
  return poChartGDALRaster->toGeoPosition( qPointFDatPosition );
}

QPointF CChart::getDrawPositionCenter() const
{
  return qRectFDrawArea.center();
}

double CChart::getZoomFit() const
{
  QRect __qRectDatGeometry = poChartGDALRaster->getDatGeometry();
  double __fdZoomWidth = qRectFDrawArea.width() / (double)__qRectDatGeometry.width();
  double __fdZoomHeight = qRectFDrawArea.height() / (double)__qRectDatGeometry.height();
  return( __fdZoomWidth < __fdZoomHeight ? __fdZoomWidth : __fdZoomHeight );
}

double CChart::getZoomArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2 ) const
{
  double __fdLongitude1 = _roGeoPosition1.getLongitude(), __fdLongitude2 = _roGeoPosition2.getLongitude();
  double __fdLatitude1 = _roGeoPosition1.getLatitude(), __fdLatitude2 = _roGeoPosition2.getLatitude();
  CDataPosition __oGeoPosition( ( __fdLongitude1 + __fdLongitude2 ) / 2.0, ( __fdLatitude1 + __fdLatitude1 ) / 2.0 );
  double __fdResolution = poChartGDALRaster->getResolution( poChartGDALRaster->toDatPosition( __oGeoPosition ) );
  double __fdLatitudeMaxCos;
  if( __fdLatitude1 * __fdLatitude2 < 0 ) __fdLatitudeMaxCos = 0;
  else if( fabs( __fdLatitude1 ) < fabs( __fdLatitude2 ) ) __fdLatitudeMaxCos = __fdLatitude1;
  else __fdLatitudeMaxCos = __fdLatitude2;
  double __fdZoomWidth = qRectFDrawArea.width() / ( fabs( ( __fdLongitude2 - __fdLongitude1 ) * cos( __fdLatitudeMaxCos * QVCT::DEG2RAD ) ) * 111120.0 / __fdResolution );
  double __fdZoomHeight = qRectFDrawArea.height() / ( fabs( __fdLatitude2 - __fdLatitude1 ) * 111120.0 / __fdResolution );
  return( __fdZoomWidth < __fdZoomHeight ? __fdZoomWidth : __fdZoomHeight );
}

double CChart::getResolution() const
{
  return poChartGDALRaster->getResolution( qPointFDatPosition );
}

//
// OTHER
//

CDataPosition CChart::toGeoPosition( const QPointF& _rqPointFDrawPosition ) const
{
  CDataPosition __oGeoPosition = poChartGDALRaster->toGeoPosition( poChartGDALRaster->getDatPosition() + ( _rqPointFDrawPosition - qRectFDrawArea.center() ) / poChartGDALRaster->getZoom() );
  if( poChartGDALElevation ) __oGeoPosition.setElevation( poChartGDALElevation->getElevation( __oGeoPosition ) );
  return __oGeoPosition;
}

QPointF CChart::toDrawPosition( const CDataPosition& _roGeoPosition ) const
{
  if( bShowElevation ) return( ( poChartGDALElevation->toDatPosition( _roGeoPosition ) - poChartGDALElevation->getDatPosition() ) * poChartGDALElevation->getZoom() + qRectFDrawArea.center() );
  return( ( poChartGDALRaster->toDatPosition( _roGeoPosition ) - poChartGDALRaster->getDatPosition() ) * poChartGDALRaster->getZoom() + qRectFDrawArea.center() );
}

void CChart::move( const QPointF& _rqPointFDrawPositionOffset )
{
  qPointFDatPosition += _rqPointFDrawPositionOffset / poChartGDALRaster->getZoom();
}

void CChart::draw()
{
  // Constant drawing resources
  static const QPointF __qPointFCrosshairA1(5,0), __qPointFCrosshairA2(15,0);
  static const QPointF __qPointFCrosshairB1(0,5), __qPointFCrosshairB2(0,15);
  static const QPointF __qPointFCrosshairC1(-5,0), __qPointFCrosshairC2(-15,0);
  static const QPointF __qPointFCrosshairD1(0,-5), __qPointFCrosshairD2(0,-15);

  // Draw
  QPainter __qPainter;
  __qPainter.begin( this );
  __qPainter.setRenderHints( QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing );
  // ... chart / elevation model
  __qPainter.setOpacity( (double)QVCTRuntime::useSettings()->getChartOpacity()/100.0 );
  if( bShowElevation )
  {
    QPointF __qPointFDatPosition = poChartGDALElevation->toDatPosition( poChartGDALRaster->toGeoPosition( qPointFDatPosition ) );
    poChartGDALRaster->move( qPointFDatPosition, fdZoom ); // keep the actual chart position/zoom in sync.
    poChartGDALElevation->draw( &__qPainter,
                                __qPointFDatPosition,
                                fdZoom * poChartGDALElevation->getResolution( __qPointFDatPosition ) / poChartGDALRaster->getResolution( qPointFDatPosition ) );
  }
  else
  {
    if( poChartGDALElevation ) poChartGDALElevation->move( qPointFDatPosition, fdZoom ); // clear any cached rendering
    poChartGDALRaster->draw( &__qPainter, qPointFDatPosition, fdZoom );
  }
  __qPainter.setOpacity( 1.0 );
  // ... overlays
  QVCTRuntime::useLandmarkOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useRouteOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useTrackOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useVesselOverlay()->draw( this, &__qPainter );
  QVCTRuntime::usePointerOverlay()->draw( this, &__qPainter );
  // ... center crosshair
  QPen __qPen;
  __qPen.setColor( QColor( 0, 0, 0, 128 ) );
  __qPen.setWidth( 1 );
  __qPainter.setPen( __qPen );
  QPointF __qPointF = qRectFDrawArea.center();
  __qPainter.drawPoint( __qPointF );
  __qPainter.setPen( __qPen );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairA1, __qPointF + __qPointFCrosshairA2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairB1, __qPointF + __qPointFCrosshairB2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairC1, __qPointF + __qPointFCrosshairC2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairD1, __qPointF + __qPointFCrosshairD2 );
  // ... [end]
  __qPainter.end();
}

void CChart::print( QPrinter* _pqPrinter )
{
  // Printer
  QPainter __qPainter;
  __qPainter.begin( _pqPrinter );
  __qPainter.setRenderHints( QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing );
  // ... chart / elevation model
  if( bShowElevation )
  {
    QPointF __qPointFDatPosition = poChartGDALElevation->toDatPosition( poChartGDALRaster->toGeoPosition( qPointFDatPosition ) );
    poChartGDALRaster->move( qPointFDatPosition, fdZoom ); // keep the actual chart position/zoom in sync.
    poChartGDALElevation->draw( &__qPainter,
                                __qPointFDatPosition,
                                fdZoom * poChartGDALElevation->getResolution( __qPointFDatPosition ) / poChartGDALRaster->getResolution( qPointFDatPosition ) );
  }
  else
  {
    if( poChartGDALElevation ) poChartGDALElevation->move( qPointFDatPosition, fdZoom ); // clear any cached rendering
    poChartGDALRaster->draw( &__qPainter, qPointFDatPosition, fdZoom );
  }
  // ... overlays
  QVCTRuntime::useLandmarkOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useRouteOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useTrackOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useVesselOverlay()->draw( this, &__qPainter );
  QVCTRuntime::usePointerOverlay()->draw( this, &__qPainter );
  // ... [end]
  __qPainter.end();
}

void CChart::parseQVCT( const QDomElement& _rqDomElement )
{
  // Chart table
  if( _rqDomElement.hasAttribute( "longitude" ) && _rqDomElement.hasAttribute( "longitude" ) )
  {
    bPositionLock = false;
    CDataPosition __oGeoPosition( _rqDomElement.attribute( "longitude" ).toDouble(),
                                  _rqDomElement.attribute( "latitude" ).toDouble() );
    qPointFDatPosition = poChartGDALRaster->toDatPosition( __oGeoPosition );
  }
  if( _rqDomElement.hasAttribute( "zoom" ) )
  {
    bZoomLock = false;
    fdZoom = _rqDomElement.attribute( "zoom" ).toDouble();
  }
  if( _rqDomElement.hasAttribute( "dem" ) )
  {
    QString __qsFilename = _rqDomElement.attribute( "dem" );
    addElevation( __qsFilename );
    if( !hasElevation() )
    {
      QVCTRuntime::useMainWindow()->fileError( QVCT::OPEN, __qsFilename );
    }
  }
}

void CChart::dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const
{
  _rqXmlStreamWriter.writeStartElement( "Chart" );
  _rqXmlStreamWriter.writeAttribute( "raster", poChartGDALRaster->getFileName() );
  if( poChartGDALElevation ) _rqXmlStreamWriter.writeAttribute( "dem", poChartGDALElevation->getFileName() );
  if( !bPositionLock )
  {
    CDataPosition __oDataPosition = poChartGDALRaster->toGeoPosition( qPointFDatPosition );
    _rqXmlStreamWriter.writeAttribute( "longitude", QString::number( __oDataPosition.getLongitude() ) );
    _rqXmlStreamWriter.writeAttribute( "latitude", QString::number( __oDataPosition.getLatitude() ) );
  }
  if( !bZoomLock )
    _rqXmlStreamWriter.writeAttribute( "zoom", QString::number( fdZoom ) );
  _rqXmlStreamWriter.writeEndElement(); // Chart
}
