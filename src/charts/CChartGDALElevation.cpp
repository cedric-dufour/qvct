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

// GDAL
#include <gdal.h>
#include <gdal_alg.h>

// QVCT
#include "data/CDataPosition.hpp"
#include "charts/CChartGDALElevation.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChartGDALElevation::CChartGDALElevation( const QString& _rqsFileName )
  : CChartGDAL()
{
  open( _rqsFileName );

  // Check data type
  if( eStatus == QVCT::OK )
  {
    switch( eColorEncoding )
    {
    case GRAY_U16:
    case GRAY_S16:
    case GRAY_U32:
    case GRAY_S32:
    case GRAY_F32:
    case GRAY_F64:
      break;
    default:
      qCritical( "ERROR[%s]: Unsupported data type (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
      eStatus = QVCT::ERROR;
      return;
    }
  }

  // Update palette for elevation rendering
  qColorTable[0] = qRgba( 0, 0, 0, 0 ); // no value
  for( int i=1; i<=127; i++ ) qColorTable[i] = qRgba( 0, 48+0.375*i, 96+0.75*i, 255 ); // -9'921.875m to -78.125m; blue (water)
  for( int i=128; i<=152; i++ ) qColorTable[i] = qRgba( 4*i-480, 128, 0, 255 ); // 0.0m to 1'875.0m; green (greenery)
  for( int i=153; i<=183; i++ ) qColorTable[i] = qRgba( 4*i-480, 4*i-480, 8*i-1216, 255 ); // 1'953.125m to 4'296.875m; fade to white (snow)
  for( int i=184; i<=255; i++ ) qColorTable[i] = qRgba( 439-i, 255, 255, 255 ); // 4'375.0m to 9'921.875m; fade to cyan (ice)
}

CChartGDALElevation::~CChartGDALElevation()
{}


//------------------------------------------------------------------------------
// METHODS: CChartGDAL (implements/override)
//------------------------------------------------------------------------------

CDataPosition CChartGDALElevation::toGeoPosition( const QPointF& _rqPointFDatPosition ) const
{
  CDataPosition __oGeoPosition;
  if( eStatus != QVCT::OK ) return __oGeoPosition;
  double __fdX = (double)_rqPointFDatPosition.x(), __fdY = (double)_rqPointFDatPosition.y(), __fdZ = 0;
  int __iSuccess = TRUE;
  GDALGenImgProjTransform( pGDALProjectionTransformer, false, 1, &__fdX, &__fdY, &__fdZ, &__iSuccess );
  if( __iSuccess != TRUE ) return __oGeoPosition;
  __oGeoPosition.setPosition( __fdX, __fdY, getElevation( _rqPointFDatPosition ) );
  return __oGeoPosition;
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<quint16>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const quint16* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<qint16>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const qint16* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<quint32>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const quint32* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<qint32>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const qint32* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<float>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const float* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}

void CChartGDALElevation::rasterBuffer( QImage* _pqImage, const QVector<double>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const double* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = rasterValue( *__pqVectorBit++ );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

double CChartGDALElevation::getElevation( const QPointF& _rqPointFDatPosition ) const
{
  double __fdElevation = CDataPosition::UNDEFINED_ELEVATION;
  if( !qRectGeometry.contains( (int)( _rqPointFDatPosition.x()+0.5 ), (int)( _rqPointFDatPosition.y()+0.5 ) ) ) return __fdElevation;
  double __fdXint, __fdXdec, __fdYint, __fdYdec;
  __fdXdec = modf( _rqPointFDatPosition.x(), &__fdXint );
  __fdYdec = modf( _rqPointFDatPosition.y(), &__fdYint );
  switch( eColorEncoding )
  {

  case GRAY_U16:
    {
      QVector<quint16> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_UInt16,
                                                     sizeof(quint16), 2*sizeof(quint16) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  case GRAY_S16:
    {
      QVector<qint16> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_Int16,
                                                     sizeof(qint16), 2*sizeof(qint16) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  case GRAY_U32:
    {
      QVector<quint32> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_UInt32,
                                                     sizeof(quint32), 2*sizeof(quint32) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  case GRAY_S32:
    {
      QVector<qint32> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_Int32,
                                                     sizeof(qint32), 2*sizeof(qint32) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  case GRAY_F32:
    {
      QVector<float> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_Float32,
                                                     sizeof(float), 2*sizeof(float) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  case GRAY_F64:
    {
      QVector<double> __qVector( 4 );
      GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
      CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                     (int)__fdXint, (int)__fdYint,
                                                     2, 2,
                                                     __qVector.data(),
                                                     2, 2,
                                                     GDT_Float64,
                                                     sizeof(double), 2*sizeof(double) );
      if( tCPLErr != CE_None )
      {
        qCritical( "ERROR[%s]: Failed to rasterize elevation data", Q_FUNC_INFO );
        return __fdElevation;
      }
      __fdElevation = interpolateValue( __qVector[0], __qVector[1], __qVector[2], __qVector[3], __fdXdec, __fdYdec );
    }
    break;

  default:;

  }

  return __fdElevation;
}

double CChartGDALElevation::getElevation( const CDataPosition& _roGeoPosition ) const
{
  return getElevation( toDatPosition( _roGeoPosition ) );
}

uchar CChartGDALElevation::rasterValue( double _fdValue ) const
{
  _fdValue = ( _fdValue / 78.125 ) + 128.0;
  if( _fdValue > 255.0 ) _fdValue = 255.0;
  else if( _fdValue < 0.0 ) _fdValue = 0.0;
  return (uchar)(_fdValue+0.5);
}

double CChartGDALElevation::interpolateValue( double _fdP11, double _fdP21, double _fdP12, double _fdP22, double _fdX, double _fdY ) const
{
  return _fdP11 * ( 1.0 - _fdX ) * ( 1.0 - _fdY )
    + _fdP21 * _fdX * ( 1.0 - _fdY )
    + _fdP12 * ( 1.0 - _fdX ) * _fdY
    + _fdP22 * _fdX * _fdY;
}
