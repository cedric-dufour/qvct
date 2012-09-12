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

#ifndef QVCT_COVERLAYURL_HPP
#define QVCT_COVERLAYURL_HPP

// QT
#include <QWidget>

// QVCT
#include "overlays/COverlayText.hpp"


/// [UI] Overlay-specific URL label
/**
 *  This class provides the widget that allows the display an URL and make it
 *  clickable if it is valid.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayUrl: public COverlayText
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  COverlayUrl( QWidget* _pqParent );
  COverlayUrl( const QString& _rqsText, QWidget* _pqParent );
  COverlayUrl( const QString& _rqsText, const QString& _rqsLink, QWidget* _pqParent );
  virtual ~COverlayUrl() {};


  //------------------------------------------------------------------------------
  // METHODS: QLabel (override)
  //------------------------------------------------------------------------------

public:
  void setText( const QString& _rqsText );


  //------------------------------------------------------------------------------
  // METHODS: COverlayText (override)
  //------------------------------------------------------------------------------

public:
  virtual void setText( const QString& _rqsText, bool _bInvalid );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the underlying QLabel's text
  /** This methods sets the underlying QLabel's text and makes it clickable if
   *  the provided corresponding link is valid. */
  virtual void setText( const QString& _rqsText, const QString& _rqsLink, bool _bInvalid );

};

#endif // QVCT_COVERLAYURL_HPP
