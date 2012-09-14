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
#include <QString>

// QVCT
#include "QVCTRuntime.hpp"
#include "devices/CDevice.hpp"
#include "overlays/device/CDeviceOverlay.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CDevice::CDevice( const QString& _rqsName )
  : COverlayItem( COverlayObject::ITEM, _rqsName )
{
  QTreeWidgetItem::setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
  QTreeWidgetItem::setText( CDeviceOverlay::NAME, qsName );
  QTreeWidgetItem::setCheckState( CDeviceOverlay::SELECT, Qt::Unchecked );
  QObject::connect( this, SIGNAL( signalError(const QString&) ), QVCTRuntime::useMainWindow(), SLOT( slotError(const QString&) ) );
}
