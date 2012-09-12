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

#ifndef QVCT_CTRACKITEMDETAILVIEW_HPP
#define QVCT_CTRACKITEMDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CTrackPoint;
class COverlayText;
class COverlayUrl;


/// [UI] Track item's detail view
/**
 *  This class implements the user-interface that allows to display track
 *  items' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CTrackPointDetailView: public COverlayObjectDetailView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
  /// [UI:Label] Longitude
  COverlayText* poTextLongitude;
  /// [UI:Label] Latitude
  COverlayText* poTextLatitude;
  /// [UI:Label] Elevation
  COverlayText* poTextElevation;
  /// [UI:Label] Date
  COverlayText* poTextDate;
  /// [UI:Label] Time
  COverlayText* poTextTime;
  /// [UI:Label] Time (alternate timezone)
  COverlayText* poTextTime2;
  /// [UI:Label] Fix type
  COverlayText* poTextFixType;
  /// [UI:Label] Satellite
  COverlayText* poTextSatellite;
  /// [UI:Label] DOP
  COverlayText* poTextDop;
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
  CTrackPointDetailView( QWidget* _pqParent );
  virtual ~CTrackPointDetailView() {};

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
  /// [UI:Slot] Slot to center chart (on item's position)
  void slotPositionCenter();
  /// [UI:Slot] Slot to delete (this item/point)
  void slotDelete();
  /// [UI:Slot] Slot to add new landmark
  void slotAddLandmark();
  /// [UI:Slot] Slot to add new (route) waypoint
  void slotAddRoute();

};

#endif // QVCT_CTRACKITEMDETAILVIEW_HPP
