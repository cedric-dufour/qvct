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

#ifndef QVCT_CVESSELPOSITION_HPP
#define QVCT_CVESSELPOSITION_HPP

// QT
#include <QDockWidget>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/CVesselWidgetAdaptive.hpp"
class COverlayText;


/// [UI] Vessel position view (dock widget)
/**
 *  This class provides the user-interface (QDockWidget) that allows to display
 *  a vessel point's live position data.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPosition: public CVesselWidgetAdaptive
{


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Longitude
  COverlayText* poTextLongitude;
  /// [UI:Label] Latitude
  COverlayText* poTextLatitude;
  /// [UI:Label] Elevation
  COverlayText* poTextElevation;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPosition( QWidget* _pqParent );
  virtual ~CVesselPosition() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: CVesselWidget (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void refreshContent();
  virtual void resetContent();


  //------------------------------------------------------------------------------
  // METHODS: CVesselWidgetAdaptive (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void setFont( QFont _qFont );

};

#endif // QVCT_CVESSELPOSITION_HPP
