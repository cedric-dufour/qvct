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

#ifndef QVCT_CLANDMARKCONTAINERPICKVIEW_HPP
#define QVCT_CLANDMARKCONTAINERPICKVIEW_HPP

// QVCT
#include "overlays/COverlayObjectPickView.hpp"
class CLandmarkContainer;
class CLandmarkOverlay;


/// [UI] Landmark container's pick (select) view
/**
 *  This class implements the user-interface that allows to pick a landmark
 *  container among the available ones.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CLandmarkContainerPickView: public COverlayObjectPickView
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  /** @param[in] _poLandmarkOverlay Base overlay
   *  @param[out] _ppoLandmarkContainer Picked (selected) container */
  CLandmarkContainerPickView( const CLandmarkOverlay* _poLandmarkOverlay, COverlayObject** _ppoOverlayObject );
  virtual ~CLandmarkContainerPickView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();

};

#endif // QVCT_CLANDMARKCONTAINERPICKVIEW_HPP
