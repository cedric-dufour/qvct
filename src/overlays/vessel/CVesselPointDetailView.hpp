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

#ifndef QVCT_CVESSELITEMDETAILVIEW_HPP
#define QVCT_CVESSELITEMDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CVesselPoint;
class COverlayText;
class COverlayUrl;


/// [UI] Vessel item's detail view
/**
 *  This class implements the user-interface that allows to display vessel
 *  items' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPointDetailView: public COverlayObjectDetailView
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
  /// [UI:Label] Ground bearing
  COverlayText* poTextGroundBearing;
  /// [UI:Label] Ground horizontal speed
  COverlayText* poTextGroundSpeed;
  /// [UI:Label] Ground vertical speed
  COverlayText* poTextGroundSpeedVertical;
  /// [UI:Label] Apparent bearing
  COverlayText* poTextApparentBearing;
  /// [UI:Label] Apparent horizontal speed
  COverlayText* poTextApparentSpeed;
  /// [UI:Label] Apparent vertical speed
  COverlayText* poTextApparentSpeedVertical;
  /// [UI:Label] Date
  COverlayText* poTextDate;
  /// [UI:Label] Time
  COverlayText* poTextTime;
  /// [UI:Label] Time (alternate timezone)
  COverlayText* poTextTime2;
  /// [UI:Label] Type
  COverlayText* poTextType;
  /// [UI:Label] Description
  COverlayText* poTextDescription;
  /// [UI:Label] Comment
  COverlayText* poTextComment;
  /// [UI:Label] Symbol
  QLabel* pqLabelSymbol;
  /// [UI:Label] URL
  COverlayUrl* poUrl;
  /// [UI:Button] Visibility status
  QPushButton* pqPushButtonVisible;
  /// [UI:Button] Center (on chart)
  QPushButton* pqPushButtonCenter;
  /// [UI:Button] Center (on chart) lock
  QPushButton* pqPushButtonCenterLock;
  /// [UI:Button] Edit
  QPushButton* pqPushButtonEdit;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  // /// [UI:Button] Add new landmark
  // QPushButton* pqPushButtonAddLandmark;
  // /// [UI:Button] Add new (route) waypoint
  // QPushButton* pqPushButtonAddRoute;
  /// [UI:Button] Track recording status
  QPushButton* pqPushButtonTrackRecord;
  /// [UI:Button] Add new device
  QPushButton* pqPushButtonAddDevice;

  /// Flag that disables checkable buttons update
  bool bIgnoreUpdate;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPointDetailView( QWidget* _pqParent = 0 );
  virtual ~CVesselPointDetailView() {};

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
  /// [UI:Slot] Slot to enable center lock (on this vessel)
  void slotPositionCenterLock( bool _bLock );
  /// [UI:Slot] Slot to display edit view
  void slotEdit();
  /// [UI:Slot] Slot to delete (this item/point)
  void slotDelete();
  /// [UI:Slot] Slot to switch track recording status
  void slotTrackRecord( bool _bTrackRecord );
  /// [UI:Slot] Slot to add new device
  void slotAddDevice();

};

#endif // QVCT_CVESSELITEMDETAILVIEW_HPP
