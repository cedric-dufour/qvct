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

#ifndef QVCT_COVERLAYOBJECTDETAILVIEW_HPP
#define QVCT_COVERLAYOBJECTDETAILVIEW_HPP

// QT
#include <QWidget>

// QVCT
class COverlayObject;


/// [UI] Generic overlay object's detail view
/**
 *  This class defines the base user-interface required to display an overlay
 *  object's details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayObjectDetailView: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Overlay object being displayed
  /** @see setOverlayObject(), resetOverlayObject() */
  COverlayObject* poOverlayObject;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayObjectDetailView( QWidget* _pqParent = 0 );
  virtual ~COverlayObjectDetailView() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to handle object destruction
  void slotDestroyed( QObject* _pqObject );

  // SETTERS
public:
  /// Sets the overlay object to be displayed (and refreshes the underlying widget)
  void setOverlayObject( COverlayObject* _poOverlayObject );
  /// Resets (clears) the overlay object being displayed (and clears the underlying widget)
  void resetOverlayObject();

  // OTHER
public:
  /// Refreshes the content of the underlying widget
  virtual void refreshContent() = 0;
  /// Resets (clears) the content of the underlying widget
  virtual void resetContent() = 0;

protected:
  /// Enables the content (controls) of the underlying widget
  virtual void enableContent() {};
  /// Disables the content (controls) of the underlying widget
  virtual void disableContent() {};

};

#endif // QVCT_COVERLAYOBJECTDETAILVIEW_HPP
