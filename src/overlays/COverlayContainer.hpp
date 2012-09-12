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

#ifndef QVCT_COVERLAYCONTAINER_HPP
#define QVCT_COVERLAYCONTAINER_HPP

// QT
#include <QObject>

// QVCT
#include "overlays/COverlayObject.hpp"
class COverlayPoint;


/// Generic overlay container
/**
 *  This class defines the generic representation of an intermediate container
 *  (any level between base and items) of overlay objects
 *  @see COverlay
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayContainer: public COverlayObject
{
  // Q_OBJECT // required for 'tr' support

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayContainer( COverlayObject::EType _eType, const QString& _rqsName );
  virtual ~COverlayContainer() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // OTHER
public:
  /// Returns the overlay container's point that (first) matches the given screen position (0 if none is found)
  virtual COverlayPoint* matchScrPosition( const CChart* _poChart, const QPointF& _rqPointFScrPosition ) const = 0;

};

#endif // QVCT_COVERLAYCONTAINER_HPP
