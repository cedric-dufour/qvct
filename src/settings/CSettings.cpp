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
#include <QColor>
#include <QDir>
#include <QDomDocument> // QtXml module
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QXmlStreamWriter>

// QVCT
#include "main.hpp"
#include "settings/CSettings.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CSettings::CSettings()
  : eUnitDate( CUnitDate::YMD )
  , eUnitTime( CUnitTime::HMS )
  , eUnitTimeDelta( CUnitTimeDelta::HMS )
  , eUnitTimeZone( CUnitTimeZone::UTC )
  , eUnitPosition( CUnitPosition::DMS )
  , eUnitBearing( CUnitBearing::DEG )
  , eUnitDistance( CUnitDistance::M )
  , eUnitSpeed( CUnitSpeed::M_S )
  , eUnitElevation( CUnitElevation::M )
  , eUnitSpeedVertical( CUnitSpeedVertical::M_S )
  , iPrecisionTime( 0 )
  , iPrecisionTimeDelta( 0 )
  , iPrecisionPosition( 0 )
  , iPrecisionBearing( 0 )
  , iPrecisionDistance( 0 )
  , iPrecisionSpeed( 0 )
  , iPrecisionElevation( 0 )
  , iPrecisionSpeedVertical( 0 )
  , fdMinValuePosition( 100.0 )
  , fdMinValueBearing( 30.0 )
  , fdMinValueSpeed( 0.25 )
  , fdMinValueSpeedVertical( 0.25 )
  , fdMaxErrorPosition( 25.0 )
  , fdMaxErrorElevation( 25.0 )
  , fdMaxErrorTime( 1.0 )
  , fdMaxErrorBearing( 10.0 )
  , fdMaxErrorSpeed( 10.0 )
  , fdMaxErrorSpeedVertical( 10.0 )
  , fdMaxAgePosition( 5.0 )
  , fdMaxAgeElevation( 5.0 )
  , fdMaxAgeTime( 5.0 )
  , fdMaxAgeBearing( 5.0 )
  , fdMaxAgeSpeed( 5.0 )
  , fdMaxAgeSpeedVertical( 5.0 )
  , qColorPointer( 64, 64, 64 )
  , qColorLandmark( 0, 64, 0 )
  , qColorRoute( 0, 0, 128 )
  , qColorTrack( 128, 0, 128 )
  , qColorVessel( 128, 0, 0 )
  , iScreenDpi( 96 )
  , bScreenGestures( false )
  , iRateRefresh( 1000 )
  , iRateRedraw( 5 )
  , bVisibleSymbols( true )
  , bPrintHighRes( false )
{
  qsPathWorkingDirectory = QDir::homePath();
  qsPathSymbolsDirectory = QDir::homePath()+CONFDIR_STRING+"/symbols";
  validate();
}



//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CSettings::slotPathWorkingDirectory( const QString& _rqsPathWorkingDirectory )
{
  qsPathWorkingDirectory = _rqsPathWorkingDirectory;
}

void CSettings::slotPathSymbolsDirectory( const QString& _rqsPathSymbolsDirectory )
{
  qsPathSymbolsDirectory = _rqsPathSymbolsDirectory;
}

