/**
 * Copyright (C) 2003, Luís Pedro Coelho,
 * based on tdelibs/kparts/genericfactory.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KGVPart_H_INCLUDE_GUARD_
#define KGVPart_H_INCLUDE_GUARD_

#include <kparts/factory.h>
#include <kparts/part.h>

class TDEInstance;
class TDEAboutData;

class KDE_EXPORT KGVFactory : public KParts::Factory
{
    public:
        KGVFactory();
        virtual ~KGVFactory();
        static TDEInstance *instance();
        static TDEAboutData *aboutData();

        virtual KParts::Part *createPartObject( TQWidget *parentWidget, const char *widgetName,
                TQObject *parent, const char *name,
                const char *className,
                const TQStringList &args );

    protected:
        virtual TDEInstance *createInstance();
    private:
        static KGVFactory*s_self;
        static TDEInstance *s_instance;
        static TDEAboutData *s_aboutData;
};

#endif

