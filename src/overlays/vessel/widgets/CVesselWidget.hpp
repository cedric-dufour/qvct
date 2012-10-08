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
#include <QBoxLayout>
#include <QDockWidget>
#include <QFont>
#include <QString>
#include <QWidget>

// QVCT
class CVesselPoint;


/// [UI] Generic vessel (dock) widget
/**
 *  This class defines the base user-interface required to display vessels
 *  (dock) widgets.
 *  These widgets will automatically change their layout between vertical
 *  and horizontal depending on where they are docked, as well as adapt their
 *  font size according to their size.
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
  /// [UI:Layout] Layout
  QBoxLayout* pqBoxLayout;

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
  // METHODS: QWidget (override)
  //------------------------------------------------------------------------------

protected slots:
  virtual void resizeEvent( QResizeEvent* _pqResizeEvent );


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// Slot to handle object destruction
  void slotDestroyed( QObject* _pqObject );

private slots:
  /// Slot to handle dock area change
  void slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea );
  /// Slot to handle floating change
  void slotTopLevelChanged( bool _bTopLevel );

  // SETTERS
public:
  /// Sets the vessel used to synchronize the instruments display
  void setVesselPoint( CVesselPoint* _poVesselPoint );
  /// Resets (clears) the vessel used to synchronize the instruments display
  void resetVesselPoint();

  // OTHER
public:
  /// Sets the font for the content of the underlying widget
  virtual void setFont( QFont _qFont ) = 0;
  /// Refreshes the content of the underlying widget
  virtual void refreshContent() = 0;
  /// Resets (clears) the content of the underlying widget
  virtual void resetContent() = 0;

};

#endif // QVCT_CVESSELWIDGET_HPP
