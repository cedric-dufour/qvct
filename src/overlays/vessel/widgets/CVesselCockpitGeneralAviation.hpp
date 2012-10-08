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

#ifndef QVCT_CVESSELCOCKPITGENERALAVIATION_HPP
#define QVCT_CVESSELCOCKPITGENERALAVIATION_HPP

// QT
#include <QWidget>

// QVCT
#include "overlays/vessel/widgets/CVesselCockpit.hpp"
class CInstrumentGAAltimeter;
class CInstrumentGAArtificialHorizon;
class CInstrumentGAChronograph;
class CInstrumentGAHorizontalSituation;
class CInstrumentGASpeed;
class CInstrumentGASpeedVertical;

/// [UI] General aviation cockpit view (dock widget)
/**
 *  This class implement the general aviation "Basic T" cockpit view, including
 *  speed indicator, artificial horizon, altimeter, watch, horizontal situation
 *  indicator (HSI) and and climb rate indicator.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselCockpitGeneralAviation: public CVesselCockpit
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Instrument: horizontal speed indicator (GS)
  CInstrumentGASpeed* poSpeed;
  /// Instrument: artificial horizon (AH)
  CInstrumentGAArtificialHorizon* poArtificialHorizon;
  /// Instrument: altimeter (ALT)
  CInstrumentGAAltimeter* poAltimeter;
  /// Instrument: chronograph (TIME)
  CInstrumentGAChronograph* poChronograph;
  /// Instrument: horizontal situation indicator (HSI)
  CInstrumentGAHorizontalSituation* poHorizontalSituation;
  /// Instrument: vertical speed indicator (VSI)
  CInstrumentGASpeedVertical* poSpeedVertical;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselCockpitGeneralAviation( QWidget* _pqParent );
  virtual ~CVesselCockpitGeneralAviation() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: CVesselWidget (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void refreshContent();
  virtual void resetContent();

};

#endif // QVCT_CVESSELCOCKPITGENERALAVIATION_HPP
