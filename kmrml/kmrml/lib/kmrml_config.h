/* This file is part of the KDE project
   Copyright (C) 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KMRML_CONFIG_H
#define KMRML_CONFIG_H

class TDEConfig;

#include <tqstringlist.h>
#include <kurl.h>

namespace KMrml
{
    class ServerSettings
    {
    public:
        ServerSettings();
        ServerSettings(const TQString& host, unsigned short int port,
                       bool autoPort, bool useAuth, const
                       TQString& user, const TQString& pass);

        // does NOT set the port in the KURL object, if autoPort is selected
        // kio_mrml is going to determine itself (via ServerSettings::port()).
        // This deuglifies the mrml:/ url a bit (no port is shown)
        KURL getUrl() const;

        TQString host;
        TQString user;
        TQString pass;
        unsigned short int configuredPort;
        bool autoPort :1; // only possible with host == localhost
        bool useAuth  :1;

        static ServerSettings defaults();

        // returns configuredPort or the automatically determined port,
        // depending on the value of autoPort
        unsigned short int port() const;
    };

    class Config
    {
    public:
        Config();
        Config( TDEConfig *config ); // does not take ownership of TDEConfig
        ~Config();

        bool sync();

        ServerSettings defaultSettings() const
        {
            return settingsForHost( m_defaultHost );
        }

        ServerSettings settingsForLocalHost() const;
        ServerSettings settingsForHost( const TQString& host ) const;

        void setDefaultHost( const TQString& host );

        /**
         * Indexed by the hostname -- ensures there are no dupes
         */
        void addSettings( const ServerSettings& settings );

        bool removeSettings( const TQString& host );

        TQStringList hosts() const { return m_hostList; }

        /**
         * The list of indexable directories -- only applicable to "localhost"
         */
        TQStringList indexableDirectories() const;
        void setIndexableDirectories( const TQStringList& dirs );

        TQString addCollectionCommandLine() const;
        void setAddCollectionCommandLine( const TQString& cmd );

        TQString removeCollectionCommandLine() const;
        void setRemoveCollectionCommandLine( const TQString& cmd );

        void setMrmldCommandLine( const TQString& cmd );
        TQString mrmldCommandline() const;

        // e.g. Wolfgang needs this :)
        bool serverStartedIndividually() const { 
            return m_serverStartedIndividually; 
        }
        
        static TQString mrmldDataDir();

    private:
        void init();

        TQString settingsGroup( const TQString& host ) const
        {
            return TQString::fromLatin1( "SettingsFor: " ).append( host );
        }

        bool m_serverStartedIndividually;
        TQString m_defaultHost;
        TQStringList m_hostList;

        TDEConfig *m_config;
        TDEConfig *m_ownConfig;
    };
}

#endif // KMRML_CONFIG_H
