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

/** @mainpage Qt Virtual Chart Table (QVCT)
 *  @author     Cedric Dufour <http://cedric.dufour.name>
 *
 *  @tableofcontents
 *
 *  @section sysnopsis Synopsis
 *
 *  The Qt Virtual Chart Table (QVCT) is a Qt4 application which provides a
 *  navigation software that simulates the chart table (and instruments corner)
 *  usually found in ships.
 *
 *  It is thus suitable for sailing, flying or any other outdoor activity performed
 *  using a conventional navigation/planning approach:
 *   - using heading, speed and time as primary navigation means
 *   - using easily identifiable landmarks as references
 *   - navigating along discrete waypoints routes
 *   - relying on GPS only as a secondary navigation mean
 *   - using offline charts (rather than Internet-dependent online resources)
 *
 *  It features:
 *   - loading offline geo-referenced charts, stored in any format/projection
 *     supported by the Geospatial Data Abstraction Library (GDAL)
 *   - associating a digital elevation model (DEM) to any chart, stored in any
 *     format/projection supported by the Geospatial Data Abstraction Library (GDAL)
 *   - creating, loading, saving and managing landmarks, routes and tracks, stored
 *     in GPX or QVCT (XML) format
 *   - creating, loading, saving and managing (multiple) vessels, stored in QVCT
 *     (XML) format and linkable, individually or globally (dynamic flotilla), to
 *     (multiple) navigation devices (see below)
 *   - defining and using multiple navigation devices, including:
 *      * GPS daemon (GPSD) - GPS mode
 *      * GPS daemon (GPSD) - AIS mode
 *   - loading and saving entire navigation/planning projects, stored in QVCT (XML)
 *     format
 *   - displaying vessel instruments in an activity-specific cockpit, including:
 *      * general aviation "basic six"
 *   - printing charts and overlays (landmarks, routes, etc.) in standard (screen)
 *     or high (1200 DPI) resolution
 *   - touchscreen-friendly user interface (you really don't need a mouse or key-
 *     board while navigating)
 *
 *
 *  @section copyright Copyright
 *
 *  The Qt Virtual Chart Table (QVCT) is free software:
 *  you can redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation, Version 3.
 *
 *  The Qt Virtual Chart Table (QVCT) is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  See the GNU General Public License for more details.
 *
 *  The Qt Virtual Chart Table (QVCT) includes all resources which contain the
 *  mention "Qt Virtual Chart Table (QVCT)" in their preamble.
 *
 *  Other resources are (and must be) used according to their original license,
 *  which is (should be made) available from their respective author.
 *
 *
 *  @section install Build and Install
 *
 *  The first step is recovering the source code using the GIT versioning system:
 *
 *  @verbatim
mkdir /path/to/source && cd /path/to/source
git clone https://github.com/cedric-dufour/qvct
@endverbatim
 *
 *  Build environment configuration is then achieved using CMake:
 *
 *  @verbatim
mkdir /path/to/build && cd /path/to/build
cmake /path/to/source
@endverbatim
 *
 *  Compilation and installation are then achieved using regular Make:
 *
 *  @verbatim
cd /path/to/build
make && ls -al ./bin
make doc && ls -al ./doc
make install
@endverbatim
 *
 *  Alternatively (to Make), distribution-specific Debian packages can be built
 *  and installed using the ad-hoc commands:
 *
 *  @verbatim
cd /path/to/source
debuild -us -uc -b && ls -al ../qvct*.deb
dpkg -i ../qvct*.deb
@endverbatim
 *
 *
 *  @section development Contribute to the Development
 *
 *
 *  @subsection development_architecture General Architecture
 *
 *  The Qt Virtual Chart Table (QVCT) is build on top of the standard Qt4's
 *  QMainWindow object. The screenshot below shows which class(es) correspond to
 *  each of the visible parts of the interface (and give you the place to start
 *  digging into the source code):
 *
 *  @image html qvct-devel.jpg
 *
 *  Logically speaking, QVCT is based on charts (CChart), each corresponding to a
 *  separate tab in the virtual chart table (CChartTable), and several overlays,
 *  which correspond respectively to:
 *   - the mouse pointer (CPointerOverlay, CPointerPoint)
 *   - landmarks (CLandmarkOverlay, CLandmarkContainer, CLandmarkPoint)
 *   - routes (CRouteOverlay, CRouteContainer, CRoutePoint)
 *   - tracks (CTrackOverlay, CTrackContainer, CTrackSubContainer, CTrackPoint)
 *   - vessels (CVesselOverlay, CVesselContainer, CVesselContainerDevice, CVesselPoint, CVesselPointDevice)
 *   - devices (CDeviceOverlay, CDevice).
 *
 *  Each overlay is actually a QTreeWidget and a (root) QTreeWidgetItem, which
 *  hierarchically gathers objects (COverlayObject, QTreeWidgetItem) in (sub-)containers
 *  (COverlayContainer) and (sub-)items (COverlayItem), which in turn can be
 *  geographical points (COverlayPoint) or courses (COverlayCourse).
 *
 *
 *  @subsection development_device Device Drivers
 *
 *  The architecture of the software should allow to integrate and use many kind
 *  of navigation devices. Please look for the SAMPLE_DRIVER keyword in the
 *  source tree to find out about the key places (and sample code) to create a new
 *  driver for your device.
 *
 *  Looking at the source code of the existing GPS daemon (GPSD) driver (CDeviceGpsdGps)
 *  may also provide some useful insights and ways to avoid pitfalls.
 *
 *
 *  @subsection development_coding Coding Conventions
 *
 *  If you are to submit patches for bug-fixing or features-enhancement purposes,
 *  please make sure to stick to the coding conventions detailed in the CODING
 *  file located in the root of the source tree. Patches that do not respect those
 *  conventions *will not* be considered.
 *
 */

#ifndef QVCT_MAIN_HPP
#define QVCT_MAIN_HPP

#ifndef VER_STRING
#define VER_STRING "development snapshot"
#endif

#ifndef CONFDIR_STRING
#define CONFDIR_STRING "/.config/qvct"
#endif

/// Returns the system time with microseconds resolution, in seconds
double microtime();

#endif // QVCT_MAIN_HPP
