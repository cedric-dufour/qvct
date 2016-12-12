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
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

// QVCT
#include "overlays/vessel/device/CVesselPointDevice.hpp"
#include "overlays/vessel/device/CVesselPointDeviceCreateView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointDeviceCreateView::CVesselPointDeviceCreateView( CVesselPointDevice** _ppoVesselPointDevice )
  : QDialog( 0 )
  , ppoVesselPointDevice( _ppoVesselPointDevice )
  , pqLineEditName( 0 )
  , pqLineEditSourceName( 0 )
{
  QDialog::setWindowTitle( tr("Create Device")+"..." );
  *ppoVesselPointDevice = 0;
  constructLayout();
}

void CVesselPointDeviceCreateView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout();

  // Add form
  QHBoxLayout* __pqHBoxLayout = new QHBoxLayout();
  // ... icon
  QLabel* __pqLabelIcon = new QLabel();
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_device.png" ) );
  __pqHBoxLayout->addWidget( __pqLabelIcon, 0 );
  // ... form
  QFormLayout* __pqFormLayout = new QFormLayout();
  // ... name
  pqLineEditName = new QLineEdit();
  pqLineEditName->setToolTip( tr("Device name (matching one of the defined devices)") );
  __pqFormLayout->addRow( tr("Device")+":", pqLineEditName );
  // ... source name
  pqLineEditSourceName = new QLineEdit();
  pqLineEditSourceName->setToolTip( tr("Source name/filter (exactly matching the device provided source name)") );
  __pqFormLayout->addRow( tr("Source")+":", pqLineEditSourceName );
  // ... [end]
  __pqHBoxLayout->addLayout( __pqFormLayout, 1 );
  __pqVBoxLayout->addLayout( __pqHBoxLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Ok, Qt::Horizontal );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  QDialog::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CVesselPointDeviceCreateView::accept()
{
  *ppoVesselPointDevice = new CVesselPointDevice( pqLineEditName->text(), pqLineEditSourceName->text() );
  QDialog::accept();
}
