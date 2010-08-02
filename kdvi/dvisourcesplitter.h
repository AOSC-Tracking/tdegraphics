// -*- C++ -*-
//
// C++ Interface: dvisourcesplitter
//
// Author: Jeroen Wijnhout <Jeroen.Wijnhout@kdemail.net>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef DVI_SOURCEFILESPLITTER_H
#define DVI_SOURCEFILESPLITTER_H

#include <tqfileinfo.h>

class TQString;


class DVI_SourceFileSplitter 
{
public:
  DVI_SourceFileSplitter(const TQString & scrlink, const TQString & dviFile);
  
  TQString  fileName() { return m_fileInfo.fileName(); }
  TQString  filePath() { return m_fileInfo.absFilePath(); }
  bool     fileExists() { return m_fileInfo.exists(); }
  
  Q_UINT32 line()     { return m_line; }
  
private:
  TQFileInfo m_fileInfo;
  Q_UINT32  m_line;
  bool      m_exists; 
};
#endif
