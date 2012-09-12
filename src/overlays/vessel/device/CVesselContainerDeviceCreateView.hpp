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

#ifndef QVCT_CVESSELCONTAINERDEVICECREATEVIEW_HPP
#define QVCT_CVESSELCONTAINERDEVICECREATEVIEW_HPP

// QT
#include <QDialog>
#include <QLineEdit>

// QVCT
class CVesselContainerDevice;


/// [UI] Generic overlay object's pick (select) view
/**
 *  This class defines the base user-interface required to create a new
 *  device among the available device drivers.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselContainerDeviceCreateView: public QDialog
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [out] Created VesselContainerDevice
  CVesselContainerDevice** ppoVesselContainerDevice;
  /// [UI:LineEdit] Device name
  QLineEdit* pqLineEditName;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  /** @param[out] _ppoVesselContainerDevice Created vessel's device */
  CVesselContainerDeviceCreateView( CVesselContainerDevice** _ppoVesselContainerDevice );
  virtual ~CVesselContainerDeviceCreateView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QDialog (override)
  //------------------------------------------------------------------------------

public slots:
  /// [override] QDialog::accept()
  virtual void accept();

};

#endif // QVCT_CVESSELCONTAINERDEVICECREATEVIEW_HPP
