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

#ifndef QVCT_CPOINTERITEMDETAILVIEW_HPP
#define QVCT_CPOINTERITEMDETAILVIEW_HPP

// QT
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class COverlayText;


/// [UI] Pointer item's detail view
/**
 *  This class implements the user-interface that allows to display pointer
 *  items' (point and path) details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CPointerPointDetailView: public COverlayObjectDetailView
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Longitude
  COverlayText* poTextLongitude;
  /// [UI:Label] Latitude
  COverlayText* poTextLatitude;
  /// [UI:Label] Path segments count
  COverlayText* poTextPathSegments;
  /// [UI:Label] Path (rhumb-line) length
  COverlayText* poTextPathLengthRL;
  /// [UI:Label] Path (great-circle) length
  COverlayText* poTextPathLengthGC;
  /// [UI:Button] Visibility status
  QPushButton* pqPushButtonVisible;
  /// [UI:Button] Center (on chart)
  QPushButton* pqPushButtonCenter;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Add new landmark
  QPushButton* pqPushButtonAddLandmark;
  /// [UI:Button] Add new (route) waypoint
  QPushButton* pqPushButtonAddRoute;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CPointerPointDetailView( QWidget* _pqParent );
  virtual ~CPointerPointDetailView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: COverlayObjectDetailView (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void refreshContent();
  virtual void resetContent();

private:
  virtual void enableContent();
  virtual void disableContent();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// [UI:Slot] Slot to modify the visibility status
  void slotToggleVisible();
  /// [UI:Slot] Slot to center chart (on item's position)
  void slotPositionCenter();
  /// [UI:Slot] Slot to delete last path's point
  void slotDelete();
  /// [UI:Slot] Slot to add new landmark
  void slotAddLandmark();
  /// [UI:Slot] Slot to add new (route) waypoint
  void slotAddRoute();

};

#endif // QVCT_CPOINTERITEMDETAILVIEW_HPP
