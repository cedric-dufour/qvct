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

#ifndef QVCT_CVESSELCONTAINERDEVICEEDITVIEW_HPP
#define QVCT_CVESSELCONTAINERDEVICEEDITVIEW_HPP

// QT
#include <QSpinBox>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectEditView.hpp"
#include "overlays/COverlayText.hpp"
class CVesselContainerDevice;


/// [UI] Vessel container's device edit view
/**
 *  This class implements the user-interface that allows to edit vessel
 *  container's device.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselContainerDeviceEditView: public COverlayObjectEditView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Label] Name
  COverlayText* poTextName;
  /// [UI:SpinBox] (Vessels) Time-to-live
  QSpinBox* pqSpinBoxTTL;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselContainerDeviceEditView( CVesselContainerDevice* _poVesselContainerDevice );
  virtual ~CVesselContainerDeviceEditView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QDialog (override)
  //------------------------------------------------------------------------------

public slots:
  virtual void accept();

};

#endif // QVCT_CVESSELCONTAINERDEVICEEDITVIEW_HPP
