#include "morrowindsavegameinfowidget.h"
#include "ui_morrowindsavegameinfowidget.h"

#include "gamemorrowind.h"
#include "morrowindsavegame.h"
#include "morrowindsavegameinfo.h"
#include "imoinfo.h"
#include "ipluginlist.h"

#include <QDate>
#include <QDateTime>
#include <QFrame>
#include <QFont>
#include <QLabel>
#include <QLayout>
#include <QLayoutItem>
#include <QPixmap>
#include <QString>
#include <QStyle>
#include <QTime>
#include <QVBoxLayout>

#include <Qt>
#include <QtGlobal>

#include <memory>

MorrowindSaveGameInfoWidget::MorrowindSaveGameInfoWidget(MorrowindSaveGameInfo const *info,
  QWidget *parent)
  : MOBase::ISaveGameInfoWidget(parent), ui(new Ui::MorrowindSaveGameInfoWidget), m_Info(info) {
  ui->setupUi(this);
  this->setWindowFlags(Qt::ToolTip | Qt::BypassGraphicsProxyWidget);
  setWindowOpacity(style()->styleHint(QStyle::SH_ToolTipLabel_Opacity, 0, this) / qreal(255.0));
  ui->gameFrame->setStyleSheet("background-color: transparent;");

  QVBoxLayout *gameLayout = new QVBoxLayout();
  gameLayout->setMargin(0);
  gameLayout->setSpacing(2);
  ui->gameFrame->setLayout(gameLayout);
}

MorrowindSaveGameInfoWidget::~MorrowindSaveGameInfoWidget() {
  delete ui;
}

void MorrowindSaveGameInfoWidget::setSave(QString const &file) {
  std::unique_ptr < MorrowindSaveGame const> save(
    std::move(dynamic_cast<MorrowindSaveGame const *>(m_Info->getSaveGameInfo(file))));
  ui->saveNameLabel->setText(QString("%1 (Day %2)").arg(save->getSaveName()).arg(save->getGameDays()));
  ui->saveNumLabel->setText(QString("%1").arg(save->getSaveNumber()));
  ui->healthLabel->setText(QString("%1 / %2").arg(round(save->getPCCurrentHealth())).arg(save->getPCMaxHealth()));
  ui->characterLabel->setText(save->getPCName());
  ui->locationLabel->setText(save->getPCLocation());
  ui->levelLabel->setText(QString("%1").arg(save->getPCLevel()));
  //This somewhat contorted code is because on my system at least, the
  //old way of doing this appears to give short date and long time.
  QDateTime t = save->getCreationTime();
  ui->dateLabel->setText(t.date().toString(Qt::DefaultLocaleShortDate) + " " +
    t.time().toString(Qt::DefaultLocaleLongDate));
  ui->screenshotLabel->setPixmap(QPixmap::fromImage(save->getScreenshot()));
  if (ui->gameFrame->layout() != nullptr) {
    QLayoutItem *item = nullptr;
    while ((item = ui->gameFrame->layout()->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
    ui->gameFrame->layout()->setSizeConstraint(QLayout::SetFixedSize);
  }

  // Resize box to new content
  this->resize(0, 0);

  QLayout *layout = ui->gameFrame->layout();
  QLabel *header = new QLabel(tr("Missing ESPs"));
  QFont headerFont = header->font();
  QFont contentFont = headerFont;
  headerFont.setItalic(true);
  contentFont.setBold(true);
  contentFont.setPointSize(7);
  header->setFont(headerFont);
  layout->addWidget(header);
  int count = 0;
  MOBase::IPluginList *pluginList = m_Info->m_Game->m_Organizer->pluginList();
  for (QString const &pluginName : save->getPlugins()) {
    if (pluginList->state(pluginName) == MOBase::IPluginList::STATE_ACTIVE) {
      continue;
    }

    ++count;

    if (count > 7) {
      break;
    }

    QLabel *pluginLabel = new QLabel(pluginName);
    pluginLabel->setIndent(10);
    pluginLabel->setFont(contentFont);
    layout->addWidget(pluginLabel);
  }
  if (count > 7) {
    QLabel *dotDotLabel = new QLabel("...");
    dotDotLabel->setIndent(10);
    dotDotLabel->setFont(contentFont);
    layout->addWidget(dotDotLabel);
  }
  if (count == 0) {
    QLabel *dotDotLabel = new QLabel(tr("None"));
    dotDotLabel->setIndent(10);
    dotDotLabel->setFont(contentFont);
    layout->addWidget(dotDotLabel);
  }
}
