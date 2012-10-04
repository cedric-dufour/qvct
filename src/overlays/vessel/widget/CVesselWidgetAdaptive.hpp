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

#ifndef QVCT_CVESSELWIDGETADAPTIVE_HPP
#define QVCT_CVESSELWIDGETADAPTIVE_HPP

// QT
#include <QBoxLayout>
#include <QDockWidget>
#include <QFont>
#include <QString>
#include <QWidget>

// QVCT
#include "overlays/vessel/widget/CVesselWidget.hpp"


/// [UI] Adaptive generic vessel (dock) widget
/**
 *  This class defines the base user-interface required to display adaptive
 *  vessels (dock) widgets.
 *  Adaptive widgets will automatically change their layout between vertical
 *  and horizontal depending on where they are docked, as well as adapt their
 *  font size according to their size.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselWidgetAdaptive: public CVesselWidget
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

protected:
  /// [UI:Layout] Layout
  QBoxLayout* pqBoxLayout;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

protected:
  CVesselWidgetAdaptive( const QString& _qsTitle, QWidget* _pqParent );
  virtual ~CVesselWidgetAdaptive() {};


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
  /// Slot to handle dock area change
  void slotLocationChanged( Qt::DockWidgetArea _qDockWidgetArea );
  /// Slot to handle floating change
  void slotTopLevelChanged( bool _bTopLevel );

  // OTHER
public:
  /// Sets the font for the content of the underlying widget
  virtual void setFont( QFont _qFont ) = 0;

};

#endif // QVCT_CVESSELWIDGETADAPTIVE_HPP
