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
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMap>
#include <QVBoxLayout>

// QVCT
#include "devices/CDeviceCreateView.hpp"
#include "devices/drivers/CDeviceDriver.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDeviceCreateView::CDeviceCreateView( CDevice** _ppoDevice )
  : QDialog( 0 )
  , ppoDevice( _ppoDevice )
  , pqLineEditName( 0 )
  , pqComboBoxDriver( 0 )
{
  QDialog::setWindowTitle( tr("Create Device")+"..." );
  *ppoDevice = 0;
  constructLayout();
}

void CDeviceCreateView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add form
  QHBoxLayout* __pqHBoxLayout = new QHBoxLayout();
  // ... icon
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/device.png" ) );
  __pqHBoxLayout->addWidget( __pqLabelIcon, 0 );
  // ... form
  QFormLayout* __pqFormLayout = new QFormLayout();
  // ... name
  pqLineEditName = new QLineEdit( this );
  __pqFormLayout->addRow( tr("Name")+":", pqLineEditName );
  // ... driver
  pqComboBoxDriver = new QComboBox( this );
  QList<QString> __qListDriverName = CDeviceDriver::names().values();
  int __iCount = __qListDriverName.count();
  for( int __i = 0; __i < __iCount; __i++ ) pqComboBoxDriver->addItem( __qListDriverName.at( __i ) );
  __pqFormLayout->addRow( tr("Driver")+":", pqComboBoxDriver );
  // ... [end]
  __pqHBoxLayout->addLayout( __pqFormLayout, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Ok, Qt::Horizontal, this );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  QDialog::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CDeviceCreateView::accept()
{
  *ppoDevice = CDeviceDriver::createDevice( pqLineEditName->text(), CDeviceDriver::fromName( pqComboBoxDriver->currentText() ) );
  QDialog::accept();
}
