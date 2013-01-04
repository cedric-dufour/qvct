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

#ifndef QVCT_CCHARTCONTROL_HPP
#define QVCT_CCHARTCONTROL_HPP

// QT
#include <QPushButton>
#include <QSlider>
#include <QWidget>

// QVCT
class CChartTable;


/// [UI] Chart control user-interface
/**
 *  This class implements the user-interface that allows to interact with the
 *  charts displayed in the chart table and change parameters such as scale,
 *  position options, measurement status, etc.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CChartControl: public QWidget
{
  Q_OBJECT
  friend class CChartTable;


  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:Button] Lock/unlock position
  QPushButton* pqPushButtonPositionLock;
  /// [UI:Button] Lock/unlock scale
  QPushButton* pqPushButtonScaleLock;
  /// [UI:Button] Scale actual
  QPushButton* pqPushButtonScaleActual;
  /// [UI:Button] Scale in (zoom out)
  QPushButton* pqPushButtonScaleIn;
  /// [UI:Button] Scale out (zoom in)
  QPushButton* pqPushButtonScaleOut;
  /// [UI:Button] Scale fit
  QPushButton* pqPushButtonScaleFit;
  /// [UI:Button] Target
  QPushButton* pqPushButtonTarget;
  /// [UI:Button] Single measurement
  QPushButton* pqPushButtonMeasureSingle;
  /// [UI:Button] Ongoing measurements
  QPushButton* pqPushButtonMeasure;
  /// [UI:Button] Elevation model
  QPushButton* pqPushButtonElevation;
  /// [UI:Button] Scale slider
  QSlider* pqSliderScale;

  /// Pointer actions activation status
  /** @see enablePointer() */
  bool bPointerEnable;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CChartControl( QWidget* _pqParent );
  virtual ~CChartControl() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SLOTS
private slots:
  /// [UI:Slot] Slot to modify the position lock/unlock status
  void slotPositionLock( bool _bLock );
  /// [UI:Slot] Slot to modify the scale lock/unlock status
  void slotScaleLock( bool _bLock );
  /// [UI:Slot] Slot to trigger scaling to the given value
  void slotScaleTo( int _iValue );
  /// [UI:Slot] Slot to trigger scale in (zoom out)
  void slotScaleIn();
  /// [UI:Slot] Slot to trigger scale out (zoom in)
  void slotScaleOut();
  /// [UI:Slot] Slot to manage elevation model (add, view, hide)
  void slotElevation( bool _bAddOrShow );
  /// [UI:Slot] Slot to add elevation model
  void slotElevationAdd();

  // SETTERS
private:
  /// Sets the position lock/unlock status
  void lockPosition( bool _bLock );
  /// Sets the scale lock/unlock status
  void lockScale( bool _bLock );
  /// Sets the scale at the given value
  void setScale( double _fdScale );
  /// Increases/decreases the scale by discrete steps
  void stepScale( bool _bIncrease, bool _bBigStep = true );
  /// Enables/disables target
  void enableTarget( bool _bEnable );
  /// Enables/disables single measurement
  void enableMeasureSingle( bool _bEnable );
  /// Enables/disables ongoing measurements
  void enableMeasure( bool _bEnable );

  // OTHER
public:
  /// Enables/disables all chart controls
  void enableControls( bool _bEnable );
  /// Enables/disables pointer actions
  void enablePointer( bool _bEnable );

};

#endif // QVCT_CCHARTCONTROL_HPP
