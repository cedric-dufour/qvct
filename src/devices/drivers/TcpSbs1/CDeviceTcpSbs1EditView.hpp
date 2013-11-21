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

#ifndef QVCT_CDEVICETCPSBS1EDITVIEW_HPP
#define QVCT_CDEVICETCPSBS1EDITVIEW_HPP

// QT
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectEditView.hpp"
class CDeviceTcpSbs1;


/// [UI] Route container's edit view
/**
 *  This class implements the user-interface that allows to edit route
 *  containers.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceTcpSbs1EditView: public COverlayObjectEditView
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:LineEdit] Host
  QLineEdit* pqLineEditHost;
  /// [UI:LineEdit] Port
  QLineEdit* pqLineEditPort;
  /// [UI:ComboBox] Time zone
  QComboBox* pqComboBoxTimeZone;
  /// [UI:CheckBox] Ground traffic
  QCheckBox* pqCheckBoxGroundTraffic;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceTcpSbs1EditView( CDeviceTcpSbs1* _poDeviceTcpSbs1 );
  virtual ~CDeviceTcpSbs1EditView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QDialog (override)
  //------------------------------------------------------------------------------

public slots:
  virtual void accept();

};

#endif // QVCT_CDEVICETCPSBS1EDITVIEW_HPP
