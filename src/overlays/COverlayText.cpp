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
#include <QLabel>
#include <QWidget>

// QVCT
#include "overlays/COverlayText.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayText::COverlayText( QWidget* _pqParent )
  : QLabel( _pqParent )
{}

COverlayText::COverlayText( const QString& _rqsText, QWidget* _pqParent )
  : QLabel( _pqParent )
{
  setText( _rqsText, !_rqsText.length() );
}

//------------------------------------------------------------------------------
// METHODS: QLabel (override)
//------------------------------------------------------------------------------

void COverlayText::setText( const QString& _rqsText )
{
  setText( _rqsText, !_rqsText.length() );
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void COverlayText::setText( const QString& _rqsText, bool _bInvalid )
{
  QFont __qFont = QLabel::font();
  __qFont.setItalic( _bInvalid );
  QLabel::setFont( __qFont );
  QLabel::setText( _rqsText.length() ? _rqsText : emptyString() );
}

void COverlayText::resetText()
{
  QFont __qFont = QLabel::font();
  __qFont.setItalic( true );
  QLabel::setFont( __qFont );
  QLabel::setText( emptyString() );
}

//
// OTHER
//

const QString& COverlayText::emptyString()
{
  static const QString __qsEmpty( tr("n/a") );
  return __qsEmpty;
}

QString COverlayText::formatString( const QString& _rqsString )
{
  return( _rqsString.length() ? _rqsString : emptyString() );
}
