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

#ifndef QVCT_CROUTEITEMDETAILVIEW_HPP
#define QVCT_CROUTEITEMDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CRoutePoint;
class COverlayText;
class COverlayUrl;


/// [UI] Route item's detail view
/**
 *  This class implements the user-interface that allows to display route
 *  items' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CRoutePointDetailView: public COverlayObjectDetailView
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
  /// [UI:Button] Move (change position)
  QPushButton* pqPushButtonMove;
  /// [UI:Button] Edit
  QPushButton* pqPushButtonEdit;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Add new landmark
  QPushButton* pqPushButtonAddLandmark;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CRoutePointDetailView( QWidget* _pqParent );
  virtual ~CRoutePointDetailView() {};

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
  /// [UI:Slot] Slot to enable/disable move (position change)
  void slotMove( bool _bEnable );
  /// [UI:Slot] Slot to display edit view
  void slotEdit();
  /// [UI:Slot] Slot to delete (this item/point)
  void slotDelete();
  /// [UI:Slot] Slot to add new landmark
  void slotAddLandmark();

};

#endif // QVCT_CROUTEITEMDETAILVIEW_HPP
