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

#ifndef QVCT_CTIMEVIEW_HPP
#define QVCT_CTIMEVIEW_HPP

// QT
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QWidget>


/// [UI] Time view (dock widget)
/**
 *  This class provides the user-interface (QDockWidget) that allows to display
 *  overlay containers/items' details.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CTimeView: public QDockWidget
{
  Q_OBJECT


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Widget] Container widget
  QWidget* pqWidget;
  /// [UI:Layout] Layout
  QBoxLayout* pqBoxLayout;

  /// [UI:Label] Date
  QLabel* pqLabelDate;
  /// [UI:Label] Time
  QLabel* pqLabelTime;
  /// [UI:Label] Time (alternate timezone)
  QLabel* pqLabelTime2;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CTimeView( QWidget* _pqParent = 0 );
  virtual ~CTimeView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


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
  /// Refreshes the content of the underlying widget
  void refreshContent();

};

#endif // QVCT_CTIMEVIEW_HPP
