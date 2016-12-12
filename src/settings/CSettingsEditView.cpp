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

// QT
#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

// QVCT
#include "settings/CSettingsEditView.hpp"
#include "settings/CSettingsMiscEditView.hpp"
#include "settings/CSettingsUnitsEditView.hpp"
#include "settings/CSettingsValidityEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CSettingsEditView::CSettingsEditView( QWidget* _pqParent )
  : QDialog( _pqParent )
{
  QDialog::setWindowTitle( tr("Settings")+"..." );
  constructLayout();
}

void CSettingsEditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add settings tabs
  // ? BUG ? If (child) widget parent is not set before addTab, QDialog size is messed up
  QTabWidget* __pqTabWidget = new QTabWidget();
  // ... units
  __pqTabWidget->addTab( new CSettingsUnitsEditView( __pqTabWidget ), tr("Units") );
  // ... validity
  __pqTabWidget->addTab( new CSettingsValidityEditView( __pqTabWidget ), tr("Validity") );
  // ... misc. options
  __pqTabWidget->addTab( new CSettingsMiscEditView( __pqTabWidget ), tr("Misc.") );
  __pqVBoxLayout->addWidget( __pqTabWidget );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok, Qt::Horizontal );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  QDialog::setLayout( __pqVBoxLayout );
}
