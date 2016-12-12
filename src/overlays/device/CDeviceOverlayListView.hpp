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

#ifndef QVCT_CDEVICEOVERLAYLISTVIEW_HPP
#define QVCT_CDEVICEOVERLAYLISTVIEW_HPP

// QT
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

// QVCT
class CDeviceOverlay;


/// [UI] Device overlay's list view
/**
 *  This class implements the user-interface that allows to store and display
 *  the device overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceOverlayListView: public QWidget
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Device overlay
  CDeviceOverlay* poDeviceOverlay;

  /// [UI:Button] Load
  QPushButton* pqPushButtonLoad;
  /// [UI:Button] Add
  QPushButton* pqPushButtonAdd;
  /// [UI:Button] Up
  QPushButton* pqPushButtonUp;
  /// [UI:Button] Down
  QPushButton* pqPushButtonDown;
  /// [UI:Button] Actions
  QPushButton* pqPushButtonActions;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceOverlayListView( QWidget* _pqParent = 0 );
  virtual ~CDeviceOverlayListView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// [UI:Slot] Slot to load overlay's content from file
  void slotLoad();
  /// [UI:Slot] Slot to add a new container
  void slotAdd();
  /// [UI:Slot] Slot to move overlay's content up
  void slotUp();
  /// [UI:Slot] Slot to move overlay's content down
  void slotDown();
  /// [UI:Slot] Slot to show additional actions
  void slotActions();

};

#endif // QVCT_CDEVICEOVERLAYLISTVIEW_HPP
