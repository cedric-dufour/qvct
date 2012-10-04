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

#ifndef QVCT_CVESSELWIDGET_HPP
#define QVCT_CVESSELWIDGET_HPP

// QT
#include <QDockWidget>
#include <QString>
#include <QWidget>

// QVCT
class CVesselPoint;


/// [UI] Generic vessel (dock) widget
/**
 *  This class defines the base user-interface required to display vessels
 *  (dock) widgets.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselWidget: public QDockWidget
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// [UI:Widget] Container widget
  QWidget* pqWidget;

  /// Overlay course being displayed
  /** @see setVesselPoint(), resetVesselPoint() */
  CVesselPoint* poVesselPoint;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CVesselWidget( const QString& _qsTitle, QWidget* _pqParent );
  virtual ~CVesselWidget() {};


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to handle object destruction
  void slotDestroyed( QObject* _pqObject );

  // SETTERS
public:
  /// Sets the vessel used to synchronize the instruments display
  void setVesselPoint( CVesselPoint* _poVesselPoint );
  /// Resets (clears) the vessel used to synchronize the instruments display
  void resetVesselPoint();

  // OTHER
public:
  /// Refreshes the content of the underlying widget
  virtual void refreshContent() = 0;
  /// Resets (clears) the content of the underlying widget
  virtual void resetContent() = 0;

};

#endif // QVCT_CVESSELWIDGET_HPP
