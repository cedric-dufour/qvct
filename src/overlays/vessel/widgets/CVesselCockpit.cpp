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
#include <QGridLayout>
#include <QString>
#include <QWidget>

// QVCT
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/widgets/CVesselCockpit.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselCockpit::CVesselCockpit( const QString& _rqsTitle, QWidget* _pqParent )
  : QWidget( _pqParent, Qt::Window )
  , poVesselPoint( 0 )
{
  QWidget::setWindowTitle( _rqsTitle );
  pqGridLayout = new QGridLayout( this );
  pqGridLayout->setContentsMargins( 0, 0, 0, 0 );
  pqGridLayout->setSpacing( 0 );
  QWidget::setLayout( pqGridLayout );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SLOTS
//

void CVesselCockpit::slotDestroyed( QObject* _pqObject )
{
  if( !_pqObject || (QObject*)poVesselPoint != _pqObject ) return;
  resetVesselPoint();
}

//
// SETTERS
//

void CVesselCockpit::setVesselPoint( CVesselPoint* _poVesselPoint )
{
  if( poVesselPoint == _poVesselPoint ) return;
  poVesselPoint = _poVesselPoint;
  QObject::connect( poVesselPoint, SIGNAL( destroyed(QObject*) ), this, SLOT( slotDestroyed(QObject*) ) );
}

void CVesselCockpit::resetVesselPoint()
{
  if( poVesselPoint ) QObject::disconnect( (QObject*)poVesselPoint, 0, this, 0 );
  poVesselPoint = 0;
  resetContent();
}