void CSettings::slotUnitDate( const QString& _rqsSymbol )
{
  eUnitDate = CUnitDate::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitTime( const QString& _rqsSymbol )
{
  eUnitTime = CUnitTime::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitTimeDelta( const QString& _rqsSymbol )
{
  eUnitTimeDelta = CUnitTimeDelta::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitTimeZone( const QString& _rqsSymbol )
{
  eUnitTimeZone = CUnitTimeZone::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitPosition( const QString& _rqsSymbol )
{
  eUnitPosition = CUnitPosition::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitBearing( const QString& _rqsSymbol )
{
  eUnitBearing = CUnitBearing::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitDistance( const QString& _rqsSymbol )
{
  eUnitDistance = CUnitDistance::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitSpeed( const QString& _rqsSymbol )
{
  eUnitSpeed = CUnitSpeed::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitElevation( const QString& _rqsSymbol )
{
  eUnitElevation = CUnitElevation::fromSymbol( _rqsSymbol );
}

void CSettings::slotUnitSpeedVertical( const QString& _rqsSymbol )
{
  eUnitSpeedVertical = CUnitSpeedVertical::fromSymbol( _rqsSymbol );
}

void CSettings::slotMinValueBearing( const QString& _rqsMinValue )
{
  fdMinValueBearing = _rqsMinValue.toDouble();
}

void CSettings::slotMinValuePosition( const QString& _rqsMinValue )
{
  fdMinValuePosition = _rqsMinValue.toDouble();
}

void CSettings::slotMinValueSpeed( const QString& _rqsMinValue )
{
  fdMinValueSpeed = _rqsMinValue.toDouble();
}

void CSettings::slotMinValueSpeedVertical( const QString& _rqsMinValue )
{
  fdMinValueSpeedVertical = _rqsMinValue.toDouble();
}

void CSettings::slotMaxErrorPosition( const QString& _rqsMaxError )
{
  fdMaxErrorPosition = _rqsMaxError.toDouble();
}

void CSettings::slotMaxErrorElevation( const QString& _rqsMaxError )
{
  fdMaxErrorElevation = _rqsMaxError.toDouble();
}

void CSettings::slotMaxErrorTime( const QString& _rqsMaxError )
{
  fdMaxErrorTime = _rqsMaxError.toDouble();
}

void CSettings::slotMaxErrorBearing( const QString& _rqsMaxError )
{
  fdMaxErrorBearing = _rqsMaxError.toDouble();
}

void CSettings::slotMaxErrorSpeed( const QString& _rqsMaxError )
{
  fdMaxErrorSpeed = _rqsMaxError.toDouble();
}

void CSettings::slotMaxErrorSpeedVertical( const QString& _rqsMaxError )
{
  fdMaxErrorSpeedVertical = _rqsMaxError.toDouble();
}

void CSettings::slotMaxAgePosition( const QString& _rqsMaxAge )
{
  fdMaxAgePosition = _rqsMaxAge.toDouble();
}

void CSettings::slotMaxAgeElevation( const QString& _rqsMaxAge )
{
  fdMaxAgeElevation = _rqsMaxAge.toDouble();
}

void CSettings::slotMaxAgeTime( const QString& _rqsMaxAge )
{
  fdMaxAgeTime = _rqsMaxAge.toDouble();
}

void CSettings::slotMaxAgeBearing( const QString& _rqsMaxAge )
{
  fdMaxAgeBearing = _rqsMaxAge.toDouble();
}

void CSettings::slotMaxAgeSpeed( const QString& _rqsMaxAge )
{
  fdMaxAgeSpeed = _rqsMaxAge.toDouble();
}

void CSettings::slotMaxAgeSpeedVertical( const QString& _rqsMaxAge )
{
  fdMaxAgeSpeedVertical = _rqsMaxAge.toDouble();
}

void CSettings::slotScreenDpi( int _iScreenDpi )
{
  iScreenDpi = _iScreenDpi;
}

void CSettings::slotScreenGestures( int _iScreenGestures )
{
  bScreenGestures = _iScreenGestures == Qt::Checked;
}

void CSettings::slotRateRefresh( int _iRateRefresh )
{
  iRateRefresh = _iRateRefresh;
}

void CSettings::slotRateRedraw( int _iRateRedraw )
{
  iRateRedraw = _iRateRedraw;
}

void CSettings::slotVisibleSymbols( int _iVisibleSymbols )
{
  bVisibleSymbols = _iVisibleSymbols == Qt::Checked;
}

void CSettings::slotPrintHighRes( int _iPrintHighRes )
{
  bPrintHighRes = _iPrintHighRes == Qt::Checked;
}

//
// OTHER
//

void CSettings::validate()
{
  // Paths
  // ... working directory
  {
    QFileInfo __qFileInfo( qsPathWorkingDirectory );
    if( !__qFileInfo.exists() || !__qFileInfo.isReadable() )
    {
      qCritical( "INFO[%s]: Working directory can not be read (%s); resetting to default", Q_FUNC_INFO, qPrintable( qsPathWorkingDirectory ) );
      qsPathWorkingDirectory = QDir::homePath();
      QDir __qDir( qsPathWorkingDirectory );
      if( !__qDir.exists() && !__qDir.mkpath( __qDir.path() ) )
      {
        qCritical( "ERROR[%s]: Failed to create directory (%s)", Q_FUNC_INFO, qPrintable( __qDir.path() ) );
      }
    }
  }
  // ... symbols directory
  {
    QFileInfo __qFileInfo( qsPathSymbolsDirectory );
    if( !__qFileInfo.exists() || !__qFileInfo.isReadable() )
    {
      qCritical( "INFO[%s]: Symbols directory can not be read (%s); resetting to default", Q_FUNC_INFO, qPrintable( qsPathSymbolsDirectory ) );
      qsPathSymbolsDirectory = QDir::homePath()+CONFDIR_STRING+"/symbols";
      QDir __qDir( qsPathSymbolsDirectory );
      if( !__qDir.exists() && !__qDir.mkpath( __qDir.path() ) )
      {
        qCritical( "ERROR[%s]: Failed to create directory (%s)", Q_FUNC_INFO, qPrintable( __qDir.path() ) );
      }
    }
  }

  // Units
  if( eUnitDate == CUnitDate::UNDEFINED ) eUnitDate = CUnitDate::YMD;
  if( eUnitTime == CUnitTime::UNDEFINED ) eUnitTime = CUnitTime::HMS;
  if( eUnitTimeDelta == CUnitTimeDelta::UNDEFINED ) eUnitTimeDelta = CUnitTimeDelta::HMS;
  if( eUnitTimeZone == CUnitTimeZone::UNDEFINED ) eUnitTimeZone = CUnitTimeZone::UTC;
  if( eUnitPosition == CUnitPosition::UNDEFINED ) eUnitPosition = CUnitPosition::DMS;
  if( eUnitBearing == CUnitBearing::UNDEFINED ) eUnitBearing = CUnitBearing::DEG;
  if( eUnitDistance == CUnitDistance::UNDEFINED ) eUnitDistance = CUnitDistance::M;
  if( eUnitSpeed == CUnitSpeed::UNDEFINED ) eUnitSpeed = CUnitSpeed::M_S;
  if( eUnitElevation == CUnitElevation::UNDEFINED ) eUnitElevation = CUnitElevation::M;
  if( eUnitSpeedVertical == CUnitSpeedVertical::UNDEFINED ) eUnitSpeedVertical = CUnitSpeedVertical::M_S;

  // Precision
  if( iPrecisionTime < 0 ) iPrecisionTime = 0; if( iPrecisionTime > 3 ) iPrecisionTime = 3;
  if( iPrecisionTimeDelta < 0 ) iPrecisionTimeDelta = 0; if( iPrecisionTimeDelta > 3 ) iPrecisionTimeDelta = 3;
  if( iPrecisionPosition < 0 ) iPrecisionPosition = 0; if( iPrecisionPosition > 6 ) iPrecisionPosition = 6;
  if( iPrecisionBearing < 0 ) iPrecisionBearing = 0; if( iPrecisionBearing > 3 ) iPrecisionBearing = 3;
  if( iPrecisionDistance < 0 ) iPrecisionDistance = 0; if( iPrecisionDistance > 3 ) iPrecisionDistance = 3;
  if( iPrecisionSpeed < 0 ) iPrecisionSpeed = 0; if( iPrecisionSpeed > 3 ) iPrecisionSpeed = 3;
  if( iPrecisionElevation < 0 ) iPrecisionElevation = 0; if( iPrecisionElevation > 3 ) iPrecisionElevation = 3;
  if( iPrecisionSpeedVertical < 0 ) iPrecisionSpeedVertical = 0; if( iPrecisionSpeedVertical > 3 ) iPrecisionSpeedVertical = 3;

  // Minimum values
  if( fdMinValueBearing < 0.0 ) fdMinValueBearing = 0.0; if( fdMinValueBearing > 180.0 ) fdMinValueBearing = 180.0;
  if( fdMinValuePosition < 0.0 ) fdMinValuePosition = 0.0;
  if( fdMinValueSpeed < 0.0 ) fdMinValueSpeed = 0.0;
  if( fdMinValueSpeedVertical < 0.0 ) fdMinValueSpeedVertical = 0.0;

  // Maximum error
  if( fdMaxErrorPosition < 0.0 ) fdMaxErrorPosition = 0.0;
  if( fdMaxErrorElevation < 0.0 ) fdMaxErrorElevation = 0.0;
  if( fdMaxErrorTime < 0.0 ) fdMaxErrorTime = 0.0;
  if( fdMaxErrorBearing < 0.0 ) fdMaxErrorBearing = 0.0;
  if( fdMaxErrorSpeed < 0.0 ) fdMaxErrorSpeed = 0.0;
  if( fdMaxErrorSpeedVertical < 0.0 ) fdMaxErrorSpeedVertical = 0.0;

  // Maximum age
  if( fdMaxAgePosition < 0.0 ) fdMaxAgePosition = 0.0;
  if( fdMaxAgeElevation < 0.0 ) fdMaxAgeElevation = 0.0;
  if( fdMaxAgeTime < 0.0 ) fdMaxAgeTime = 0.0;
  if( fdMaxAgeBearing < 0.0 ) fdMaxAgeBearing = 0.0;
  if( fdMaxAgeSpeed < 0.0 ) fdMaxAgeSpeed = 0.0;
  if( fdMaxAgeSpeedVertical < 0.0 ) fdMaxAgeSpeedVertical = 0.0;


  // Misc.
  if( iScreenDpi < 1 ) iScreenDpi = 1; if( iScreenDpi > 1200 ) iScreenDpi = 1200;
  if( iRateRefresh < 100 ) iRateRefresh = 100; if( iRateRefresh > 5000 ) iRateRefresh = 5000;
  if( iRateRedraw < 1 ) iRateRedraw = 1; if( iRateRedraw > 300 ) iRateRedraw = 300;
}

void CSettings::save( const QString& _rqsFilename ) const
{
  //qDebug( "DEBUG[%s]: %s", Q_FUNC_INFO, _rqsFilename.toUtf8().constData() );

  // File [open]
  QFileInfo __qFileInfo( _rqsFilename );
  QDir __qDir( __qFileInfo.absolutePath() );
  if( !__qDir.exists() && !__qDir.mkpath( __qDir.path() ) )
  {
    qCritical( "ERROR[%s]: Failed to create directory (%s)", Q_FUNC_INFO, qPrintable( __qDir.path() ) );
    return;
  }
  QFile __qFile( __qFileInfo.absoluteFilePath() );
  if( !__qFile.open( QIODevice::WriteOnly ) )
  {
    qCritical( "ERROR[%s]: Failed to open file (%s)", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    return;
  }

  // XML
  QXmlStreamWriter __qXmlStreamWriter( &__qFile );
  __qXmlStreamWriter.setAutoFormatting( true );
  __qXmlStreamWriter.writeStartDocument();
  __qXmlStreamWriter.writeStartElement( "QVCT" );
  dumpQVCT( __qXmlStreamWriter, false );
  __qXmlStreamWriter.writeEndElement(); // QVCT
  __qXmlStreamWriter.writeEndDocument();

  // File [close]
  __qFile.close();
}

void CSettings::dumpQVCT( QXmlStreamWriter& _rqXmlStreamWriter, bool _bProjectDump ) const
{
  // XML [start]
  _rqXmlStreamWriter.writeStartElement( "Settings" );

  // Paths [start]
  if( !_bProjectDump )
  {
    _rqXmlStreamWriter.writeStartElement( "Paths" );
    // ... working directory
    _rqXmlStreamWriter.writeStartElement( "Working" );
    _rqXmlStreamWriter.writeAttribute( "path", qsPathWorkingDirectory );
    _rqXmlStreamWriter.writeEndElement(); // Working
    // ... symbols directory
    _rqXmlStreamWriter.writeStartElement( "Symbols" );
    _rqXmlStreamWriter.writeAttribute( "path", qsPathSymbolsDirectory );
    _rqXmlStreamWriter.writeEndElement(); // Symbols
    // Paths [end]
    _rqXmlStreamWriter.writeEndElement(); // Paths
  }

  // Units [start]
  _rqXmlStreamWriter.writeStartElement( "Units" );
  // ... date unit
  _rqXmlStreamWriter.writeStartElement( "Date" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitDate::toCode( eUnitDate ) );
  _rqXmlStreamWriter.writeEndElement(); // Date
  // ... time unit/precision
  _rqXmlStreamWriter.writeStartElement( "Time" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitTime::toCode( eUnitTime ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionTime ) );
  _rqXmlStreamWriter.writeEndElement(); // Time
  // ... time difference unit/precision
  _rqXmlStreamWriter.writeStartElement( "TimeDelta" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitTimeDelta::toCode( eUnitTimeDelta ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionTimeDelta ) );
  _rqXmlStreamWriter.writeEndElement(); // TimeDelta
  // ... timezone unit
  _rqXmlStreamWriter.writeStartElement( "TimeZone" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitTimeZone::toCode( eUnitTimeZone ) );
  _rqXmlStreamWriter.writeEndElement(); // TimeZone
  // ... position unit/precision
  _rqXmlStreamWriter.writeStartElement( "Position" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitPosition::toCode( eUnitPosition ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionPosition ) );
  _rqXmlStreamWriter.writeEndElement(); // Position
  // ... elevation unit/precision
  _rqXmlStreamWriter.writeStartElement( "Elevation" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitElevation::toCode( eUnitElevation ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionElevation ) );
  _rqXmlStreamWriter.writeEndElement(); // Elevation
  // ... bearing unit/precision
  _rqXmlStreamWriter.writeStartElement( "Bearing" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitBearing::toCode( eUnitBearing ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionBearing ) );
  _rqXmlStreamWriter.writeEndElement(); // Bearing
  // ... distance unit/precision
  _rqXmlStreamWriter.writeStartElement( "Distance" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitDistance::toCode( eUnitDistance ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionDistance ) );
  _rqXmlStreamWriter.writeEndElement(); // Distance
  // ... speed unit/precision
  _rqXmlStreamWriter.writeStartElement( "Speed" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitSpeed::toCode( eUnitSpeed ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionSpeed ) );
  _rqXmlStreamWriter.writeEndElement(); // Speed
  // ... vertical speed unit/precision
  _rqXmlStreamWriter.writeStartElement( "SpeedVertical" );
  _rqXmlStreamWriter.writeAttribute( "unit", CUnitSpeedVertical::toCode( eUnitSpeedVertical ) );
  _rqXmlStreamWriter.writeAttribute( "precision", QString::number( iPrecisionSpeedVertical ) );
  _rqXmlStreamWriter.writeEndElement(); // SpeedVertical
  // Units [end]
  _rqXmlStreamWriter.writeEndElement(); // Units

  // Validity [start]
  _rqXmlStreamWriter.writeStartElement( "Validity" );
  // ... position validity parameters
  _rqXmlStreamWriter.writeStartElement( "Position" );
  _rqXmlStreamWriter.writeAttribute( "min_value", QString::number( fdMinValuePosition ) );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorPosition ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgePosition ) );
  _rqXmlStreamWriter.writeEndElement(); // Position
  // ... elevation validity parameters
  _rqXmlStreamWriter.writeStartElement( "Elevation" );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorElevation ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgeElevation ) );
  _rqXmlStreamWriter.writeEndElement(); // Elevation
  // ... time validity parameters
  _rqXmlStreamWriter.writeStartElement( "Time" );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorTime ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgeTime ) );
  _rqXmlStreamWriter.writeEndElement(); // Time
  // ... bearing validity parameters
  _rqXmlStreamWriter.writeStartElement( "Bearing" );
  _rqXmlStreamWriter.writeAttribute( "min_value", QString::number( fdMinValueBearing ) );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorBearing ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgeBearing ) );
  _rqXmlStreamWriter.writeEndElement(); // Bearing
  // ... horizontal speed validity parameters
  _rqXmlStreamWriter.writeStartElement( "Speed" );
  _rqXmlStreamWriter.writeAttribute( "min_value", QString::number( fdMinValueSpeed ) );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorSpeed ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgeSpeed ) );
  _rqXmlStreamWriter.writeEndElement(); // Speed
  // ... vertical speed validity parameters
  _rqXmlStreamWriter.writeStartElement( "SpeedVertical" );
  _rqXmlStreamWriter.writeAttribute( "min_value", QString::number( fdMinValueSpeedVertical ) );
  _rqXmlStreamWriter.writeAttribute( "max_error", QString::number( fdMaxErrorSpeedVertical ) );
  _rqXmlStreamWriter.writeAttribute( "max_age", QString::number( fdMaxAgeSpeedVertical ) );
  _rqXmlStreamWriter.writeEndElement(); // SpeedVertical
  // Validity [end]
  _rqXmlStreamWriter.writeEndElement(); // Validity

  // Colors [start]
  if( !_bProjectDump )
  {
    _rqXmlStreamWriter.writeStartElement( "Colors" );
    // ... chart
    _rqXmlStreamWriter.writeStartElement( "Pointer" );
    _rqXmlStreamWriter.writeAttribute( "red", QString::number( qColorPointer.red() ) );
    _rqXmlStreamWriter.writeAttribute( "green", QString::number( qColorPointer.green() ) );
    _rqXmlStreamWriter.writeAttribute( "blue", QString::number( qColorPointer.blue() ) );
    _rqXmlStreamWriter.writeEndElement(); // Pointer
    // ... landmark
    _rqXmlStreamWriter.writeStartElement( "Landmark" );
    _rqXmlStreamWriter.writeAttribute( "red", QString::number( qColorLandmark.red() ) );
    _rqXmlStreamWriter.writeAttribute( "green", QString::number( qColorLandmark.green() ) );
    _rqXmlStreamWriter.writeAttribute( "blue", QString::number( qColorLandmark.blue() ) );
    _rqXmlStreamWriter.writeEndElement(); // Landmark
    // ... route
    _rqXmlStreamWriter.writeStartElement( "Route" );
    _rqXmlStreamWriter.writeAttribute( "red", QString::number( qColorRoute.red() ) );
    _rqXmlStreamWriter.writeAttribute( "green", QString::number( qColorRoute.green() ) );
    _rqXmlStreamWriter.writeAttribute( "blue", QString::number( qColorRoute.blue() ) );
    _rqXmlStreamWriter.writeEndElement(); // Route
    // ... track
    _rqXmlStreamWriter.writeStartElement( "Track" );
    _rqXmlStreamWriter.writeAttribute( "red", QString::number( qColorTrack.red() ) );
    _rqXmlStreamWriter.writeAttribute( "green", QString::number( qColorTrack.green() ) );
    _rqXmlStreamWriter.writeAttribute( "blue", QString::number( qColorTrack.blue() ) );
    _rqXmlStreamWriter.writeEndElement(); // Track
    // ... vessel
    _rqXmlStreamWriter.writeStartElement( "Vessel" );
    _rqXmlStreamWriter.writeAttribute( "red", QString::number( qColorVessel.red() ) );
    _rqXmlStreamWriter.writeAttribute( "green", QString::number( qColorVessel.green() ) );
    _rqXmlStreamWriter.writeAttribute( "blue", QString::number( qColorVessel.blue() ) );
    _rqXmlStreamWriter.writeEndElement(); // Vessel
    // Colors [end]
    _rqXmlStreamWriter.writeEndElement(); // Colors
  }

  // Options [start]
  if( !_bProjectDump )
  {
    _rqXmlStreamWriter.writeStartElement( "Options" );
    // ... main window
    _rqXmlStreamWriter.writeStartElement( "MainWindow" );
    _rqXmlStreamWriter.writeAttribute( "geometry", qsMainWindowGeometry );
    _rqXmlStreamWriter.writeAttribute( "state", qsMainWindowState );
    _rqXmlStreamWriter.writeEndElement(); // MainWindow
    // ... screen
    _rqXmlStreamWriter.writeStartElement( "Screen" );
    _rqXmlStreamWriter.writeAttribute( "dpi", QString::number( iScreenDpi ) );
    _rqXmlStreamWriter.writeAttribute( "gestures", QString::number( (int)bScreenGestures ) );
    _rqXmlStreamWriter.writeEndElement(); // Screen
    // ... rates
    _rqXmlStreamWriter.writeStartElement( "Rates" );
    _rqXmlStreamWriter.writeAttribute( "refresh", QString::number( iRateRefresh ) );
    _rqXmlStreamWriter.writeAttribute( "redraw", QString::number( iRateRedraw ) );
    _rqXmlStreamWriter.writeEndElement(); // Rates
    // ... symbols
    _rqXmlStreamWriter.writeStartElement( "Symbols" );
    _rqXmlStreamWriter.writeAttribute( "visible", QString::number( (int)bVisibleSymbols ) );
    _rqXmlStreamWriter.writeEndElement(); // Symbols
    // ... printing
    _rqXmlStreamWriter.writeStartElement( "Print" );
    _rqXmlStreamWriter.writeAttribute( "high_res", QString::number( (int)bPrintHighRes ) );
    _rqXmlStreamWriter.writeEndElement(); // Print
    // Options [end]
    _rqXmlStreamWriter.writeEndElement(); // Options
  }

  // XML [end]
  _rqXmlStreamWriter.writeEndElement(); // Settings
}

