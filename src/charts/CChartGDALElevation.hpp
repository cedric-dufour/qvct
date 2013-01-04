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

#ifndef QVCT_CCHARTGDALELEVATION_HPP
#define QVCT_CCHARTGDALELEVATION_HPP

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
#include "charts/CChartGDAL.hpp"
class CDataPosition;


/// GDAL-based (elevation) chart
/**
 *  This class wraps a GDAL dataset to be used as data-elevation-model (DEM)
 *  or rasterized as an elevation chart in QVCT.
 *
 *  All scalar types (UInt16 ... Float64) Gray color encodings are supported.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChartGDALElevation: public CChartGDAL
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChartGDALElevation( const QString& _rqsFileName );
  virtual ~CChartGDALElevation();


  //------------------------------------------------------------------------------
  // METHODS: CChartGDAL (implements/override)
  //------------------------------------------------------------------------------

public:
  CDataPosition toGeoPosition( const QPointF& _rqPointFDatPosition ) const;

protected:
  virtual void rasterBuffer( QImage* _pqImage, const QVector<quint16>& _rqVector ) const;
  virtual void rasterBuffer( QImage* _pqImage, const QVector<qint16>& _rqVector ) const;
  virtual void rasterBuffer( QImage* _pqImage, const QVector<quint32>& _rqVector ) const;
  virtual void rasterBuffer( QImage* _pqImage, const QVector<qint32>& _rqVector ) const;
  virtual void rasterBuffer( QImage* _pqImage, const QVector<float>& _rqVector ) const;
  virtual void rasterBuffer( QImage* _pqImage, const QVector<double>& _rqVector ) const;


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Returns the elevation at the given chart pixel position
  double getElevation( const QPointF& _rqPointFDatPosition ) const;
  /// Returns the elevation at the given geographical position
  double getElevation( const CDataPosition& _roGeoPosition ) const;

private:
  uchar rasterValue( double _fdValue ) const;
  double interpolateValue( double _fdP11, double _fdP21, double _fdP12, double _fdP22, double _fdX, double _fdY ) const;

};

#endif // QVCT_CCHARTGDALELEVATION_HPP
