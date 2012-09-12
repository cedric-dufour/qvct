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
#include <QUrl>
#include <QWidget>

// QVCT
#include "overlays/COverlayUrl.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

COverlayUrl::COverlayUrl( QWidget* _pqParent )
  : COverlayText( _pqParent )
{}

COverlayUrl::COverlayUrl( const QString& _rqsText, QWidget* _pqParent )
  : COverlayText( _rqsText, _pqParent )
{}

COverlayUrl::COverlayUrl( const QString& _rqsText, const QString& _rqsLink, QWidget* _pqParent )
  : COverlayText( _pqParent )
{
  setText( _rqsText, _rqsLink, !_rqsText.length() );
}


//------------------------------------------------------------------------------
// METHODS: QLabel (override)
//------------------------------------------------------------------------------

void COverlayUrl::setText( const QString& _rqsText )
{
  setText( _rqsText, _rqsText, !_rqsText.length() );
}


//------------------------------------------------------------------------------
// METHODS: COverlayText (override)
//------------------------------------------------------------------------------

//
// SETTERS
//

void COverlayUrl::setText( const QString& _rqsText, bool _bInvalid )
{
  setText( _rqsText, _rqsText, _bInvalid );
}

void COverlayUrl::setText( const QString& _rqsText, const QString& _rqsLink, bool _bInvalid )
{
  QFont __qFont = QLabel::font();
  __qFont.setItalic( _bInvalid );
  QLabel::setFont( __qFont );
  QUrl __qUrl( _rqsLink );
  if( _rqsText.length() && __qUrl.isValid() )
  {
    QLabel::setOpenExternalLinks( true );
    QLabel::setText( "<A HREF=\""+__qUrl.toString()+"\">"+_rqsText+"</A>" );
  }
  else
  {
    QLabel::setOpenExternalLinks( false );
    QLabel::setText( _rqsText.length() ? _rqsText : emptyString() );
  }
}
