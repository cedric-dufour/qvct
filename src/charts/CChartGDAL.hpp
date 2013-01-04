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

#ifndef QVCT_CCHARTGDAL_HPP
#define QVCT_CCHARTGDAL_HPP

// QT
#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QString>
#include <QVector>

// GDAL
#include <gdal_priv.h>

// QVCT
#include "QVCT.hpp"
class CDataPosition;


/// Generic GDAL-based dataset
/**
 *  This class defines the generic resources to wrap a GDAL dataset and use it
 *  as a chart along Qt drawing resources (QPainter, QImage, etc.).
 *
 *  The GDAL dataset internal projection data are used to convert pixel to
 *  geographical coordinates and vice-versa (using QVCT internal Longitude/Latitude
 *  WGS84 reference).
 *
 *  In order to improve I/O performances, rendering (rasterization) is performed
 *  only when necessary (the rendering buffer being cached between calls to the
 *  drawing primitive).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChartGDAL
{

  //------------------------------------------------------------------------------
  // CONSTANTS
  //------------------------------------------------------------------------------

public:
  /// Underlying GDAL dataset color encoding IDs
  enum EColorEncoding { GRAY, GRAY_U16, GRAY_S16, GRAY_U32, GRAY_S32, GRAY_F32, GRAY_F64,
                        PALETTE_GRAY, PALETTE_RGB, PALETTE_CMYK, PALETTE_HSL, ARGB, RGB, YUV };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  //
  // THIS
  //

  /// Chart internal status
  QVCT::EStatus eStatus;
  /// Chart (GDAL dataset) file name
  QString qsFileName;
  /// Chart (GDAL dataset) geometry (size)
  QRect qRectGeometry;
  /// Chart (GDAL dataset) color encoding
  EColorEncoding eColorEncoding;
  /// Last drawn chart (GDAL dataset) center pixel position (requested by user)
  QPointF qPointFDatPositionWished;
  /// Last drawn chart (GDAL dataset) center pixel position (actually rendered)
  QPointF qPointFDatPositionActual;
  /// Last drawn chart (GDAL dataset) zoom factor (requested by user)
  double fdZoomWished;
  /// Last drawn chart (GDAL dataset) zoom factor (actually rendered)
  double fdZoomActual;

  //
  // GDAL
  //

  /// Underlying GDAL dataset
  GDALDataset* poGDALDataset;
  /// Underlying GDAL dataset projection transformer (to internal WGS84)
  void* pGDALProjectionTransformer;
  /// Underlying GDAL dataset band mapping (for ARGB/RGB/YUV color encodings)
  int* piGDALBandMap;

  //
  // Qt
  //

  /// Pixmap buffer where the chart (GDAL dataset) is rendered (rasterized)
  QPixmap qPixmapBuffer;
  /// Color table (for GRAY/PALETTE_* color encodings)
  QVector<QRgb> qColorTable;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CChartGDAL();
  virtual ~CChartGDAL();

protected:
  /// Opens the underlying GDAL dataset
  void open( const QString& _rqsFileName );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // GETTERS
public:
  /// Returns the chart internal status (after constructing object)
  QVCT::EStatus getStatus() const { return eStatus; };
  /// Returns the chart (GDAL dataset) file name
  QString getFileName() const { return qsFileName; };
  /// Returns the chart (GDAL dataset) geometry (size)
  QRect getDatGeometry() const { return qRectGeometry; };
  /// Returns the chart (GDAL dataset) color encoding
  EColorEncoding getColorEncoding() const { return eColorEncoding; };
  /// Returns the last drawn chart (GDAL dataset) center pixel position
  QPointF getDatPosition() const { return qPointFDatPositionActual; };
  /// Returns the last drawn chart (GDAL dataset) zoom factor
  double getZoom() const { return fdZoomActual; };

  // OTHER
public:
  /// Converts the given chart pixel position to geographical position
  CDataPosition toGeoPosition( const QPointF& _rqPointFDatPosition ) const;
  /// Converts the given (geographical) position [long,lat,elev] to chart (GDAL dataset) point [px]
  QPointF toDatPosition( const CDataPosition& _roGeoPosition ) const;
  /// Returns the resolution at given chart (GDAL dataset) point [px], in meters per pixel [m/px]
  double getResolution( const QPointF& _rqPointFDatPosition ) const;
  /// Draws chart, centered on given chart (GDAL dataset) point [px] and zoomed according to given factor
  void draw( QPainter* _pqPainter, const QPointF& _rqPointFDatPosition, double _fdZoom );
  /// Moves the chart, centered on given chart (GDAL dataset) point [px] and zoomed according to given factor
  void move( const QPointF& _rqPointFDatPosition, double _fdZoom );

protected:
  /// Rasterizes UInt16 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<quint16>& _rqVector ) const;
  /// Rasterizes Int16 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<qint16>& _rqVector ) const;
  /// Rasterizes UInt32 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<quint32>& _rqVector ) const;
  /// Rasterizes Int32 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<qint32>& _rqVector ) const;
  /// Rasterizes Float32 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<float>& _rqVector ) const;
  /// Rasterizes Float64 buffer
  virtual void rasterBuffer( QImage* _pqImage, const QVector<double>& _rqVector ) const;

};

#endif // QVCT_CCHARTGDAL_HPP
