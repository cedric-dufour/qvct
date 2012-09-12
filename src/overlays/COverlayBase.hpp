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

#ifndef QVCT_COVERLAYBASE_HPP
#define QVCT_COVERLAYBASE_HPP

// QT
#include <QObject>

// QVCT
#include "overlays/COverlay.hpp"


/// Generic overlay base (non-widget)
/**
 *  This class defines the generic representation of an overlay (base object),
 *  devoid of corresponding QTreeWidget.
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayBase: public QObject, public COverlay
{
  Q_OBJECT // required for 'tr' support

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayBase( const QString& _rqsName );
  virtual ~COverlayBase() {};

};

#endif // QVCT_COVERLAYBASE_HPP
