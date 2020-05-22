#ifndef GAMEBRYO_MODATACHECKER_H
#define GAMEBRYO_MODATACHECKER_H

#include <moddatachecker.h>
#include <ifiletree.h>

class GameGamebryo;

class GamebryoModDataChecker: public ModDataChecker 
{
  /**
   * @brief Standard list of folders.
   */
  static const QStringList STANDARD_FOLDERS;

  /**
   * @brief Standard list of extensions.
   */
  static const QStringList STANDARD_EXTENSIONS;

public:


  /**
   * @brief Construct a new mod-data checker for GameBryo games using the default
   *     list of possible folders and extensions.
   */
  GamebryoModDataChecker(const GameGamebryo* game) : 
    GamebryoModDataChecker(game, STANDARD_FOLDERS, STANDARD_EXTENSIONS) { }


  /**
   * @brief Construct a new mod-data checker for GameBryo games using the given
   *     list of possible folders and extensions.
   *
   * @param folders List of folders that should be found in the data folder.
   * @param extensions List of extension of files that should be found in the data folder (without
   *     the leading dot).
   */
  GamebryoModDataChecker(const GameGamebryo* game, QStringList folders, QStringList extensions);


  virtual bool dataLooksValid(std::shared_ptr<const MOBase::IFileTree> fileTree) const override;

protected:
  const GameGamebryo* game() const { return m_Game; }

private:
  const GameGamebryo* m_Game;

  std::set<QString, MOBase::FileNameComparator> m_FolderNames;
  std::set<QString, MOBase::FileNameComparator> m_FileExtensions;

};

#endif // GAMEBRYO_MODATACHECKER_H
