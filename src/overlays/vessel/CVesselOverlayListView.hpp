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

#ifndef QVCT_CVESSELOVERLAYLISTVIEW_HPP
#define QVCT_CVESSELOVERLAYLISTVIEW_HPP

// QT
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

// QVCT
class CVesselOverlay;


/// [UI] Vessel overlay's list view
/**
 *  This class implements the user-interface that allows to store and display
 *  the vessel overlay.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselOverlayListView: public QWidget
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Vessel overlay
  CVesselOverlay* poVesselOverlay;

  /// [UI:Button] Load
  QPushButton* pqPushButtonLoad;
  /// [UI:Button] Add
  QPushButton* pqPushButtonAdd;
  /// [UI:Button] Load
  QPushButton* pqPushButtonSave;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Up
  QPushButton* pqPushButtonUp;
  /// [UI:Button] Down
  QPushButton* pqPushButtonDown;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselOverlayListView( QWidget* _pqParent );
  virtual ~CVesselOverlayListView() {};

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
  /// [UI:Slot] Slot to save overlay's selected content to file
  void slotSave();
  /// [UI:Slot] Slot to delete overlay's selected content
  void slotDelete();
  /// [UI:Slot] Slot to sort or move overlay's content (ascending/up direction)
  void slotUp();
  /// [UI:Slot] Slot to sort or move overlay's content (descending/down direction)
  void slotDown();

};

#endif // QVCT_CVESSELOVERLAYLISTVIEW_HPP