void CSettings::load( const QString& _rqsFilename )
{
  //qDebug( "DEBUG[%s]: %s", Q_FUNC_INFO, _rqsFilename.toUtf8().constData() );

  // File
  QFileInfo __qFileInfo( _rqsFilename );
  QFile __qFile( __qFileInfo.absoluteFilePath() );
  if( !__qFile.open( QIODevice::ReadOnly ) )
  {
    qCritical( "INFO[%s]: Failed to open file (%s)", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    return;
  }
  QDomDocument __qDocDocument;
  if( !__qDocDocument.setContent( &__qFile ) )
  {
    qCritical( "ERROR[%s]: Failed to parse XML (%s)", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    __qFile.close();
    return;
  }
  __qFile.close();

  // XML
  QDomElement __qDomElement = __qDocDocument.documentElement();
  if( __qDomElement.isNull() || __qDomElement.nodeName() != "QVCT" )
  {
    qCritical( "ERROR[%s]: Invalid XML document type (%s); expected: 'QVCT'", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    return;
  }
  if( __qDomElement.firstChildElement( "Settings" ).isNull() )
  {
    qCritical( "ERROR[%s]: Invalid XML content (%s); expected: 'Settings'", Q_FUNC_INFO, qPrintable( __qFile.fileName() ) );
    return;
  }
  parseQVCT( __qDomElement );
}

void CSettings::parseQVCT( const QDomElement& _rqDomElement )
{
  // XML
  QDomElement __qDomElementSettings = _rqDomElement.firstChildElement( "Settings" );
  if( __qDomElementSettings.isNull() ) return;
  QDomElement __qDomElement;

  // ... paths
  {
    QDomElement __qDomElementContext = __qDomElementSettings.firstChildElement( "Paths" );
    if( !__qDomElementContext.isNull() )
    {
      // ... working directory
      __qDomElement = __qDomElementContext.firstChildElement( "Working" );
      if( !__qDomElement.isNull() )
      {
        qsPathWorkingDirectory = __qDomElement.attribute( "path", "undef" );
      }

      // ... symbols directory
      __qDomElement = __qDomElementContext.firstChildElement( "Symbols" );
      if( !__qDomElement.isNull() )
      {
        qsPathSymbolsDirectory = __qDomElement.attribute( "path", "undef" );
      }
    }
  }

  // ... units
  {
    QDomElement __qDomElementContext = __qDomElementSettings.firstChildElement( "Units" );
    if( !__qDomElementContext.isNull() )
    {
      // ... date unit
      __qDomElement = __qDomElementContext.firstChildElement( "Date" );
      if( !__qDomElement.isNull() )
      {
        eUnitDate = CUnitDate::fromCode( __qDomElement.attribute( "unit", "undef" ) );
      }
      // ... time unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Time" );
      if( !__qDomElement.isNull() )
      {
        eUnitTime = CUnitTime::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionTime = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... time difference unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "TimeDelta" );
      if( !__qDomElement.isNull() )
      {
        eUnitTimeDelta = CUnitTimeDelta::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionTimeDelta = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... timeZone unit
      __qDomElement = __qDomElementContext.firstChildElement( "TimeZone" );
      if( !__qDomElement.isNull() )
      {
        eUnitTimeZone = CUnitTimeZone::fromCode( __qDomElement.attribute( "unit", "undef" ) );
      }
      // ... position unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Position" );
      if( !__qDomElement.isNull() )
      {
        eUnitPosition = CUnitPosition::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionPosition = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... bearing unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Bearing" );
      if( !__qDomElement.isNull() )
      {
        eUnitBearing = CUnitBearing::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionBearing = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... distance unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Distance" );
      if( !__qDomElement.isNull() )
      {
        eUnitDistance = CUnitDistance::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionDistance = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... speed unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Speed" );
      if( !__qDomElement.isNull() )
      {
        eUnitSpeed = CUnitSpeed::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionSpeed = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... elevation unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "Elevation" );
      if( !__qDomElement.isNull() )
      {
        eUnitElevation = CUnitElevation::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionElevation = __qDomElement.attribute( "precision", "0" ).toInt();
      }
      // ... vertical speed unit/precision
      __qDomElement = __qDomElementContext.firstChildElement( "SpeedVertical" );
      if( !__qDomElement.isNull() )
      {
        eUnitSpeedVertical = CUnitSpeedVertical::fromCode( __qDomElement.attribute( "unit", "undef" ) );
        iPrecisionSpeedVertical = __qDomElement.attribute( "precision", "0" ).toInt();
      }
    }
  }

  // ... validity
  {
    QDomElement __qDomElementContext = __qDomElementSettings.firstChildElement( "Validity" );
    if( !__qDomElementContext.isNull() )
    {
      // ... position validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "Position" );
      if( !__qDomElement.isNull() )
      {
        fdMinValuePosition = __qDomElement.attribute( "min_value", "100.0" ).toDouble();
        fdMaxErrorPosition = __qDomElement.attribute( "max_error", "25.0" ).toDouble();
        fdMaxAgePosition = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
      // ... elevation validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "Elevation" );
      if( !__qDomElement.isNull() )
      {
        fdMaxErrorElevation = __qDomElement.attribute( "max_error", "25.0" ).toDouble();
        fdMaxAgeElevation = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
      // ... time validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "Time" );
      if( !__qDomElement.isNull() )
      {
        fdMaxErrorTime = __qDomElement.attribute( "max_error", "1.0" ).toDouble();
        fdMaxAgeTime = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
      // ... bearing validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "Bearing" );
      if( !__qDomElement.isNull() )
      {
        fdMinValueBearing = __qDomElement.attribute( "min_value", "30.0" ).toDouble();
        fdMaxErrorBearing = __qDomElement.attribute( "max_error", "10.0" ).toDouble();
        fdMaxAgeBearing = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
      // ... horizontal speed validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "Speed" );
      if( !__qDomElement.isNull() )
      {
        fdMinValueSpeed = __qDomElement.attribute( "min_value", "0.25" ).toDouble();
        fdMaxErrorSpeed = __qDomElement.attribute( "max_error", "10.0" ).toDouble();
        fdMaxAgeSpeed = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
      // ... vertical speed validity parameters
      __qDomElement = __qDomElementContext.firstChildElement( "SpeedVertical" );
      if( !__qDomElement.isNull() )
      {
        fdMinValueSpeedVertical = __qDomElement.attribute( "min_value", "0.25" ).toDouble();
        fdMaxErrorSpeedVertical = __qDomElement.attribute( "max_error", "10.0" ).toDouble();
        fdMaxAgeSpeedVertical = __qDomElement.attribute( "max_age", "5.0" ).toDouble();
      }
    }
  }

  // ... colors
  {
    QDomElement __qDomElementContext = __qDomElementSettings.firstChildElement( "Colors" );
    if( !__qDomElementContext.isNull() )
    {
      int __iRed, __iGreen, __iBlue;
      // ... chart
      __qDomElement = __qDomElementContext.firstChildElement( "Pointer" );
      if( !__qDomElement.isNull() )
      {
        __iRed = __qDomElement.attribute( "red", "64" ).toInt();
        __iGreen = __qDomElement.attribute( "green", "64" ).toInt();
        __iBlue = __qDomElement.attribute( "blue", "64" ).toInt();
        qColorPointer.setRgb( __iRed, __iGreen, __iBlue );
      }
      // ... landmark
      __qDomElement = __qDomElementContext.firstChildElement( "Landmark" );
      if( !__qDomElement.isNull() )
      {
        __iRed = __qDomElement.attribute( "red", "0" ).toInt();
        __iGreen = __qDomElement.attribute( "green", "64" ).toInt();
        __iBlue = __qDomElement.attribute( "blue", "0" ).toInt();
        qColorLandmark.setRgb( __iRed, __iGreen, __iBlue );
      }
      // ... route
      __qDomElement = __qDomElementContext.firstChildElement( "Route" );
      if( !__qDomElement.isNull() )
      {
        __iRed = __qDomElement.attribute( "red", "0" ).toInt();
        __iGreen = __qDomElement.attribute( "green", "0" ).toInt();
        __iBlue = __qDomElement.attribute( "blue", "128" ).toInt();
        qColorRoute.setRgb( __iRed, __iGreen, __iBlue );
      }
      // ... track
      __qDomElement = __qDomElementContext.firstChildElement( "Track" );
      if( !__qDomElement.isNull() )
      {
        __iRed = __qDomElement.attribute( "red", "128" ).toInt();
        __iGreen = __qDomElement.attribute( "green", "0" ).toInt();
        __iBlue = __qDomElement.attribute( "blue", "128" ).toInt();
        qColorTrack.setRgb( __iRed, __iGreen, __iBlue );
      }
      // ... vessel
      __qDomElement = __qDomElementContext.firstChildElement( "Vessel" );
      if( !__qDomElement.isNull() )
      {
        __iRed = __qDomElement.attribute( "red", "128" ).toInt();
        __iGreen = __qDomElement.attribute( "green", "0" ).toInt();
        __iBlue = __qDomElement.attribute( "blue", "0" ).toInt();
        qColorVessel.setRgb( __iRed, __iGreen, __iBlue );
      }
    }
  }

  // ... options
  {
    QDomElement __qDomElementContext = __qDomElementSettings.firstChildElement( "Options" );
    if( !__qDomElementContext.isNull() )
    {
      // ... main window
      __qDomElement = __qDomElementContext.firstChildElement( "MainWindow" );
      if( !__qDomElement.isNull() )
      {
        qsMainWindowGeometry = __qDomElement.attribute( "geometry", "undef" );
        qsMainWindowState = __qDomElement.attribute( "state", "undef" );
      }
      // ... screen
      __qDomElement = __qDomElementContext.firstChildElement( "Screen" );
      if( !__qDomElement.isNull() )
      {
        iScreenDpi = __qDomElement.attribute( "dpi", "96" ).toInt();
        bScreenGestures = (bool)__qDomElement.attribute( "gestures", "0" ).toInt();
      }
      // ... rates
      __qDomElement = __qDomElementContext.firstChildElement( "Rates" );
      if( !__qDomElement.isNull() )
      {
        iRateRefresh = __qDomElement.attribute( "refresh", "1000" ).toInt();
        iRateRedraw = __qDomElement.attribute( "redraw", "5" ).toInt();
      }
      // ... symbols
      __qDomElement = __qDomElementContext.firstChildElement( "Symbols" );
      if( !__qDomElement.isNull() )
      {
        bVisibleSymbols = (bool)__qDomElement.attribute( "visible", "1" ).toInt();
      }
      // ... printing
      __qDomElement = __qDomElementContext.firstChildElement( "Print" );
      if( !__qDomElement.isNull() )
      {
        bPrintHighRes = (bool)__qDomElement.attribute( "high_res", "1" ).toInt();
      }
    }
  }

  // Validate all values
  validate();
}
