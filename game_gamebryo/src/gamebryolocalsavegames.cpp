/*
Copyright (C) 2015 Sebastian Herbord. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "gamebryolocalsavegames.h"
#include <iprofile.h>
#include <QtDebug>
#include <windows.h>
#include <stddef.h>
#include <string>


static const QString LocalSavesDummy = "__MO_Saves";


GamebryoLocalSavegames::GamebryoLocalSavegames(const QDir &myGamesDir,
                                               const QString &iniFileName)
  : m_LocalSavesDir(myGamesDir.absoluteFilePath(LocalSavesDummy))
  , m_LocalGameDir(myGamesDir.absolutePath())
  , m_IniFileName(iniFileName)
{}


void GamebryoLocalSavegames::prepareProfile(MOBase::IProfile *profile)
{
  bool enable = profile->localSavesEnabled();
  qDebug("enable local saves: %d", enable);
  QString basePath
          = profile->localSettingsEnabled()
            ? profile->absolutePath()
            : m_LocalGameDir.absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;
  WritePrivateProfileStringW(L"General", L"bUseMyGamesDirectory",
                             enable ? L"0" : L"1",
                             iniFilePath.toStdWString().c_str());

  WritePrivateProfileStringW(L"General", L"SLocalSavePath",
                             enable ? (LocalSavesDummy + "\\").toStdWString().c_str()
                                    : NULL,
                             iniFilePath.toStdWString().c_str());
}


MappingType GamebryoLocalSavegames::mappings(const QDir &profileSaveDir) const
{
  return {{
           profileSaveDir.absolutePath(),
           m_LocalSavesDir.absolutePath(),
           true,
           true
    }};
}
