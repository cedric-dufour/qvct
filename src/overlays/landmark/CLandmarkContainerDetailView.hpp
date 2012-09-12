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

#ifndef QVCT_CLANDMARKCONTAINERDETAILVIEW_HPP
#define QVCT_CLANDMARKCONTAINERDETAILVIEW_HPP

// QT
#include <QLabel>
#include <QPushButton>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectDetailView.hpp"
class CLandmarkContainer;
class COverlayText;


/// [UI] Landmark container's detail view
/**
 *  This class implements the user-interface that allows to display landmark
 *  containers' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CLandmarkContainerDetailView: public COverlayObjectDetailView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
  /// [UI:Label] Content (items summary)
  COverlayText* poTextContent;
  /// [UI:Button] Visibility status
  QPushButton* pqPushButtonVisible;
  /// [UI:Button] Center (on chart)
  QPushButton* pqPushButtonCenter;
  /// [UI:Button] Save
  QPushButton* pqPushButtonSave;
  /// [UI:Button] Edit
  QPushButton* pqPushButtonEdit;
  /// [UI:Button] Delete
  QPushButton* pqPushButtonDelete;
  /// [UI:Button] Add new (landmark) point
  QPushButton* pqPushButtonAddPoint;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CLandmarkContainerDetailView( QWidget* _pqParent );
  virtual ~CLandmarkContainerDetailView() {};

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
  /// [UI:Slot] Slot to center chart (on container's items position)
  void slotPositionCenter();
  /// [UI:Slot] Slot to save this container to file
  void slotSave();
  /// [UI:Slot] Slot to display edit view
  void slotEdit();
  /// [UI:Slot] Slot to delete (this container)
  void slotDelete();
  /// [UI:Slot] Slot to add new (landmark) point
  void slotAddPoint();
  
};

#endif // QVCT_CLANDMARKCONTAINERDETAILVIEW_HPP
