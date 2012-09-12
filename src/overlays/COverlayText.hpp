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

#ifndef QVCT_COVERLAYTEXT_HPP
#define QVCT_COVERLAYTEXT_HPP

// QT
#include <QLabel>
#include <QWidget>


/// [UI] Overlay-specific text label
/**
 *  This class provides the widget that allows the display text and handle
 *  undefined (empty) content gracefully.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class COverlayText: public QLabel
{

  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  COverlayText( QWidget* _pqParent );
  COverlayText( const QString& _rqsText, QWidget* _pqParent );
  virtual ~COverlayText() {};


  //------------------------------------------------------------------------------
  // METHODS: QLabel (override)
  //------------------------------------------------------------------------------

public:
  /// [override] QLabel::setText( const QString& )
  void setText( const QString& _rqsText );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the underlying QLabel's text
  /** This methods sets the underlying QLabel's text. If the text is undefined
   *  (empty), it displays a graceful equivalent content instead ("n/a" in English).
   *  If the content is deemed invalid by the user, it is displayed using italic font. */
  virtual void setText( const QString& _rqsText, bool _bInvalid );
  /// Resets (clears) the underlying QLabel's text
  void resetText();

  // OTHER
public:
  /// Returns the string corresponding to undefined (empty) content ("n/a" in English)
  const QString& emptyString();
  /// Returns the string properly formatted if it is undefined (empty)
  QString formatString( const QString& _rqsString );

};

#endif // QVCT_COVERLAYTEXT_HPP
