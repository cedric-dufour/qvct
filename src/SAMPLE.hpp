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

#ifndef QVCT_CSAMPLE_HPP
#define QVCT_CSAMPLE_HPP

// QT
#include <QWidget>

// QVCT
#include "QVCT.hpp"


/// Brief description
/**
 * Long description
 * @author Cedric Dufour <http://cedric.dufour.name>
 */
class CSample: public QWidget
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:


  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  static const int CONSTANT;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CSample( QWidget* _pqParent = 0 );
  virtual ~CSample() {};

private:
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: CParentClass (override/implement)
  //------------------------------------------------------------------------------

public:
protected:
private:


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:

  // SETTERS
public:

  // GETTERS
public:

  // USERS
public:

  // OTHER
public:

};

#endif // QVCT_CSAMPLE_HPP
