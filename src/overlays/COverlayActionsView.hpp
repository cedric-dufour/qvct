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

#ifndef QVCT_COVERLAYACTIONSVIEW_HPP
#define QVCT_COVERLAYACTIONSVIEW_HPP

// QT
#include <QComboBox>
#include <QDialog>


/// [UI] Generic overlay (additional) actions view
/**
 *  This class defines the base user-interface required to display additional
 *  available actions.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayActionsView: public QDialog
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  COverlayActionsView();
  virtual ~COverlayActionsView() {};

};

#endif // QVCT_COVERLAYACTIONSVIEW_HPP
