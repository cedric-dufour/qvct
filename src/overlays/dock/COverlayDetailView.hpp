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


#ifndef QVCT_COVERLAYDETAILVIEW_HPP
#define QVCT_COVERLAYDETAILVIEW_HPP

// QT
#include <QDockWidget>
#include <QStackedWidget>
#include <QWidget>

// QVCT
#include "overlays/COverlay.hpp"


/// [UI] Overlays detail view (dock widget)
/**
 *  This class provides the user-interface (QDockWidget) that allows to display
 *  overlay containers/items' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayDetailView: public QDockWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  enum EView {
    POINTER_POINT = 0,           ///< Pointer point
    LANDMARK_CONTAINER = 1,      ///< Landmark container
    LANDMARK_POINT = 2,          ///< Landmark point
    ROUTE_CONTAINER = 3,         ///< Route container
    ROUTE_POINT = 4,             ///< Route point
    TRACK_CONTAINER = 5,         ///< Track container
    TRACK_SUBCONTAINER = 6,      ///< Track sub-container (segment)
    TRACK_POINT = 7,             ///< Track point
    VESSEL_CONTAINER = 8,        ///< Vessel container
    VESSEL_CONTAINER_DEVICE = 9, ///< Vessel container's device
    VESSEL_POINT = 10,           ///< Vessel point
    VESSEL_POINT_DEVICE = 11,    ///< Vessel point's device
    DEVICE = 12                  ///< Device
  };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Stacked widget to present all viewable overlays
  /** @see switchView() */
  QStackedWidget* pqStackedWidget;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  COverlayDetailView( QWidget* _pqParent );
  virtual ~COverlayDetailView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

public:
  /// Displays the requested container/item details (switching to the appropriate widget)
  void switchView( EView eView );

};

#endif // QVCT_COVERLAYDETAILVIEW_HPP
