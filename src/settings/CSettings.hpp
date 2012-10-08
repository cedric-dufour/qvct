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

#ifndef QVCT_CSETTINGS_HPP
#define QVCT_CSETTINGS_HPP

// QT
#include <QColor>
#include <QDomDocument> // QtXml module
#include <QObject>
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "units/CUnitBearing.hpp"
#include "units/CUnitDate.hpp"
#include "units/CUnitDistance.hpp"
#include "units/CUnitElevation.hpp"
#include "units/CUnitPosition.hpp"
#include "units/CUnitSpeed.hpp"
#include "units/CUnitSpeedVertical.hpp"
#include "units/CUnitTime.hpp"
#include "units/CUnitTimeDelta.hpp"
#include "units/CUnitTimeZone.hpp"

/// [UI] Container for the application's settings
/**
 *  This class gathers all the parameters that allows the user to modify
 *  the application's behavior.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
//  NOTE: I'd rather use a dedicated class and appropriate fields/methods,
//        along with interoperable, well-structured and hand-editable
//        XML persistency, than rely on QSettings.
class CSettings: public QObject
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [Path] Current working directory
  /** @see slotPathWorkingDirectory(), setPathWorkingDirectory(), getPathWorkingDirectory() */
  QString qsPathWorkingDirectory;
  /// [Path] Symbols directory
  /** @see slotPathSymbolsDirectory(), getPathSymbolsDirectory() */
  //  NOTE: Symbols are images used for chart points illustration
  QString qsPathSymbolsDirectory;

  /// [Unit] Date format/unit
  /** @see slotUnitDate(), getUnitDate() */
  CUnitDate::EUnit eUnitDate;
  /// [Unit] Time format/unit
  /** @see slotUnitTime(), getUnitTime() */
  CUnitTime::EUnit eUnitTime;
  /// [Unit] Time difference format/unit
  /** @see slotUnitTimeDelta(), getUnitTimeDelta() */
  CUnitTimeDelta::EUnit eUnitTimeDelta;
  /// [Unit] timezone format/unit
  /** @see slotUnitTimeZone(), getUnitTimeZone() */
  CUnitTimeZone::EUnit eUnitTimeZone;
  /// [Unit] Position format/unit
  /** @see slotUnitPosition(), getUnitPosition() */
  CUnitPosition::EUnit eUnitPosition;
  /// [Unit] Bearing format/unit
  /** @see slotUnitBearing(), getUnitBearing() */
  CUnitBearing::EUnit eUnitBearing;
  /// [Unit] Distance format/unit
  /** @see slotUnitDistance(), getUnitDistance() */
  CUnitDistance::EUnit eUnitDistance;
  /// [Unit] Speed format/unit
  /** @see slotUnitSpeed(), getUnitSpeed() */
  CUnitSpeed::EUnit eUnitSpeed;
  /// [Unit] Elevation format/unit
  /** @see slotUnitElevation(), getUnitElevation() */
  CUnitElevation::EUnit eUnitElevation;
  /// [Unit] Vertical speed format/unit
  /** @see slotUnitSpeedVertical(), getUnitSpeedVertical() */
  CUnitSpeedVertical::EUnit eUnitSpeedVertical;

  /// [Precision] Time decimal precision
  /** @see slotPrecisionTime(), getPrecisionTime() */
  int iPrecisionTime;
  /// [Precision] Time difference decimal precision
  /** @see slotPrecisionTimeDelta(), getPrecisionTimeDelta() */
  int iPrecisionTimeDelta;
  /// [Precision] Position decimal precision
  /** @see slotPrecisionPosition(), getPrecisionPosition() */
  int iPrecisionPosition;
  /// [Precision] Bearing decimal precision
  /** @see slotPrecisionBearing(), getPrecisionBearing() */
  int iPrecisionBearing;
  /// [Precision] Distance decimal precision
  /** @see slotPrecisionDistance(), getPrecisionDistance() */
  int iPrecisionDistance;
  /// [Precision] Speed decimal precision
  /** @see slotPrecisionSpeed(), getPrecisionSpeed() */
  int iPrecisionSpeed;
  /// [Precision] Elevation decimal precision
  /** @see slotPrecisionElevation(), getPrecisionElevation() */
  int iPrecisionElevation;
  /// [Precision] Vertical speed decimal precision
  /** @see slotPrecisionSpeedVertical(), getPrecisionSpeedVertical() */
  int iPrecisionSpeedVertical;

  /// [MinValue] Minimum (absolute) position (delta), in meters
  double fdMinValuePosition;
  /// [MinValue] Minimum (absolute) bearing (delta), in degrees
  double fdMinValueBearing;
  /// [MinValue] Minimum (absolute) horizontal speed, in meters per second
  double fdMinValueSpeed;
  /// [MinValue] Minimum (absolute) vertical speed, in meters per second
  double fdMinValueSpeedVertical;

  /// [MaxError] Position maximum error, in meters
  double fdMaxErrorPosition;
  /// [MaxError] Elevation maximum error, in meters
  double fdMaxErrorElevation;
  /// [MaxError] Time maximum error, in seconds
  double fdMaxErrorTime;
  /// [MaxError] Bearing maximum error, in degrees
  double fdMaxErrorBearing;
  /// [MaxError] Horizontal speed maximum error, in meters per second
  double fdMaxErrorSpeed;
  /// [MaxError] Vertical speed maximum error, in meters per second
  double fdMaxErrorSpeedVertical;

  /// [MaxAge] Position maximum age, in seconds
  double fdMaxAgePosition;
  /// [MaxAge] Elevation maximum age, in seconds
  double fdMaxAgeElevation;
  /// [MaxAge] Time maximum age, in seconds
  double fdMaxAgeTime;
  /// [MaxAge] Bearing maximum age, in seconds
  double fdMaxAgeBearing;
  /// [MaxAge] Horizontal speed maximum age, in seconds
  double fdMaxAgeSpeed;
  /// [MaxAge] Vertical speed maximum age, in seconds
  double fdMaxAgeSpeedVertical;

  /// [Color] Pointer overlay's base color
  /** @see slotColorPointer(), getColorPointer() */
  QColor qColorPointer;
  /// [Color] Landmark overlay's base color
  /** @see slotColorLandmark(), getColorLandmark() */
  QColor qColorLandmark;
  /// [Color] Route overlay's base color
  /** @see slotColorRoute(), getColorRoute() */
  QColor qColorRoute;
  /// [Color] Track overlay's base color
  /** @see slotColorTrack(), getColorTrack() */
  QColor qColorTrack;
  /// [Color] Vessel overlay's base color
  /** @see slotColorVessel(), getColorVessel() */
  QColor qColorVessel;

  /// [Misc] Main window geometry (Base64-encoded binary data)
  /** @see setMainWindowGeometry(), getMainWindowGeometry() */
  QString qsMainWindowGeometry;
  /// [Misc] Main window state (Base64-encoded binary data)
  /** @see setMainWindowState(), getMainWindowState() */
  QString qsMainWindowState;

  /// [Misc] Screen Dots-per-Inch (DPI)
  /** @see slotScreenDpi(), getScreenDpi() */
  int iScreenDpi;
  /// [Misc] Screen (multitouch) gestures (activation status)
  /** @see slotScreenGestures(), isScreenGestures() */
  bool bScreenGestures;
  /// [Misc] Content refresh rate, in milliseconds
  /** @see slotRateRefresh(), getRateRefresh() */
  int iRateRefresh;
  /// [Misc] Content redraw rate, in seconds
  /** @see slotRateRedraw(), getRateRedraw() */
  int iRateRedraw;
  /// [Misc] Chart opacity [0,100]
  /** @see slotChartOpacity(), getChartOpacity() */
  int iChartOpacity;
  /// [Misc] Symbols visibility (on chart)
  /** @see slotVisibleSymbols(), isVisibleSymbols() */
  bool bVisibleSymbols;
  /// [Misc] High-resolution printing
  /** @see slotPrintHighRes(), isPrintHighRes() */
  bool bPrintHighRes;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CSettings();
  virtual ~CSettings() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  //
  // SLOTS
  //
