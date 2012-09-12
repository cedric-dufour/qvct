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

#ifndef QVCT_CDEVICEDATASOURCE_HPP
#define QVCT_CDEVICEDATASOURCE_HPP


/// Data source details [name,type]
/**
 *  This class gathers all details of a generic navigational data source.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CDeviceDataSource
{

  //------------------------------------------------------------------------------
  // CONSTANTS / STATIC
  //------------------------------------------------------------------------------

public:
  /// Data source type
  enum EType { UNDEFINED, GPS, AIS };


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Source name
  /** @see getName() */
  QString qsName;
  /// Source type
  /** @see setType(), getType() */
  EType eType;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CDeviceDataSource( const QString& _rqsName );
  virtual ~CDeviceDataSource() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SETTERS
public:
  /// Sets the source type
  void setSourceType( EType _eType ) { eType = _eType; };

  // GETTERS
public:
  /// Returns the source name
  QString getSourceName() const { return qsName; };
  /// Returns the source type
  EType getSourceType() const { return eType; };

};

#endif // QVCT_CDEVICEDATASOURCE_HPP
