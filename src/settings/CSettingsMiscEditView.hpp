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

#ifndef QVCT_CSETTINGSMISCEDIT_HPP
#define QVCT_CSETTINGSMISCEDIT_HPP

// QT
#include <QWidget>


/// [UI] Application settings - misc tab
/**
 *  This class provides the widget that allows the modify miscellaneous settings
 *  within the application's settings dialog.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CSettingsMiscEditView: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CSettingsMiscEditView( QWidget* _pqParent );
  virtual ~CSettingsMiscEditView() {};

private:
  /// Constructs the layout of the user interface
  void constructLayout();

};

#endif // QVCT_CSETTINGSMISCEDIT_HPP
