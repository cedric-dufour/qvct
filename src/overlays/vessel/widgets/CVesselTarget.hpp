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

#ifndef QVCT_CVESSELTARGET_HPP
#define QVCT_CVESSELTARGET_HPP

// QT
#include <QDockWidget>
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/CVesselWidgetAdaptive.hpp"
class COverlayText;


/// [UI] Vessel target view (dock widget)
/**
 *  This class provides the user-interface (QDockWidget) that allows to display
 *  a vessel point's live target data (bearing, distance, ETE).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselTarget: public CVesselWidgetAdaptive
{


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Flag to track whether content data are currently displayed
  bool bContentDisplayed;

  /// [UI:Label] Bearing
  COverlayText* poTextBearing;
  /// [UI:Label] Distance
  COverlayText* poTextDistance;
  /// [UI:Label] Estimated Time En-Route (ETE)
  COverlayText* poTextEte;
  /// [UI:Label] Estimated Time of Arrival (ETA)
  COverlayText* poTextEta;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselTarget( QWidget* _pqParent );
  virtual ~CVesselTarget() {};

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

#endif // QVCT_CVESSELTARGET_HPP
