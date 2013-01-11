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
#include <cstring>

// QT
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>

// GDAL
#include <cpl_string.h>
#include <gdal.h>
#include <gdal_alg.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>

// QVCT
#include "data/CDataPosition.hpp"
#include "charts/CChartGDAL.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChartGDAL::CChartGDAL()
  : eStatus( QVCT::UNDEFINED )
  , poGDALDataset( NULL )
  , pGDALProjectionTransformer( NULL )
  , piGDALBandMap( NULL )
{}

CChartGDAL::~CChartGDAL()
{
  if( poGDALDataset ) GDALClose( poGDALDataset );
  if( pGDALProjectionTransformer ) GDALDestroyGenImgProjTransformer( pGDALProjectionTransformer );
  if( piGDALBandMap ) delete piGDALBandMap;
}

void CChartGDAL::open( const QString& _rqsFileName )
{
  eStatus = QVCT::ERROR;

  // Save file name
  qsFileName = _rqsFileName;

  // Open chart as GDAL dataset
  GDALAllRegister();
  poGDALDataset = (GDALDataset*)GDALOpen( _rqsFileName.toLocal8Bit().constData(), GA_ReadOnly );
  if( !poGDALDataset )
  {
    qCritical( "ERROR[%s]: Failed to open dataset (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    return;
  }

  // Retrieve/save dataset geometry
  qRectGeometry.setCoords( 0, 0, poGDALDataset->GetRasterXSize()-1, poGDALDataset->GetRasterYSize()-1 );

  // Create the GDAL projection transformer corresponding to the opened dataset
  OGRSpatialReference __oOGRSpatialReference;
  char* __pcProjectionDefinitionString;
  if( __oOGRSpatialReference.importFromProj4( "+proj=longlat +ellps=WGS84 +datum=WGS84" ) != OGRERR_NONE
      || __oOGRSpatialReference.exportToWkt( &__pcProjectionDefinitionString ) != OGRERR_NONE )
  {
    qCritical( "ERROR[%s]: Failed to initialize projection system", Q_FUNC_INFO );
    return;
  }
  char **__ppcGDALProjectionOptions = CSLSetNameValue( NULL, "DST_SRS", __pcProjectionDefinitionString );
  CPLFree( __pcProjectionDefinitionString );
  pGDALProjectionTransformer = GDALCreateGenImgProjTransformer2( poGDALDataset, NULL, __ppcGDALProjectionOptions );
  CSLDestroy( __ppcGDALProjectionOptions );
  if( !pGDALProjectionTransformer )
  {
    qCritical( "ERROR[%s]: Failed to create projection transformer for dataset (%s)\n[...]: Use 'gdalinfo' to check your dataset projection data", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    return;
  }

  // Validate dataset central (geographical) position
  double __fdX = (double)qRectGeometry.center().x(), __fdY = (double)qRectGeometry.center().y(), __fdZ = 0;
  int __iSuccess = TRUE;
  GDALGenImgProjTransform( pGDALProjectionTransformer, false, 1, &__fdX, &__fdY, &__fdZ, &__iSuccess );
  if( __iSuccess != TRUE )
  {
    qCritical( "ERROR[%s]: Failed to perform dataset projection transformation (%s)\n[...]: Use 'gdalinfo' to check your dataset projection data", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    return;
  }
  if( __fdX < -180 || __fdX > 180 || __fdY < -90 || __fdY > 90 )
  {
    qCritical( "ERROR[%s]: Dataset projection transformation leads to invalid results (%s)\n[...]: Use 'gdalinfo' to check your dataset projection data", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    return;
  }

  // Retrieve dataset color (encoding) type
  bool __bHasGDALBandA = false, __bHasGDALBandR = false, __bHasGDALBandG = false, __bHasGDALBandB = false;
  bool __bHasGDALBandY = false, __bHasGDALBandCb = false, __bHasGDALBandCr = false;
  GDALRasterBand* __poGDALRasterBand = NULL;
  GDALColorTable* __poGDALColorTable = NULL;
  int __iGDALBandCount = poGDALDataset->GetRasterCount();
  switch( __iGDALBandCount )
  {

  case 0: // switch( __iGDALBandCount )
    qCritical( "ERROR[%s]: Dataset has no raster band (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    return;

  case 1: // switch( __iGDALBandCount )
    __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
    if( !__poGDALRasterBand )
    {
      qCritical( "ERROR[%s]: Failed to retrieve raster band (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
      return;
    }
    switch( __poGDALRasterBand->GetColorInterpretation() )
    {

    case GCI_GrayIndex: // switch( __poGDALRasterBand->GetColorInterpretation() )
      switch( __poGDALRasterBand->GetRasterDataType() )
      {
      case GDT_Byte: eColorEncoding = GRAY; break;
      case GDT_UInt16: eColorEncoding = GRAY_U16; break;
      case GDT_Int16: eColorEncoding = GRAY_S16; break;
      case GDT_UInt32: eColorEncoding = GRAY_U32; break;
      case GDT_Int32: eColorEncoding = GRAY_S32; break;
      case GDT_Float32: eColorEncoding = GRAY_F32; break;
      case GDT_Float64: eColorEncoding = GRAY_F64; break;
      default:
        qCritical( "ERROR[%s]: Unsupported data type (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
        return;
      }
      break;

    case GCI_PaletteIndex: // switch( __poGDALRasterBand->GetColorInterpretation() )
      __poGDALColorTable = __poGDALRasterBand->GetColorTable();
      if( !__poGDALColorTable )
      {
        qCritical( "ERROR[%s]: Failed to retrieve color table (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
        return;
      }
      switch( __poGDALColorTable->GetPaletteInterpretation() )
      {
      case GPI_Gray: eColorEncoding = PALETTE_GRAY; break;
      case GPI_RGB: eColorEncoding = PALETTE_RGB; break;
      case GPI_CMYK: eColorEncoding = PALETTE_CMYK; break;
      case GPI_HLS: eColorEncoding = PALETTE_HSL; break;
      default:
        qCritical( "ERROR[%s]: Unsupported palette interpretation for single-band dataset (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
        return;
      }
      break;

    default: // switch( __poGDALRasterBand->GetColorInterpretation() )
      qCritical( "ERROR[%s]: Unsupported color interpretation for single-band dataset (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
      return;

    } // switch( __poGDALRasterBand->GetColorInterpretation() )
    break;

  default: // switch( __iGDALBandCount )
    for( int __iGDALBandIndex = 1; __iGDALBandIndex <= __iGDALBandCount; __iGDALBandIndex++ )
    {
      __poGDALRasterBand = poGDALDataset->GetRasterBand( __iGDALBandIndex );
      if( !__poGDALRasterBand )
      {
        qCritical( "ERROR[%s]: Failed to retrieve raster band (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
        return;
      }
      switch( __poGDALRasterBand->GetColorInterpretation() )
      {
      case GCI_AlphaBand: __bHasGDALBandA = true; break;
      case GCI_RedBand: __bHasGDALBandR = true; break;
      case GCI_GreenBand: __bHasGDALBandG = true; break;
      case GCI_BlueBand: __bHasGDALBandB = true; break;
      case GCI_YCbCr_YBand: __bHasGDALBandY = true; break;
      case GCI_YCbCr_CbBand: __bHasGDALBandCb = true; break;
      case GCI_YCbCr_CrBand: __bHasGDALBandCr = true; break;
      default:; // other bands are not used
      }
    }
    if( __bHasGDALBandR && __bHasGDALBandG && __bHasGDALBandB )
    {
      eColorEncoding = __bHasGDALBandA ? ARGB : RGB;
    }
    else if( __bHasGDALBandY && __bHasGDALBandCb && __bHasGDALBandCr )
    {
      // NOTE: we should never get here since GDAL converts YCbCr color space to RGB intrinsecally
      eColorEncoding = YUV;
      qCritical( "ERROR[%s]: YCbCr color interpretation not implemented (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
      return;
    }
    else
    {
      qCritical( "ERROR[%s]: Unsupported color interpretations for multi-band dataset (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
      return;
    }

  } // switch( __iGDALBandCount )

  // Initialize color resources
  const GDALColorEntry* __poGDALColorEntry;
  qColorTable.clear();
  piGDALBandMap = new int[4];
  for( int i=0; i<4; i++ ) piGDALBandMap[i] = -1;
  switch( eColorEncoding )
  {

  case GRAY: // switch( eColorEncoding )
  case GRAY_U16:
  case GRAY_S16:
  case GRAY_U32:
  case GRAY_S32:
  case GRAY_F32:
  case GRAY_F64:
    for( int __iColorComponent = 0; __iColorComponent < 256; __iColorComponent++ )
    {
      qColorTable.append( qRgba( __iColorComponent, __iColorComponent, __iColorComponent, 255 ) );
    }
    break;

  case PALETTE_GRAY: // switch( eColorEncoding )
    for( int __iPaletteIndex = 0; __iPaletteIndex < __poGDALColorTable->GetColorEntryCount(); __iPaletteIndex++ )
    {
        __poGDALColorEntry = __poGDALColorTable->GetColorEntry( __iPaletteIndex );
        if( !__poGDALColorEntry )
        {
          qCritical( "ERROR[%s]: Failed to retrieve color table entry (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
          return;
        }
        qColorTable.append( qRgb( __poGDALColorEntry->c1, __poGDALColorEntry->c1, __poGDALColorEntry->c1 ) );
    }
    break;

  case PALETTE_RGB: // switch( eColorEncoding )
    for( int __iPaletteIndex = 0; __iPaletteIndex < __poGDALColorTable->GetColorEntryCount(); __iPaletteIndex++ )
    {
        __poGDALColorEntry = __poGDALColorTable->GetColorEntry( __iPaletteIndex );
        if( !__poGDALColorEntry )
        {
          qCritical( "ERROR[%s]: Failed to retrieve color table entry (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
          return;
        }
        qColorTable.append( qRgba( __poGDALColorEntry->c1, __poGDALColorEntry->c2, __poGDALColorEntry->c3, __poGDALColorEntry->c4 ) );
    }
    break;

  case PALETTE_CMYK: // switch( eColorEncoding )
    for( int __iPaletteIndex = 0; __iPaletteIndex < __poGDALColorTable->GetColorEntryCount(); __iPaletteIndex++ )
    {
        __poGDALColorEntry = __poGDALColorTable->GetColorEntry( __iPaletteIndex );
        if( !__poGDALColorEntry )
        {
          qCritical( "ERROR[%s]: Failed to retrieve color table entry (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
          return;
        }
        qColorTable.append( QColor::fromCmyk( __poGDALColorEntry->c1, __poGDALColorEntry->c2, __poGDALColorEntry->c3, __poGDALColorEntry->c4 ).rgb() );
    }
    break;

  case PALETTE_HSL: // switch( eColorEncoding )
    for( int __iPaletteIndex = 0; __iPaletteIndex < __poGDALColorTable->GetColorEntryCount(); __iPaletteIndex++ )
    {
        __poGDALColorEntry = __poGDALColorTable->GetColorEntry( __iPaletteIndex );
        if( !__poGDALColorEntry )
        {
          qCritical( "ERROR[%s]: Failed to retrieve color table entry (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
          return;
        }
        qColorTable.append( QColor::fromHsl( __poGDALColorEntry->c1, __poGDALColorEntry->c3, __poGDALColorEntry->c2 ).rgb() ); // WARNING: GDAL uses HLS colortable; QT uses HSL (S and L are swapped)
    }
    break;

  case ARGB: // switch( eColorEncoding )
    {
      QRgb __qRgbPixel = qRgba( GCI_RedBand, GCI_GreenBand, GCI_BlueBand, GCI_AlphaBand );
      for( int __iGDALBandIndex = 1; __iGDALBandIndex <= __iGDALBandCount; __iGDALBandIndex++ )
      {
        GDALColorInterp __eGDALColorInterp = poGDALDataset->GetRasterBand( __iGDALBandIndex )->GetColorInterpretation();
        switch( __eGDALColorInterp )
        {
        case GCI_AlphaBand:
        case GCI_RedBand:
        case GCI_GreenBand:
        case GCI_BlueBand:
          for( int __iGDALBandOffset = 0; __iGDALBandOffset < 4 ; __iGDALBandOffset++ )
            if( __eGDALColorInterp == *(((quint8 *)&__qRgbPixel)+__iGDALBandOffset) )
              piGDALBandMap[ __iGDALBandOffset ] = __iGDALBandIndex;
          break;
        default:; // other bands are not used
        }
      }
    }
    break;

  case RGB: // switch( eColorEncoding )
    {
      QRgb __qRgbPixel = qRgba( GCI_RedBand, GCI_GreenBand, GCI_BlueBand, GCI_AlphaBand );
      for( int __iGDALBandIndex = 1; __iGDALBandIndex <= __iGDALBandCount; __iGDALBandIndex++ )
      {
        GDALColorInterp __eGDALColorInterp = poGDALDataset->GetRasterBand( __iGDALBandIndex )->GetColorInterpretation();
        switch( __eGDALColorInterp )
        {
        case GCI_RedBand:
        case GCI_GreenBand:
        case GCI_BlueBand:
          for( int __iGDALBandOffset = 0; __iGDALBandOffset < 4 ; __iGDALBandOffset++ )
            if( __eGDALColorInterp == *(((quint8 *)&__qRgbPixel)+__iGDALBandOffset) )
              piGDALBandMap[ __iGDALBandOffset ] = __iGDALBandIndex;
          break;
        default:; // other bands are not used
        }
      }
    }
    break;

  case YUV: // switch( eColorEncoding )
    // NOTE: we should never get here since GDAL converts YCbCr color space to RGB intrinsecally
    qCritical( "ERROR[%s]: YCbCr color interpretation not implemented (%s)", Q_FUNC_INFO, qPrintable( _rqsFileName ) );
    break;

  default:; // other color types do not require a color table or band mapping

  } // switch( eColorEncoding )

  eStatus = QVCT::OK;
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

CDataPosition CChartGDAL::toGeoPosition( const QPointF& _rqPointFDatPosition ) const
{
  CDataPosition __oGeoPosition;
  if( eStatus != QVCT::OK ) return __oGeoPosition;
  double __fdX = (double)_rqPointFDatPosition.x(), __fdY = (double)_rqPointFDatPosition.y(), __fdZ = 0;
  int __iSuccess = TRUE;
  GDALGenImgProjTransform( pGDALProjectionTransformer, false, 1, &__fdX, &__fdY, &__fdZ, &__iSuccess );
  if( __iSuccess != TRUE ) return __oGeoPosition;
  __oGeoPosition.setPosition( __fdX, __fdY, CDataPosition::UNDEFINED_ELEVATION );
  return __oGeoPosition;
}

QPointF CChartGDAL::toDatPosition( const CDataPosition& _roGeoPosition ) const
{
  QPointF __qPointFDatPosition;
  if( eStatus != QVCT::OK ) return __qPointFDatPosition;
  if( _roGeoPosition == CDataPosition::UNDEFINED ) return __qPointFDatPosition;
  double __fdX = (double)_roGeoPosition.getLongitude(), __fdY = (double)_roGeoPosition.getLatitude(), __fdZ = _roGeoPosition.getElevation();
  if( __fdZ == CDataPosition::UNDEFINED_ELEVATION ) __fdZ = 0;
  int __iSuccess = TRUE;
  GDALGenImgProjTransform( pGDALProjectionTransformer, true, 1, &__fdX, &__fdY, &__fdZ, &__iSuccess );
  if( __iSuccess != TRUE ) return __qPointFDatPosition;
  __qPointFDatPosition.setX( __fdX );
  __qPointFDatPosition.setY( __fdY );
  return __qPointFDatPosition;
}

double CChartGDAL::getResolution( const QPointF& _rqPointFDatPosition ) const
{
  QPointF __qPointFDatPositionNorth( _rqPointFDatPosition.x(), _rqPointFDatPosition.y() - 50.0 );
  QPointF __qPointFDatPositionSouth( _rqPointFDatPosition.x(), _rqPointFDatPosition.y() + 50.0 );
  CDataPosition __oGeoPositionNorth = toGeoPosition( __qPointFDatPositionNorth );
  CDataPosition __oGeoPositionSouth = toGeoPosition( __qPointFDatPositionSouth );
  return( ( __oGeoPositionNorth.getLatitude() - __oGeoPositionSouth.getLatitude() ) * 1111.2 ); // delta(Latitude) * 60nm * 1852m/nm / 100px [m/px]
}

void CChartGDAL::draw( QPainter* _pqPainter, const QPointF& _rqPointFDatPosition, double _fdZoom )
{
  // Compute dataset region to rasterize
  // ... fix silly zoom factor
  if( _fdZoom <= 0 ) _fdZoom = 1;
  // ... "wished" pixmap area based on the painter's device size
  QSize __qSizePixmapWished( _pqPainter->device()->width(), _pqPainter->device()->height() );

  // Redraw pixmap only if required
  if( qPixmapBuffer.size() != __qSizePixmapWished || qPointFDatPositionWished != _rqPointFDatPosition || fdZoomWished != _fdZoom )
  {
    //qDebug( "DEBUG[%s] Rendering", Q_FUNC_INFO );

    // Reset pixmap buffer
    if( qPixmapBuffer.size() != __qSizePixmapWished ) qPixmapBuffer = QPixmap( __qSizePixmapWished );
    qPixmapBuffer.fill( Qt::black );
    qPointFDatPositionWished = _rqPointFDatPosition;
    fdZoomWished = _fdZoom;
    //qDebug( "DEBUG[%s] qPointFDatPositionWished: %f, %f", Q_FUNC_INFO, qPointFDatPositionWished.x(), qPointFDatPositionWished.y() );
    //qDebug( "DEBUG[%s] fdZoomWidhed: %f", Q_FUNC_INFO, fdZoomWished );

    // Compute dataset region to rasterize (cont'd)
    // ... "wished" dataset area based on center position and zoom factor
    QSize __qSizeGDALWished( (double)__qSizePixmapWished.width() / _fdZoom + 0.5, (double)__qSizePixmapWished.height() / _fdZoom + 0.5 );
    //qDebug( "DEBUG[%s] __qSizeGDALWished: %d, %d", Q_FUNC_INFO, __qSizeGDALWished.width(), __qSizeGDALWished.height() );
    QPoint __qPointDatPositionGDALWished( qPointFDatPositionWished.x() - (double)__qSizeGDALWished.width()/2.0 + 0.5, qPointFDatPositionWished.y() - (double)__qSizeGDALWished.height()/2.0 + 0.5 );
    //qDebug( "DEBUG[%s] __qPointDatPositionGDALWished: %d, %d", Q_FUNC_INFO, __qPointDatPositionGDALWished.x(), __qPointDatPositionGDALWished.y() );
    // ... "actual" dataset area by intersecting with the dataset "existing" area
    QRect __qRectGDALActual( __qPointDatPositionGDALWished.x(), __qPointDatPositionGDALWished.y(), __qSizeGDALWished.width(), __qSizeGDALWished.height() );
    __qRectGDALActual = __qRectGDALActual.intersect( qRectGeometry );
    // ... "actual" pixmap area (mirroring the correction made on the dataset area)
    QSize __qSizePixmapActual( (double)__qRectGDALActual.width() * _fdZoom + 0.5, (double)__qRectGDALActual.height() * _fdZoom + 0.5 );
    //qDebug( "DEBUG[%s] __qSizePixmapActual: %d, %d", Q_FUNC_INFO, __qSizePixmapActual.width(), __qSizePixmapActual.height() );
    // ... "actual" pixmap position
    QPoint __qPointPixmapActual( (double)( __qRectGDALActual.topLeft().x() - __qPointDatPositionGDALWished.x() ) * _fdZoom + 0.5, (double)( __qRectGDALActual.topLeft().y() - __qPointDatPositionGDALWished.y() ) * _fdZoom + 0.5 );
    //qDebug( "DEBUG[%s] __qPointPixmapActual: %d, %d", Q_FUNC_INFO, __qPointPixmapActual.x(), __qPointPixmapActual.y() );
    // ... "actual" dataset position
    //qPointFDatPositionActual.setX( ( (double)__qPointPixmapActual.x() * _fdZoom ) + (double)__qRectGDALActual.topLeft().x() + ( (double)__qSizePixmapWished.width() / _fdZoom )/2.0 );
    //qPointFDatPositionActual.setY( ( (double)__qPointPixmapActual.y() * _fdZoom ) + (double)__qRectGDALActual.topLeft().y() + ( (double)__qSizePixmapWished.height() / _fdZoom )/2.0 );
    qPointFDatPositionActual.setX( (double)__qRectGDALActual.topLeft().x() + ( (double)__qSizePixmapWished.width() / _fdZoom )/2.0 - ( (double)__qPointPixmapActual.x() / _fdZoom ) );
    qPointFDatPositionActual.setY( (double)__qRectGDALActual.topLeft().y() + ( (double)__qSizePixmapWished.height() / _fdZoom )/2.0 - ( (double)__qPointPixmapActual.y() / _fdZoom ) );
    //qDebug( "DEBUG[%s] qPointFDatPositionActual: %f, %f", Q_FUNC_INFO, qPointFDatPositionActual.x(), qPointFDatPositionActual.y() );
    // ... "actual" dataset zoom
    fdZoomActual = _fdZoom;
    //qDebug( "DEBUG[%s] fdZoomActual: %f", Q_FUNC_INFO, fdZoomActual );
    // ... "actual" image size (corrected to match the QImage-required mod-4 pixel width)
    QSize __qSizeImageActual( ( __qSizePixmapActual.width() + 3 ) & 0xFFFFFFFC,  __qSizePixmapActual.height() );
    //qDebug( "DEBUG[%s] __qSizeImageActual: %d, %d", Q_FUNC_INFO, __qSizeImageActual.width(), __qSizeImageActual.height() );

    // GDAL::RasterIO
    if( __qSizePixmapActual.isValid() )
    {
      QPainter __qPainter( &qPixmapBuffer );
      switch( eColorEncoding )
      {

      case GRAY_U16: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<quint16> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_UInt16,
                                                         sizeof(quint16), sizeof(quint16)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY_S16: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<qint16> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_Int16,
                                                         sizeof(qint16), sizeof(qint16)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY_U32: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<quint32> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_UInt32,
                                                         sizeof(quint32), sizeof(quint32)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY_S32: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<qint32> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_Int32,
                                                         sizeof(qint32), sizeof(qint32)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY_F32: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<float> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_Float32,
                                                         sizeof(float), sizeof(float)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY_F64: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize to through data-type buffer
          QVector<double> __qVector( __qImage.width()*__qImage.height() );
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qVector.data(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_Float64,
                                                         sizeof(double), sizeof(double)*__qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize typed dataset", Q_FUNC_INFO );
            return;
          }
          rasterBuffer( &__qImage, __qVector );
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case GRAY: // switch( eColorEncoding )
      case PALETTE_GRAY:
      case PALETTE_RGB:
      case PALETTE_CMYK:
      case PALETTE_HSL:
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_Indexed8 );
          __qImage.setColorTable( qColorTable );
          // Rasterize directly to QImage bitmap
          GDALRasterBand* __poGDALRasterBand = poGDALDataset->GetRasterBand( 1 );
          CPLErr tCPLErr = __poGDALRasterBand->RasterIO( GF_Read,
                                                         __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                         __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                         __qImage.bits(),
                                                         __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                         GDT_Byte,
                                                         1, __qImage.width() );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize palette dataset", Q_FUNC_INFO );
            return;
          }
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case ARGB: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_ARGB32 );
          // Rasterize directly to QImage bitmap
          CPLErr tCPLErr = poGDALDataset->RasterIO( GF_Read,
                                                    __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                    __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                    __qImage.bits(),
                                                    __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                    GDT_Byte,
                                                    4, piGDALBandMap, 4, 4*__qImage.width(), 1 );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize ARGB dataset", Q_FUNC_INFO );
            return;
          }
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case RGB: // switch( eColorEncoding )
        {
          QImage __qImage( __qSizeImageActual, QImage::Format_RGB32 );
          // Rasterize directly to QImage bitmap
          CPLErr tCPLErr = poGDALDataset->RasterIO( GF_Read,
                                                    __qRectGDALActual.topLeft().x(), __qRectGDALActual.topLeft().y(),
                                                    __qRectGDALActual.width(), __qRectGDALActual.height(),
                                                    __qImage.bits()+( piGDALBandMap[0]<0 ? 1 : 0 ),
                                                    __qSizePixmapActual.width(), __qSizePixmapActual.height(),
                                                    GDT_Byte,
                                                    3, piGDALBandMap, 4, 4*__qImage.width(), 1 );
          if( tCPLErr != CE_None )
          {
            qCritical( "ERROR[%s]: Failed to rasterize ARGB dataset", Q_FUNC_INFO );
            return;
          }
          // Draw image
          __qPainter.drawImage( __qPointPixmapActual, __qImage );
        }
        break;

      case YUV: // switch( eColorEncoding )
        // NOTE: we should never get here since GDAL converts YCbCr color space to RGB intrinsecally
        qCritical( "ERROR[%s]: YCbCr color interpretation not implemented", Q_FUNC_INFO );
        return;
        break;

      } // switch( eColorEncoding )

    } // if( __qSizePixmapActual.isValid() )

  } // if( qPixmapBuffer.size() != __qSizePixmapWished || qPointFDatPositionPixmap != _rqPointFDatPosition || fdZoomPixmap != _fdZoom )

  _pqPainter->drawPixmap( 0, 0, qPixmapBuffer );

}

void CChartGDAL::move( const QPointF& _rqPointFDatPosition, double _fdZoom )
{
  qPixmapBuffer = QPixmap(); // re-rendering shall be required
  qPointFDatPositionActual = _rqPointFDatPosition;
  fdZoomActual = _fdZoom;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<quint16>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const quint16* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = *__pqVectorBit++ >> 8;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<qint16>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const qint16* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = ( *__pqVectorBit++ >> 8 ) + 127;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<quint32>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const quint32* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = *__pqVectorBit++ >> 24;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<qint32>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const qint32* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = ( *__pqVectorBit++ >> 24 ) + 127;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<float>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const float* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = *__pqVectorBit++;
}

void CChartGDAL::rasterBuffer( QImage* _pqImage, const QVector<double>& _rqVector ) const
{
  uchar* __pqImageBit = _pqImage->bits();
  const double* __pqVectorBit = _rqVector.data();
  for( int x=_pqImage->width()-1; x>=0; x-- )
    for( int y=_pqImage->height()-1; y>=0; y-- )
      *__pqImageBit++ = *__pqVectorBit++;
}
