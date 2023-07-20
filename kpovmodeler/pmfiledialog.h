/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMFILEDIALOG_H
#define PMFILEDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "pmiomanager.h"
#include <tdefiledialog.h>

class PMPart;

/**
 * File dialog used for the import and export action.
 *
 * The filters will be set automatically dependent on the
 * supported file formats
 * @see PMIOManager
 */
class PMFileDialog : public KFileDialog
{
   TQ_OBJECT
  
public:
   /**
    * Default constructor, use the static methods
    * @ref getImportFileName or @ref getExportFileName instead
    */
   PMFileDialog( const TQString& startDir, const TQString& filter, TQWidget* parent, const char* name, bool modal );
   /**
    * Opens a modal file dialog and returns a selected file and the chosen
    * file format.
    */
   static TQString getImportFileName( TQWidget* parent, PMPart* part, PMIOFormat*& format );
   /**
    * Opens a modal file dialog and returns a selected file and the chosen
    * file format and filter.
    */
   static TQString getExportFileName( TQWidget* parent, PMPart* part,
                                     PMIOFormat*& format, TQString& filter );
};

#endif
