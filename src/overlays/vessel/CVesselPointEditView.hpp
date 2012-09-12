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

#ifndef QVCT_CVESSELPOINTEDITVIEW_HPP
#define QVCT_CVESSELPOINTEDITVIEW_HPP

// QT
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

// QVCT
#include "overlays/COverlayObjectEditView.hpp"
class CVesselPoint;


/// [UI] Vessel point's (item) edit view
/**
 *  This class implements the user-interface that allows to edit vessel
 *  points (items).
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPointEditView: public COverlayObjectEditView
{

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// [UI:LineEdit] Name
  QLineEdit* pqLineEditName;
  /// [UI:LineEdit] Longitude
  QLineEdit* pqLineEditLongitude;
  /// [UI:LineEdit] Latitude
  QLineEdit* pqLineEditLatitude;
  /// [UI:LineEdit] Elevation
  QLineEdit* pqLineEditElevation;
  /// [UI:LineEdit] Date
  QLineEdit* pqLineEditDate;
  /// [UI:LineEdit] Time
  QLineEdit* pqLineEditTime;
  /// [UI:LineEdit] Ground bearing
  QLineEdit* pqLineEditGroundBearing;
  /// [UI:LineEdit] Ground horizontal speed
  QLineEdit* pqLineEditGroundSpeed;
  /// [UI:LineEdit] Ground vertical speed
  QLineEdit* pqLineEditGroundSpeedVertical;
  /// [UI:LineEdit] Apparent bearing
  QLineEdit* pqLineEditApparentBearing;
  /// [UI:LineEdit] Apparent horizontal speed
  QLineEdit* pqLineEditApparentSpeed;
  /// [UI:LineEdit] Apparent vertical speed
  QLineEdit* pqLineEditApparentSpeedVertical;
  /// [UI:LineEdit] Type
  QLineEdit* pqLineEditType;
  /// [UI:TextEdit] Description
  QTextEdit* pqTextEditDescription;
  /// [UI:TextEdit] Comment
  QTextEdit* pqTextEditComment;
  /// [UI:LineEdit] Symbol
  QLineEdit* pqLineEditSymbol;
  /// [UI:LineEdit] URL
  QLineEdit* pqLineEditUrl;
  /// [UI:SpinBox] Track recroding rate
  QSpinBox* pqSpinBoxTrackRecordRate;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPointEditView( CVesselPoint* _poVesselPoint );
  virtual ~CVesselPointEditView() {};

private:
  /// Constructs the layout of the user-interface
  void constructLayout();


  //------------------------------------------------------------------------------
  // METHODS: QDialog (override)
  //------------------------------------------------------------------------------

public slots:
  virtual void accept();

};

#endif // QVCT_CVESSELPOINTEDITVIEW_HPP
