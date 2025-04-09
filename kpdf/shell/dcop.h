/***************************************************************************
 *   Copyright (C) 2025 by Philippe Mavridis <mavridisf@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KPDF_SHELL_IFACE_H
#define KPDF_SHELL_IFACE_H

#include <dcopobject.h>
#include <kurl.h>

class KPDFShellDCOPIface : virtual public DCOPObject
{
  K_DCOP

  k_dcop:
    virtual void openURL(const KURL & url) = 0;
    virtual void addTab() = 0;
    virtual void removeTab() = 0;
    virtual const KURL currentTabURL() = 0;
};

#endif // KPDF_SHELL_IFACE_H