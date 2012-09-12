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

#ifndef QVCT_CDEVICEDATASKYVIEW_HPP
#define QVCT_CDEVICEDATASKYVIEW_HPP

// QT
#include <QList>

// QVCT
#include "data/CDataTime.hpp"
#include "devices/data/CDeviceDataDop.hpp"
#include "devices/data/CDeviceDataSatellite.hpp"
#include "devices/data/CDeviceDataSource.hpp"


/// Sky view data (satellites details)
/**
 *  This class gathers all the details of visible satellites.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDataSkyView: public CDeviceDataSource, public CDataTime, public QList<CDeviceDataSatellite>, public CDeviceDataDop
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CDeviceDataSkyView( const QString& _rqsSourceName );
  virtual ~CDeviceDataSkyView() {};

};

#endif // QVCT_CDEVICEDATASKYVIEW_HPP
