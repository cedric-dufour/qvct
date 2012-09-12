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

#ifndef QVCT_COVERLAYLISTVIEW_HPP
#define QVCT_COVERLAYLISTVIEW_HPP

// QT
#include <QDockWidget>
#include <QTabWidget>
#include <QWidget>

// QVCT
class CPointerOverlay;


/// [UI] Overlays list view (dock widget)
/**
 *  This class provides the user-interface (QDockWidget) that allows to display
 *  all overlays and their content.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayListView: public QDockWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  // View ID
  enum EView {
    LANDMARK = 0, ///< Landmark overlay
    ROUTE = 1,    ///< Route overlay
    TRACK = 2,    ///< Track overlay
    VESSEL = 3,   ///< Vessel overlay
    DEVICE = 4    ///< Device overlay
  };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Pointer overlay (which must not be viewed)
  CPointerOverlay* poPointerOverlay;
  /// Tab widget to present all viewable overlays
  /** @see switchView() */
  QTabWidget* pqTabWidget;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  COverlayListView( QWidget* _pqParent );
  virtual ~COverlayListView();

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to handle tab change
  void slotChangeTab( int _iTabIndex );

  // OTHER
public:
  /// Displays the requested overlay (switching to the appropriate tab)
  void switchView( EView eView );

};

#endif // QVCT_COVERLAYLISTVIEW_HPP
