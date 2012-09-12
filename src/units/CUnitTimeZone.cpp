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
#include <QMap>
#include <QString>

// QVCT
#include "units/CUnitTimeZone.hpp"


//------------------------------------------------------------------------------
// CONSTANTS / STATIC
//------------------------------------------------------------------------------

const CUnitTimeZoneSymbols CUnitTimeZone::oUnitTimeZoneSymbols;
const CUnitTimeZoneCodes CUnitTimeZone::oUnitTimeZoneCodes;

const QMap<CUnitTimeZone::EUnit,QString> &CUnitTimeZone::symbols()
{
  return oUnitTimeZoneSymbols.qMapSymbols;
}

QString CUnitTimeZone::toSymbol( CUnitTimeZone::EUnit _eUnit )
{
  return oUnitTimeZoneSymbols.qMapSymbols.value( _eUnit, "?" );
}

CUnitTimeZone::EUnit CUnitTimeZone::fromSymbol( const QString& _rqString )
{
  return oUnitTimeZoneSymbols.qMapSymbols.key( _rqString, UNDEFINED );
}

const QMap<CUnitTimeZone::EUnit,QString> &CUnitTimeZone::codes()
{
  return oUnitTimeZoneCodes.qMapCodes;
}

QString CUnitTimeZone::toCode( CUnitTimeZone::EUnit _eUnit )
{
  return oUnitTimeZoneCodes.qMapCodes.value( _eUnit, "undef" );
}

CUnitTimeZone::EUnit CUnitTimeZone::fromCode( const QString& _rqString )
{
  return oUnitTimeZoneCodes.qMapCodes.key( _rqString, UNDEFINED );
}
