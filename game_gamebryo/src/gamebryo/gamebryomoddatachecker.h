#ifndef GAMEBRYO_MODATACHECKER_H
#define GAMEBRYO_MODATACHECKER_H

#include <moddatachecker.h>
#include <ifiletree.h>

class GameGamebryo;

/**
 * @brief ModDataChecker for GameBryo games that look at folder and files in the "data"
 *     directory.
 *
 * The default implementation is game-agnostic and uses the list of folders and file extensions
 * that were used before the ModDataChecker feature was added. It is possible to inherit the class
 * to provide custom list of folders or filenames.
 */
class GamebryoModDataChecker: public ModDataChecker {
public:


  /**
   * @brief Construct a new mod-data checker for GameBryo games.
   */
  GamebryoModDataChecker(const GameGamebryo* game);

  virtual bool dataLooksValid(std::shared_ptr<const MOBase::IFileTree> fileTree) const override;

protected:

  using FileNameSet = std::set<QString, MOBase::FileNameComparator>;

  const GameGamebryo* game() const { return m_Game; }

  /**
   * @return the list of possible folder names in data.
   */
  virtual const FileNameSet& possibleFolderNames() const;

  /**
   * @return the extensions of possible files in data.
   */
  virtual const FileNameSet& possibleFileExtensions() const;

private:
  const GameGamebryo* m_Game;

};

#endif // GAMEBRYO_MODATACHECKER_H
