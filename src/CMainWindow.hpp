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

#ifndef QVCT_CMAINWINDOW_HPP
#define QVCT_CMAINWINDOW_HPP

// C/C++
#include <ctime>

// QT
#include <QMainWindow>
#include <QPixmap>

// QVCT
#include "QVCT.hpp"


/// [UI] Application main window
/**
 *  This class provides the the application's main window.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CMainWindow: public QMainWindow
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CMainWindow();
  virtual ~CMainWindow() {};

private:
  /// Constructs the layout of the application
  void constructLayout();
  /// Constructs the menus of the application
  void constructMenus();


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Content last redraw time
  time_t tTimeLastRedraw;


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to display this application's "About" details
  void slotAbout();
  /// Slot to display the application's settings edit dialog
  void slotShowSettings();
  /// Slot to display the application's overlay list (dock widget)
  void slotShowOverlayListView();
  /// Slot to display the application's overlay detail (dock widget)
  void slotShowOverlayDetailView();
  /// Slot to display the application's system time (dock widget)
  void slotShowTimeView();
  /// Slot to display the application's overlay position (dock widget)
  void slotShowVesselPositionDockView();
  /// Slot to display the application's overlay course (dock widget)
  void slotShowVesselCourseDockView();
  /// Slot to toggle full-screen
  void slotToggleFullscreen();
  /// Slot to periodically refresh the user interface
  void slotTimerRefresh();

  // OTHER
public:
  /// Displays a generic confirmation request before deleting content
  /** @param[in] _rqsName Content's name
   *  @return Confirmation status (TRUE if deletion is accepted, FALSE otherwise) */
  bool deleteConfirm( const QString& _rqsName );

  /// Displays a generic dialog to pick a file for the given operation (open/save)
  /** This method displays a generic dialog to pick a file for open/save
   *  operation. It will automatically update the application's current working
   *  directory accordingly.
   *  @see QFileDialog::getOpenFileName, QFileDialog::getSaveFileName
   *  @param[in] _eFileOperation File operation (open/save)
   *  @param[in] _rqsTitle Window title
   *  @param[in] _rqsFilter File (extensions) filter)
   *  @return The chosen file name */
  QString fileDialog( QVCT::EFileOperation _eFileOperation, const QString& _rqsTitle, const QString& _rqsFilter );
  /// Checks the validity of the given file name for the given file operation (open/save)
  /**  @param[in] _eFileOperation File operation (open/save)
   *  @param[in] _rqsFilename Filename
   *  @param[in] _pqsListExtensions List of allowed file extensions
   *  @return The check status (TRUE if successful, FALSE otherwise) */
  bool fileCheck( QVCT::EFileOperation _eFileOperation, const QString& _rqsFilename, const QStringList* _pqsListExtensions = 0 );
  /// Displays a generic error message for an invalid file name and operation (open/save)
  /**  @param[in] _eFileOperation File operation (open/save)
   *  @param[in] _rqsFilename Filename */
  void fileError( QVCT::EFileOperation _eFileOperation, const QString& _rqsFilename );

  /// Displays a generic error message for an invalid parsing operation
  /**  @param[in] _rqsString Source string (which failed to be parsed) */
  void parseError( const QString& _rqsString );

  /// Returns whether the given symbol exists (in the application's symbol directory)
  bool symbolExists( const QString& _rqsSymbol );
  /// Returns pixmap matching the given symbol (loaded from the application's symbol directory)
  QPixmap symbolPixmap( const QString& _rqsSymbol );

};

#endif // QVCT_CMAINWINDOW_HPP
