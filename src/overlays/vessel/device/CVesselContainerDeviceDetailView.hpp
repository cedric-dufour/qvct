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

#ifndef QVCT_CVESSELCONTAINERDEVICEDETAILVIEW_HPP
#define QVCT_CVESSELCONTAINERDEVICEDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CVesselContainerDevice;
class COverlayText;
class COverlayUrl;


/// [UI] Vessel container's detail view
/**
 *  This class implements the user-interface that allows to display vessel
 *  container devices' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselContainerDeviceDetailView: public COverlayObjectDetailView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
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
  CVesselContainerDeviceDetailView( QWidget* _pqParent );
  virtual ~CVesselContainerDeviceDetailView() {};

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
  /// [UI:Slot] Slot to delete (this container/container)
  void slotDelete();
  /// [UI:Slot] Slot to connect (to the actual device)
  void slotConnect( bool );

};

#endif // QVCT_CVESSELCONTAINERDEVICEDETAILVIEW_HPP
