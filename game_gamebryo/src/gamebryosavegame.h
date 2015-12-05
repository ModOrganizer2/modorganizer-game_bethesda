#ifndef GAMEBRYOSAVEGAME_H
#define GAMEBRYOSAVEGAME_H

#include "isavegame.h"

#include <QDateTime>
#include <QImage>
#include <QString>
#include <QStringList>

//class QFile;
#include <QFile>

class GamebryoSaveGame : public MOBase::ISaveGame
{
public:
  GamebryoSaveGame(QString const &file);

  virtual ~GamebryoSaveGame();

  virtual QString getFilename() const override;

  virtual QDateTime getCreationTime() const override;

  //Simple getters
  QString getPCName() const { return m_PCName; }
  unsigned short getPCLevel() const { return m_PCLevel; }
  QString getPCLocation() const { return m_PCLocation; }
  unsigned long getSaveNumber() const { return m_SaveNumber; }
  QStringList const &getPlugins() const { return m_Plugins; }
  QImage const &getScreenshot() const { return m_Screenshot; }

protected:

  friend class FileWrapper;

  class FileWrapper
  {
  public:
    /** Construct the save file information.
     * @params expected - expect bytes at start of file
     **/
    FileWrapper(GamebryoSaveGame *game, QString const &expected);

    /** Set this for save games that have a marker at the end of each
     * field. Specifically fallout
     **/
    void setHasFieldMarkers(bool);

    /** The length of the string length.
     * Normally a string has a 2 byte length. Oblivion has a single byte.
     **/
    void setStringLength(std::size_t len);

    template <typename T> void skip(int count = 1)
    {
      if (!m_File.seek(m_File.pos() + count * sizeof(T))) {
        throw std::runtime_error("unexpected end of file");
      }
    }

    template <typename T> void read(T &value)
    {
      int read = m_File.read(reinterpret_cast<char*>(&value), sizeof(T));
      if (read != sizeof(T)) {
        throw std::runtime_error("unexpected end of file");
      }
      if (m_HasFieldMarkers) {
        skip<char>();
      }
    }

    template <> void read(QString &value);

    void read(void *buff, std::size_t length);

    void readPlugins();

  private:
    GamebryoSaveGame *m_Game;
    QFile m_File;
    bool m_HasFieldMarkers;
    std::size_t m_Length;
  };


  template <typename T> void FileRead(QFile &file, T &value)
  {
    int read = file.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (read != sizeof(T)) {
      throw std::runtime_error("unexpected end of file");
    }
  }

  template <typename T> void FileSkip(QFile &file, int count = 1)
  {
    if (!file.seek(file.pos() + count * sizeof(T))) {
      throw std::runtime_error("unexpected end of file");
    }
  }

  void readHeader(QFile &file, QString const &expected);

  QString m_FileName;
  QString m_PCName;
  unsigned short m_PCLevel;
  QString m_PCLocation;
  unsigned long m_SaveNumber;
  QDateTime m_CreationTime;
  QStringList m_Plugins;
  QImage m_Screenshot;
};

#endif // GAMEBRYOSAVEGAME_H
