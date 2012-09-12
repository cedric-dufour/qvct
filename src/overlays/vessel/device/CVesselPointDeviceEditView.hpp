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

#ifndef QVCT_CVESSELPOINTDEVICEEDITVIEW_HPP
#define QVCT_CVESSELPOINTDEVICEEDITVIEW_HPP

// QT
#include <QCheckBox>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectEditView.hpp"
#include "overlays/COverlayText.hpp"
class CVesselPointDevice;


/// [UI] Vessel point's (item) edit view
/**
 *  This class implements the user-interface that allows to edit vessel
 *  points (items).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPointDeviceEditView: public COverlayObjectEditView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
  /// [UI:Label] Source name
  COverlayText* poTextSourceName;
  /// [UI:CheckBox] Synchronize position
  QCheckBox* pqCheckBoxPosition;
  /// [UI:CheckBox] Synchronize elevation
  QCheckBox* pqCheckBoxElevation;
  /// [UI:CheckBox] Synchronize time
  QCheckBox* pqCheckBoxTime;
  /// [UI:CheckBox] Synchronize ground bearing
  QCheckBox* pqCheckBoxGroundBearing;
  /// [UI:CheckBox] Synchronize ground horizontal speed
  QCheckBox* pqCheckBoxGroundSpeed;
  /// [UI:CheckBox] Synchronize ground vertical speed
  QCheckBox* pqCheckBoxGroundSpeedVertical;
  /// [UI:CheckBox] Synchronize apparent bearing
  QCheckBox* pqCheckBoxApparentBearing;
  /// [UI:CheckBox] Synchronize apparent horizontal speed
  QCheckBox* pqCheckBoxApparentSpeed;
  /// [UI:CheckBox] Synchronize apparent vertical speed
  QCheckBox* pqCheckBoxApparentSpeedVertical;
  /// [UI:CheckBox] Synchronize additional textual data string
  QCheckBox* pqCheckBoxText;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPointDeviceEditView( CVesselPointDevice* _poVesselPointDevice );
  virtual ~CVesselPointDeviceEditView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QDialog (override)
  //------------------------------------------------------------------------------

public slots:
  virtual void accept();

};

#endif // QVCT_CVESSELPOINTDEVICEEDITVIEW_HPP
