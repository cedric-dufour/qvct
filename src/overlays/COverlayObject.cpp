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
#include <QDataStream>
#include <QTreeWidgetItem>

// QVCT
#include "overlays/COverlayObject.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayObject::COverlayObject( COverlayObject::EType _eType, const QString& _rqsName )
  : QTreeWidgetItem( _eType )
  , qsName( _rqsName )
{}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// OTHER
//

void COverlayObject::serialize( QDataStream& _rqDataStream ) const
{
  _rqDataStream << qsName;
}

void COverlayObject::unserialize( QDataStream& _rqDataStream )
{
  _rqDataStream >> qsName;
}

QString COverlayObject::newChildName( const QString& _rqsName, int _iZeroPrefix, bool _bForceSuffix ) const
{
  QString __qsName;
  int __iSuffix = 0;
  bool __bValid;
  do
  {
    __qsName = _rqsName;
    if( _bForceSuffix || __iSuffix ) __qsName += "("+QString::number( __iSuffix+1 ).prepend( QString( _iZeroPrefix, '0' ) ).right( _iZeroPrefix+1 )+")";
    __bValid = true;
    for( int __i = QTreeWidgetItem::childCount()-1; __i >= 0; __i-- )
    {
      if( __qsName == ((COverlayObject*)QTreeWidgetItem::child( __i ))->getName() )
      {
        __bValid = false;
        __iSuffix++;
        break;
      }
    }
  }
  while( !__bValid );
  return __qsName;
}

QString COverlayObject::newChildName( const QString& _rqsName, int _iZeroPrefix, int _iSuffix ) const
{
  return _rqsName + "("+QString::number( _iSuffix ).prepend( QString( _iZeroPrefix, '0' ) ).right( _iZeroPrefix+1 )+")";
}
