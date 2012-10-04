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

/**
 * \brief      Global/base application container
 * \author     Cedric Dufour <http://cedric.dufour.name>
 * \version    @version@
 */


#ifndef QVCT_QVCT_HPP
#define QVCT_QVCT_HPP

// QVCT
#include "main.hpp"


class QVCT
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  enum EStatus { OK = 0, WARNING = 1, ERROR = 2, CRITICAL = 3, UNDEFINED = 9 };
  enum EFileOperation { OPEN, SAVE };

public:
  static const double PI=3.1415926535897932384626433832795029;
  static const double DEG2RAD=0.0174532925199432957692369076848861271;
  static const double RAD2DEG=57.2957795130823208767981548141051703;

};

#endif // QVCT_QVCT_HPP
