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

#ifndef QVCT_CVESSELPOINTDEVICEDETAILVIEW_HPP
#define QVCT_CVESSELPOINTDEVICEDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CVesselPointDevice;
class COverlayText;
class COverlayUrl;


/// [UI] Vessel point's detail view
/**
 *  This class implements the user-interface that allows to display vessel
 *  point devices' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPointDeviceDetailView: public COverlayObjectDetailView
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
  /// [UI:Label] Bearing
  COverlayText* poTextBearing;
  /// [UI:Label] Horizontal speed
  COverlayText* poTextSpeed;
  /// [UI:Label] Vertical speed
  COverlayText* poTextSpeedVertical;
  /// [UI:Label] Date
  COverlayText* poTextDate;
  /// [UI:Label] Time
  COverlayText* poTextTime;
  /// [UI:Label] Time (alternate timezone)
  COverlayText* poTextTime2;
  /// [UI:Label] Fix type
  COverlayText* poTextFixType;
  /// [UI:Label] Sources (used/seen)
  COverlayText* poTextSources;
  /// [UI:Label] DOP
  COverlayText* poTextDop;
  /// [UI:Label] Additional textual data string
  COverlayText* poText;
  /// [UI:Button] Edit
  QPushButton* pqPushButtonEdit;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Connect (device)
  QPushButton* pqPushButtonConnect;

  /// Flag that disables checkable buttons update
  bool bIgnoreUpdate;

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPointDeviceDetailView( QWidget* _pqParent );
  virtual ~CVesselPointDeviceDetailView() {};

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
  /// [UI:Slot] Slot to refresh the view content
  void slotRefreshContent();
  /// [UI:Slot] Slot to display edit view
  void slotEdit();
  /// [UI:Slot] Slot to delete (this point/point)
  void slotDelete();
  /// [UI:Slot] Slot to connect (to the actual device)
  void slotConnect( bool );

};

#endif // QVCT_CVESSELPOINTDEVICEDETAILVIEW_HPP
