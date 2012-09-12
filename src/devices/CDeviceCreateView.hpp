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

#ifndef QVCT_CDEVICECREATEVIEW_HPP
#define QVCT_CDEVICECREATEVIEW_HPP

// QT
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>

// QVCT
class CDevice;


/// [UI] Generic overlay object's pick (select) view
/**
 *  This class defines the base user-interface required to pick (select) an
 *  overlay object among the available ones (in the parent object).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceCreateView: public QDialog
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [out] Created device
  CDevice** ppoDevice;
  /// [UI:LineEdit] Device name
  QLineEdit* pqLineEditName;
  /// [UI:ComboBox] Device driver
  QComboBox* pqComboBoxDriver;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  /** @param[out] _ppoDevice Created device */
  CDeviceCreateView( CDevice** _ppoDevice );
  virtual ~CDeviceCreateView() {};

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

#endif // QVCT_CDEVICECREATEVIEW_HPP
