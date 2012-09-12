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

#ifndef QVCT_COVERLAYOBJECTEDITVIEW_HPP
#define QVCT_COVERLAYOBJECTEDITVIEW_HPP

// QT
#include <QDialog>

// QVCT
class COverlayObject;


/// [UI] Generic overlay object's edit view
/**
 *  This class defines the base user-interface required to edit an overlay
 *  object.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayObjectEditView: public QDialog
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Overlay object being edited
  /** @see COverlayObjectEditView() */
  const COverlayObject* poOverlayObject;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayObjectEditView( COverlayObject* _poOverlayObject );
  virtual ~COverlayObjectEditView() {};

};

#endif // QVCT_COVERLAYOBJECTEDITVIEW_HPP
