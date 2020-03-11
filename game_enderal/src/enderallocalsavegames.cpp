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


#include "enderallocalsavegames.h"
#include "registry.h"
#include <iprofile.h>
#include <QtDebug>
#include <windows.h>
#include <stddef.h>
#include <string>


static const QString LocalSavesDummy = "..\\Enderal\\__MO_Saves\\";


EnderalLocalSavegames::EnderalLocalSavegames(const QDir &myGamesDir,
                                               const QString &iniFileName)
  : m_LocalSavesDir(myGamesDir.absoluteFilePath(LocalSavesDummy))
  , m_LocalGameDir(myGamesDir.absolutePath())
  , m_IniFileName(iniFileName)
{}


MappingType EnderalLocalSavegames::mappings(const QDir &profileSaveDir) const
{
  return {{
           profileSaveDir.absolutePath(),
           m_LocalSavesDir.absolutePath(),
           true,
           true
    }};
}


bool EnderalLocalSavegames::prepareProfile(MOBase::IProfile *profile)
{
  bool enable = profile->localSavesEnabled();

  QString basePath
    = profile->localSettingsEnabled()
    ? profile->absolutePath()
    : m_LocalGameDir.absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;
  QString saveIni = profile->absolutePath() + "/" + "savepath.ini";

  // Get the current sLocalSavePath
  WCHAR currentPath[MAX_PATH];
  GetPrivateProfileStringW(L"General", L"sLocalSavePath", L"SKIP_ME", currentPath, MAX_PATH, iniFilePath.toStdWString().c_str());
  bool alreadyEnabled = wcscmp(currentPath, LocalSavesDummy.toStdWString().c_str()) == 0;

  // Get the current bUseMyGamesDirectory
  WCHAR currentMyGames[MAX_PATH];
  GetPrivateProfileStringW(L"General", L"bUseMyGamesDirectory", L"SKIP_ME", currentMyGames, MAX_PATH, iniFilePath.toStdWString().c_str());

  // Create the __MO_Saves directory if local saves are enabled and it doesn't exist
  if (enable) {
    QDir saves = QDir(m_LocalGameDir.absolutePath() + "/" + LocalSavesDummy);
    if (!saves.exists()) {
      saves.mkdir(".");
    }
  }

  // Set the path to __MO_Saves if it's not already
  if (enable && !alreadyEnabled) {
    // If the path is not blank, save it to savepath.ini
    if (wcscmp(currentPath, L"SKIP_ME") != 0) {
      MOBase::WriteRegistryValue(L"General", L"sLocalSavePath", currentPath, saveIni.toStdWString().c_str());
    }
    if (wcscmp(currentMyGames, L"SKIP_ME") != 0) {
      MOBase::WriteRegistryValue(L"General", L"bUseMyGamesDirectory", currentMyGames, saveIni.toStdWString().c_str());
    }
    MOBase::WriteRegistryValue(L"General", L"sLocalSavePath", LocalSavesDummy.toStdWString().c_str(), iniFilePath.toStdWString().c_str());
    MOBase::WriteRegistryValue(L"General", L"bUseMyGamesDirectory", L"1", iniFilePath.toStdWString().c_str());
  }

  // Get rid of the local saves setting if it's still there
  if (!enable && alreadyEnabled) {
    // If savepath.ini exists, use it and delete it
    if (QFile::exists(saveIni)) {
      WCHAR savedPath[MAX_PATH];
      WCHAR savedMyGames[MAX_PATH];
      GetPrivateProfileStringW(L"General", L"sLocalSavePath", L"DELETE_ME", savedPath, MAX_PATH, saveIni.toStdWString().c_str());
      GetPrivateProfileStringW(L"General", L"bUseMyGamesDirectory", L"DELETE_ME", savedMyGames, MAX_PATH, saveIni.toStdWString().c_str());
      if (wcscmp(savedPath, L"DELETE_ME") != 0) {
        MOBase::WriteRegistryValue(L"General", L"sLocalSavePath", savedPath, iniFilePath.toStdWString().c_str());
      }
      else {
        MOBase::WriteRegistryValue(L"General", L"sLocalSavePath", NULL, iniFilePath.toStdWString().c_str());
      }
      if (wcscmp(savedMyGames, L"DELETE_ME") != 0) {
        MOBase::WriteRegistryValue(L"General", L"bUseMyGamesDirectory", savedMyGames, iniFilePath.toStdWString().c_str());
      }
      else {
        MOBase::WriteRegistryValue(L"General", L"bUseMyGamesDirectory", NULL, iniFilePath.toStdWString().c_str());
      }
      QFile::remove(saveIni);
    }
    // Otherwise just delete the setting
    else {
      MOBase::WriteRegistryValue(L"General", L"sLocalSavePath", NULL, iniFilePath.toStdWString().c_str());
      MOBase::WriteRegistryValue(L"General", L"bUseMyGamesDirectory", NULL, iniFilePath.toStdWString().c_str());
    }
  }

  return enable != alreadyEnabled;
}
