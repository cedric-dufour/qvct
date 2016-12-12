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

#ifndef QVCT_CSETTINGSUNITSEDIT_HPP
#define QVCT_CSETTINGSUNITSEDIT_HPP

// QT
#include <QWidget>


/// [UI] Applciation settings - units tab
/**
 *  This class provides the wdiget that allows the modify the units settings
 *  within the application's settings dialog.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CSettingsUnitsEditView: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CSettingsUnitsEditView( QWidget* _pqParent = 0 );
  virtual ~CSettingsUnitsEditView() {};

private:
  /// Constructs the layout of the user interface
  void constructLayout();

};

#endif // QVCT_CSETTINGSUNITSEDIT_HPP
