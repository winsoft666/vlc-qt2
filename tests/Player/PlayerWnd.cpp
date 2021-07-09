#include "PlayerWnd.h"
#include <QStyle>
#include "core/Audio.h"
#include "core/Common.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaPlayer.h"
#include "widgets/WidgetVideo.h"
#include "widgets/WidgetSeek.h"
#include "widgets/WidgetVolumeSlider.h"

#pragma execution_character_set("utf-8")

#define LIBVLC_PLUGINS_DIR ""

PlayerWnd::PlayerWnd(QWidget* parent) :
    QWidget(parent), vlcInstance_(nullptr), vlcPlayer_(nullptr) {
  setupUi();

  qunsetenv("VLC_PLUGIN_PATH");
#ifndef Q_OS_DARWIN
  VlcCommon::setPluginPath(QString(LIBVLC_PLUGINS_DIR));
#endif

  vlcInstance_ = new VlcInstance(VlcCommon::args(), this);
  vlcInstance_->setLogLevel(Vlc::DebugLevel);

  vlcPlayer_ = new VlcMediaPlayer(vlcInstance_);
  vlcPlayer_->setVideoWidget(vlcWidgetVideo_);

  vlcWidgetVideo_->setMediaPlayer(vlcPlayer_);
  vlcWidgetVolSlider_->setMediaPlayer(vlcPlayer_);
  vlcWidgetSeek_->setMediaPlayer(vlcPlayer_);

  VlcMedia * vlcMedia = new VlcMedia("D:\\C72EA373775FA9C091EE60BB0952AB34.mp4", true, vlcInstance_);
  vlcPlayer_->open(vlcMedia);
}

PlayerWnd::~PlayerWnd() {
  if (vlcInstance_->status()) {
    vlcPlayer_->stop();
  }
}

void PlayerWnd::setupUi() {
  vlcWidgetVideo_ = new VlcWidgetVideo();
  vlcWidgetVideo_->setObjectName("vlcWidgetVideo");
  vlcWidgetVideo_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  vlcWidgetSeek_ = new VlcWidgetSeek();
  vlcWidgetSeek_->setObjectName("vlcWidgetSeek");
  vlcWidgetSeek_->setFixedHeight(32);

  vlcWidgetVolSlider_ = new VlcWidgetVolumeSlider();
  vlcWidgetVolSlider_->setObjectName("vlcWidgetVolSlider");
  vlcWidgetVolSlider_->setFixedSize(100, 32);

  QHBoxLayout* hlControl = new QHBoxLayout();
  hlControl->addWidget(vlcWidgetSeek_);
  hlControl->addWidget(vlcWidgetVolSlider_);


  QVBoxLayout* vlMain = new QVBoxLayout();
  vlMain->setContentsMargins(0, 0, 0, 0);
  vlMain->addWidget(vlcWidgetVideo_);
  vlMain->addLayout(hlControl);

  this->setLayout(vlMain);

  //QString qss;
  //QFile qssFile(":/QCefWidgetTest/ControlPanel.qss");
  //qssFile.open(QFile::ReadOnly);
  //if (qssFile.isOpen()) {
  //  qss = QString::fromUtf8(qssFile.readAll());
  //  this->setStyleSheet(qss);
  //  qssFile.close();
  //}

  this->resize(800, 600);
}