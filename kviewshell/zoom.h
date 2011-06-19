// -*- C++ -*-
// zoom.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// (C) 2002 Stefan Kebekus
// Distributed under the GPL

// Add header files alphabetically

#ifndef ZOOM_H
#define ZOOM_H

#include <tqobject.h>
#include <tqstringlist.h>


class Zoom : public TQObject
{
Q_OBJECT
  TQ_OBJECT

public:
  /** Initializs the zoom with a default of 100% */
  Zoom();

  /** Returns a list like "33%", "100%", etc. If you call
      zoomNames() more than once, it is guaranteed that the same
      list of strings will be returned. */
  TQStringList zoomNames() const { return valueNames; }

  float       zoomIn();
  float       zoomOut();
  float       value() const { return _zoomValue; }

public slots:
  void        setZoomValue(float);
  void        setZoomValue(const TQString &);

  void        setZoomFitWidth(float zoom);
  void        setZoomFitHeight(float zoom);
  void        setZoomFitPage(float zoom);

signals:
  void        zoomNamesChanged(const TQStringList &);
  void        zoomNameChanged(const TQString &);
  void        valNoChanged(int);

private:
  float       _zoomValue;
  TQStringList valueNames;
  // This will be the number of the current value in the generated TQStringList.
  int         valNo;
};

#endif
