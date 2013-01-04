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

#ifndef QVCT_CCHARTGDALRASTER_HPP
#define QVCT_CCHARTGDALRASTER_HPP

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


/// GDAL-based (raster) chart
/**
 *  This class wraps a GDAL dataset to be rasterized as a chart in QVCT.
 *
 *  All color encodings are supported, EXCEPT non-palette CMYK.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChartGDALRaster: public CChartGDAL
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChartGDALRaster( const QString& _rqsFileName );
  virtual ~CChartGDALRaster();

};

#endif // QVCT_CCHARTGDALRASTER_HPP