private slots:
  /// [Path] Slot to modify the current working directory
  void slotPathWorkingDirectory( const QString& _rqsPathWorkingDirectory );
  /// [Path] Slot to modify the symbols directory
  void slotPathSymbolsDirectory( const QString& _rqsPathSymbolsDirectory );

  /// [Unit] Slot to modify the date format/unit
  void slotUnitDate( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the time format/unit
  void slotUnitTime( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the time difference format/unit
  void slotUnitTimeDelta( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the timezone format/unit
  void slotUnitTimeZone( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the position format/unit
  void slotUnitPosition( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the bearing format/unit
  void slotUnitBearing( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the distance format/unit
  void slotUnitDistance( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the speed format/unit
  void slotUnitSpeed( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the elevation format/unit
  void slotUnitElevation( const QString& _rqsSymbol );
  /// [Unit] Slot to modify the vertical speed format/unit
  void slotUnitSpeedVertical( const QString& _rqsSymbol );

  /// [Precision] Slot to modify the time decimal precision
  void slotPrecisionTime( int _iPrecision ) { iPrecisionTime = _iPrecision; };
  /// [Precision] Slot to modify the time difference decimal precision
  void slotPrecisionTimeDelta( int _iPrecision ) { iPrecisionTimeDelta = _iPrecision; };
  /// [Precision] Slot to modify the position decimal precision
  void slotPrecisionPosition( int _iPrecision ) { iPrecisionPosition = _iPrecision; };
  /// [Precision] Slot to modify the bearing decimal precision
  void slotPrecisionBearing( int _iPrecision ) { iPrecisionBearing = _iPrecision; };
  /// [Precision] Slot to modify the distance decimal precision
  void slotPrecisionDistance( int _iPrecision ) { iPrecisionDistance = _iPrecision; };
  /// [Precision] Slot to modify the speed decimal precision
  void slotPrecisionSpeed( int _iPrecision ) { iPrecisionSpeed = _iPrecision; };
  /// [Precision] Slot to modify the elevation decimal precision
  void slotPrecisionElevation( int _iPrecision ) { iPrecisionElevation = _iPrecision; };
  /// [Precision] Slot to modify the vertical speed decimal precision
  void slotPrecisionSpeedVertical( int _iPrecision ) { iPrecisionSpeedVertical = _iPrecision; };

  /// [MinValue] Slot to modify the minimum (absolute) position (delta)
  void slotMinValuePosition( const QString& _rqsMinValue );
  /// [MinValue] Slot to modify the minimum (absolute) bearing (delta)
  void slotMinValueBearing( const QString& _rqsMinValue );
  /// [MinValue] Slot to modify the minimum (absolute) horizontal speed
  void slotMinValueSpeed( const QString& _rqsMinValue );
  /// [MinValue] Slot to modify the minimum (absolute) vertical speed
  void slotMinValueSpeedVertical( const QString& _rqsMinValue );

  /// [MaxError] Slot to modify the position maximum error
  void slotMaxErrorPosition( const QString& _rqsMaxError );
  /// [MaxError] Slot to modify the elevation maximum error
  void slotMaxErrorElevation( const QString& _rqsMaxError );
  /// [MaxError] Slot to modify the time maximum error
  void slotMaxErrorTime( const QString& _rqsMaxError );
  /// [MaxError] Slot to modify the bearing maximum error
  void slotMaxErrorBearing( const QString& _rqsMaxError );
  /// [MaxError] Slot to modify the horizontal speed maximum error
  void slotMaxErrorSpeed( const QString& _rqsMaxError );
  /// [MaxError] Slot to modify the vertical speed maximum error
  void slotMaxErrorSpeedVertical( const QString& _rqsMaxError );

  /// [MaxAge] Slot to modify the position maximum age
  void slotMaxAgePosition( const QString& _rqsMaxAge );
  /// [MaxAge] Slot to modify the elevation maximum age
  void slotMaxAgeElevation( const QString& _rqsMaxAge );
  /// [MaxAge] Slot to modify the time maximum age
  void slotMaxAgeTime( const QString& _rqsMaxAge );
  /// [MaxAge] Slot to modify the bearing maximum age
  void slotMaxAgeBearing( const QString& _rqsMaxAge );
  /// [MaxAge] Slot to modify the horizontal speed maximum age
  void slotMaxAgeSpeed( const QString& _rqsMaxAge );
  /// [MaxAge] Slot to modify the vertical speed maximum age
  void slotMaxAgeSpeedVertical( const QString& _rqsMaxAge );

  /// [Misc] Slot to modify the screen Dots-per-Inch (DPI)
  void slotScreenDpi( int _iScreenDpi );
  /// [Misc] Slot to modify the (multitouch) screen gestures activation status
  void slotScreenGestures( int _iScreenGestures );
  /// [Misc] Slot to modify the content refresh rate
  void slotRateRefresh( int _iRateRefresh );
  /// [Misc] Slot to modify the content redraw rate
  void slotRateRedraw( int _iRateRedraw );
  /// [Misc] Slot to modify the chart opacity
  void slotChartOpacity( int _iChartOpacity );
  /// [Misc] Slot to modify the symbols visibility (on chart)
  void slotVisibleSymbols( int _iVisibleSymbols );
  /// [Misc] Slot to modify the high-resolution printing status
  void slotPrintHighRes( int _iPrintHighRes );

  //
  // SETTERS
  //
public:
  /// [Path] Sets the current working directory
  void setPathWorkingDirectory( const QString& _rqsPathWorkingDirectory ) { qsPathWorkingDirectory = _rqsPathWorkingDirectory; };

  /// [Misc] Sets the main window geometry (Base64-encoded binary data)
  void setMainWindowGeometry( const QString& _rqsMainWindowGeometry ) { qsMainWindowGeometry = _rqsMainWindowGeometry; };
  /// [Misc] Sets the main window state (Base64-encoded binary data)
  void setMainWindowState( const QString& _rqsMainWindowState ) { qsMainWindowState = _rqsMainWindowState; };

  //
  // GETTERS
  //
public:
  /// [Path] Returns the current working directory
  QString getPathWorkingDirectory() { return qsPathWorkingDirectory; };
  /// [Path] Returns the symbols directory
  QString getPathSymbolsDirectory() { return qsPathSymbolsDirectory; };

  /// [Unit] Returns the date format/unit
  CUnitDate::EUnit getUnitDate() { return eUnitDate; };
  /// [Unit] Returns the time format/unit
  CUnitTime::EUnit getUnitTime() { return eUnitTime; };
  /// [Unit] Returns the time difference format/unit
  CUnitTimeDelta::EUnit getUnitTimeDelta() { return eUnitTimeDelta; };
  /// [Unit] Returns the timezone format/unit
  CUnitTimeZone::EUnit getUnitTimeZone() { return eUnitTimeZone; };
  /// [Unit] Returns the position format/unit
  CUnitPosition::EUnit getUnitPosition() { return eUnitPosition; };
  /// [Unit] Returns the bearing format/unit
  CUnitBearing::EUnit getUnitBearing() { return eUnitBearing; };
  /// [Unit] Returns the distance format/unit
  CUnitDistance::EUnit getUnitDistance() { return eUnitDistance; };
  /// [Unit] Returns the speed format/unit
  CUnitSpeed::EUnit getUnitSpeed() { return eUnitSpeed; };
  /// [Unit] Returns the elevation format/unit
  CUnitElevation::EUnit getUnitElevation() { return eUnitElevation; };
  /// [Unit] Returns the vertical speed format/unit
  CUnitSpeedVertical::EUnit getUnitSpeedVertical() { return eUnitSpeedVertical; };

  /// [Precision] Returns the time decimal precision
  int getPrecisionTime() { return iPrecisionTime; };
  /// [Precision] Returns the time difference decimal precision
  int getPrecisionTimeDelta() { return iPrecisionTimeDelta; };
  /// [Precision] Returns the position decimal precision
  int getPrecisionPosition() { return iPrecisionPosition; };
  /// [Precision] Returns the bearing decimal precision
  int getPrecisionBearing() { return iPrecisionBearing; };
  /// [Precision] Returns the distance decimal precision
  int getPrecisionDistance() { return iPrecisionDistance; };
  /// [Precision] Returns the speed decimal precision
  int getPrecisionSpeed() { return iPrecisionSpeed; };
  /// [Precision] Returns the elevation decimal precision
  int getPrecisionElevation() { return iPrecisionElevation; };
  /// [Precision] Returns the vertical speed decimal precision
  int getPrecisionSpeedVertical() { return iPrecisionSpeedVertical; };

  /// [MinValue] Returns the minimum (absolute) position (delta), in meters
  double getMinValuePosition() { return fdMinValuePosition; };
  /// [MinValue] Returns the minimum (absolute) bearing (delta), in meters
  double getMinValueBearing() { return fdMinValueBearing; };
  /// [MinValue] Returns the minimum (absolute) horizontal speed, in meters per second
  double getMinValueSpeed() { return fdMinValueSpeed; };
  /// [MinValue] Returns the minimum (absolute) vertical speed, in meters per second
  double getMinValueSpeedVertical() { return fdMinValueSpeedVertical; };

  /// [MaxError] Returns the position's maximum error, in meters
  double getMaxErrorPosition() { return fdMaxErrorPosition; };
  /// [MaxError] Returns the elevation's maximum error, in meters
  double getMaxErrorElevation() { return fdMaxErrorElevation; };
  /// [MaxError] Returns the time's maximum error, in seconds
  double getMaxErrorTime() { return fdMaxErrorTime; };
  /// [MaxError] Returns the bearing's maximum error, in degrees
  double getMaxErrorBearing() { return fdMaxErrorBearing; };
  /// [MaxError] Returns the horizontal speed's maximum error, in meters per second
  double getMaxErrorSpeed() { return fdMaxErrorSpeed; };
  /// [MaxError] Returns the vertical speed's maximum error, in meters per second
  double getMaxErrorSpeedVertical() { return fdMaxErrorSpeedVertical; };

  /// [MaxAge] Returns the position's maximum error, in seconds
  double getMaxAgePosition() { return fdMaxAgePosition; };
  /// [MaxAge] Returns the elevation's maximum error, in seconds
  double getMaxAgeElevation() { return fdMaxAgeElevation; };
  /// [MaxAge] Returns the time's maximum error, in seconds
  double getMaxAgeTime() { return fdMaxAgeTime; };
  /// [MaxAge] Returns the bearing's maximum error, in seconds
  double getMaxAgeBearing() { return fdMaxAgeBearing; };
  /// [MaxAge] Returns the horizontal speed's maximum error, in seconds
  double getMaxAgeSpeed() { return fdMaxAgeSpeed; };
  /// [MaxAge] Returns the vertical speed's maximum error, in seconds
  double getMaxAgeSpeedVertical() { return fdMaxAgeSpeedVertical; };

  /// [Color] Returns the pointer overlay's base color
  QColor getColorPointer() { return qColorPointer; };
  /// [Color] Returns the landmark overlay's base color
  QColor getColorLandmark() { return qColorLandmark; };
  /// [Color] Returns the route overlay's base color
  QColor getColorRoute() { return qColorRoute; };
  /// [Color] Returns the track overlay's base color
  QColor getColorTrack() { return qColorTrack; };
  /// [Color] Returns the vessel overlay's base color
  QColor getColorVessel() { return qColorVessel; };

  /// [Misc] Returns the main window geometry (Base64-encoded binary data)
  QString getMainWindowGeometry() { return qsMainWindowGeometry; };
  /// [Misc] Returns the main window state (Base64-encoded binary data)
  QString getMainWindowState() { return qsMainWindowState; };
  /// [Misc] Returns the screen Dots-per-Inch (DPI)
  int getScreenDpi() { return iScreenDpi; };
  /// [Misc] Returns the screen (multitouch) gestures activation status
  bool isScreenGestures() { return bScreenGestures; };
  /// [Misc] Returns the content refresh rate, in milliseconds
  int getRateRefresh() { return iRateRefresh; };
  /// [Misc] Returns the content redraw rate, in seconds
  int getRateRedraw() { return iRateRedraw; };
  /// [Misc] Returns the chart opacity [0,100]
  int getChartOpacity() { return iChartOpacity; };
  /// [Misc] Returns the symbols visibility (on chart)
  bool isVisibleSymbols() { return bVisibleSymbols; };
  /// [Misc] Returns the high-resolution printing status
  bool isPrintHighRes() { return bPrintHighRes; };

  //
  // OTHER
  //
protected:
  /// Verifies and validates all parameters
  void validate();

public:
  /// Store all parameters to the given file
  void save( const QString& _rqsFilename ) const;
  /// Restore all parameters from the given file
  void load( const QString& _rqsFilename );
  /// Stores parameters to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter& _rqXmlStreamWriter, bool _bProjectDump = false ) const;
  /// Retrieves parameters from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );

};

#endif // QVCT_CSETTINGS_HPP
