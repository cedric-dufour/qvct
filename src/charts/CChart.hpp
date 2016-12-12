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

#ifndef QVCT_CCHART_HPP
#define QVCT_CCHART_HPP

// QT
#include <QDomElement> // QtXml module
#include <QPrinter>
#include <QRectF>
#include <QWidget>
#include <QXmlStreamWriter>

// QVCT
#include "QVCT.hpp"
class CChartGDAL;
class CChartGDALRaster;
class CChartGDALElevation;
class CDataPosition;


/// [UI] Chart (view)
/**
 *  This class wraps a CChartGDAL object and allows it to be manipulated and
 *  displayed in the "virtual" chart table.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChart: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// GDAL chart (dataset)
  CChartGDALRaster* poChartGDALRaster;
  /// GDAL data elevation model (dataset)
  /** @see addElevation(), hasElevation() */
  CChartGDALElevation* poChartGDALElevation;
  /// Viewport draw area
  /** @see setDrawArea(), resetDrawArea(), getDrawArea() */
  QRectF qRectFDrawArea;
  /// Current (center) position in the GDAL dataset
  /** @see setGeoPosition(), getGeoPosition(), getDatPosition() */
  QPointF qPointFDatPosition;
  /// Position lock status
  /** @see lockPosition(), isPositionLocked() */
  bool bPositionLock;
  /// Current zoom factor
  /** @see setZoom(), getZoom() */
  double fdZoom;
  /// Zoom lock status
  /** @see lockZoom(), isZoomLocked() */
  bool bZoomLock;
  /// Elevation model show status
  /** @see showElevation(), isElevationShowed() */
  bool bShowElevation;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChart( const QString& _rqsFileName, QWidget* _pqParent = 0 );
  virtual ~CChart();

public:
  /// Add elevation model data to chart
  void addElevation( const QString& _rqsFileName );


  //------------------------------------------------------------------------------
  // METHODS: QWidget (override)
  //------------------------------------------------------------------------------

protected:
  virtual void paintEvent( QPaintEvent* _pqPaintEvent );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the viewport draw area
  void setDrawArea( QRectF _qRectFDrawArea ) { qRectFDrawArea = _qRectFDrawArea; };
  /// Resets the viewport draw area (to the widget's area)
  void resetDrawArea();
  /// Sets the current (center) position (geographical coordinates) of the chart
  void setGeoPosition( const CDataPosition& _roGeoPosition );
  /// Sets the position lock status
  void lockPosition( bool _bLock ) { bPositionLock = _bLock; };
  /// Sets the current zoom factor
  void setZoom( double _fdZoom ) { fdZoom = _fdZoom; };
  /// Sets the zoom lock status
  void lockZoom( bool _bLock ) { bZoomLock = _bLock; };
  /// Sets the elevation model show status
  void showElevation( bool _bShow );

  // GETTERS
public:
  /// Returns the internal status of the object
  /** NOTE: Anything but QVCT::OK means the object is not usable. */
  QVCT::EStatus getStatus() const;
  /// Returns the underlying chart (GDAL dataset) file name
  QString getFileName() const;
  /// Returns the viewport draw area
  QRectF getDrawArea() const { return qRectFDrawArea; };
  /// Returns the center position (geographical coordinates) of the chart
  CDataPosition getGeoPositionCenter() const;
  /// Returns the current (center) position (geographical coordinates) of the chart
  CDataPosition getGeoPosition() const;
  /// Returns the viewport draw area's center position
  QPointF getDrawPositionCenter() const;
  /// Returns the current (center) position (dataset coordinates) of the chart
  QPointF getDatPosition() const { return qPointFDatPosition; };
  /// Returns the position lock status
  bool isPositionLocked() const { return bPositionLock; };
  /// Returns the current zoom factor
  double getZoom() const { return fdZoom; };
  /// Returns the zoom factor corresponding to a 1:1 dataset/screen pixels ratio
  double getZoomActual() const { return 1.0; };
  /// Returns the zoom factor allowing to view the entire chart
  double getZoomFit() const;
  /// Returns the zoom factor allowing to view the given geographical area (defined by its opposite corners)
  double getZoomArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2 ) const;
  /// Returns the zoom lock status
  bool isZoomLocked() const { return bZoomLock; };
  /// Returns whether this chart has been associated elevation model data
  bool hasElevation() const { return poChartGDALElevation; };
  /// Returns the elevation model show status
  bool isElevationShowed() const { return bShowElevation; };
  /// Returns the resolution of the chart at its current position, in meters per pixel [m/px]
  double getResolution() const;

  // OTHER
public:
  /// Converts the given chart draw position to geographical position
  CDataPosition toGeoPosition( const QPointF& _rqPointFDrawPosition ) const;
  /// Converts the given (geographical) position [long,lat,elev] to chart draw point [px]
  QPointF toDrawPosition( const CDataPosition& _roGeoPosition ) const;
  /// Move the current (center) chart position by specified offset (in the draw area)
  void move( const QPointF& _rqPointFDrawPositionOffset );

protected:
  /// Draws the chart (on screen)
  void draw();

public:
  /// Prints the chart
  void print( QPrinter* _pqPrinter );

public:
  /// Retrieves this object's content from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;

};

#endif // QVCT_CCHART_HPP
