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
#include <QApplication>
#include <QDir>
#include <QTextCodec>

// QVCT
#include "QVCTRuntime.hpp"

int main( int argc, char* argv[] )
{
  // Encoding
  QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );

  // Application
  QApplication qApplication( argc, argv );

  // Settings (load)
  QString __qsSettingsFilename = QDir::homePath()+CONFDIR_STRING+"/settings.qvct";
  CSettings* __poSettings = new CSettings();
  QVCTRuntime::registerSettings( __poSettings );
  __poSettings->load( __qsSettingsFilename );

  // Main window
  CMainWindow* __poMainWindow = new CMainWindow();
  QVCTRuntime::registerMainWindow( __poMainWindow );

  // Devices (load)
  QString __qsDevicesFilename = QDir::homePath()+CONFDIR_STRING+"/devices.qvct";
  CDeviceOverlay* __poDeviceOverlay = QVCTRuntime::useDeviceOverlay();
  __poDeviceOverlay->load( __qsDevicesFilename, true );

  // Chart table
  if( argc >= 2 ) QVCTRuntime::useChartTable()->load( argv[1] );

  // Main loop
  __poMainWindow->restoreGeometry( QByteArray::fromBase64( __poSettings->getMainWindowGeometry().toAscii() ) );
  __poMainWindow->restoreState( QByteArray::fromBase64( __poSettings->getMainWindowState().toAscii() ) );
  __poMainWindow->show();
  int __iExit = qApplication.exec();

  // Exit
  if( !__iExit ) // if application terminated normally (exit=0)
  {
    // Devices (save)
    __poDeviceOverlay->save( __qsDevicesFilename, 0, true );

    // Settings (save)
    __poSettings->setMainWindowGeometry( QString( __poMainWindow->saveGeometry().toBase64() ) );
    __poSettings->setMainWindowState( QString( __poMainWindow->saveState().toBase64() ) );
    __poSettings->save( __qsSettingsFilename );
  }
  return __iExit;
}
