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

#ifndef QVCT_CDEVICEDETAILVIEW_HPP
#define QVCT_CDEVICEDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
#include "devices/CDevice.hpp"
class COverlayText;


/// [UI] device's detail view
/**
 *  This class implements the user-interface that allows to display devices'
 *  details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDetailView: public COverlayObjectDetailView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
  /// [UI:Label] Driver
  COverlayText* poTextDriver;
  /// [UI:Label] Activity
  QLabel* pqLabelActivity;
  /// [UI:Button] Edit
  QPushButton* pqPushButtonEdit;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Stop
  QPushButton* pqPushButtonStop;
  /// [UI:Button] Pause
  QPushButton* pqPushButtonPause;
  /// [UI:Button] Start
  QPushButton* pqPushButtonStart;

  /// Flag that disables checkable buttons update
  bool bIgnoreUpdate;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceDetailView( QWidget* _pqParent = 0 );
  virtual ~CDeviceDetailView() {};

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
  /// [Slot] Slot to handle operating mode changes
  void slotOperatingMode( CDevice::EOperatingMode _eOperatingMode );
  /// [Slot] Slot to handle activity signal emitted by device
  void slotActivity();
  /// [UI:Slot] Slot to edit the device configuration
  void slotEdit();
  /// [UI:Slot] Slot to delete the device
  void slotDelete();
  /// [UI:Slot] Slot to stop the device
  void slotStop();
  /// [UI:Slot] Slot to pause the device
  void slotPause();
  /// [UI:Slot] Start to start the device
  void slotStart();

};

#endif // QVCT_CDEVICEDETAILVIEW_HPP
