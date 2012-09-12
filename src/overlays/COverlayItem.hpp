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

#ifndef QVCT_COVERLAYITEM_HPP
#define QVCT_COVERLAYITEM_HPP

// QT
#include <QObject>

// QVCT
#include "overlays/COverlayObject.hpp"


/// Generic overlay item
/**
 *  This class defines the generic representation of an item used in an
 *  overlay.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayItem: public COverlayObject
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// Item's selection status
  /** @see setMultiSelected(), isMultiSelected() */
  bool bMultiSelected;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayItem( COverlayObject::EType _eType, const QString& _rqsName );
  virtual ~COverlayItem() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets this item's selection status
  void setMultiSelected( bool _bMultiSelected ) { bMultiSelected = _bMultiSelected; };

  // GETTERS
public:
  /// Returns this item's selection status
  bool isMultiSelected() const { return bMultiSelected; };

};

#endif // QVCT_COVERLAYITEM_HPP
