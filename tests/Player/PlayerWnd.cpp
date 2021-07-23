#include "PlayerWnd.h"
#include <QStyle>
#include "core/Audio.h"
#include "core/Video.h"
#include "core/Common.h"
#include "core/Instance.h"
#include "core/Media.h"
#include "core/MediaPlayer.h"
#include "core/MediaListPlayer.h"
#include "core/MediaList.h"
#include "core/MetaManager.h"
#include "widgets/WidgetVideo.h"
#include "widgets/WidgetSeek.h"
#include "widgets/WidgetVolumeSlider.h"

#pragma execution_character_set("utf-8")

#define LIBVLC_PLUGINS_DIR ""

#define REFRESH_STYLE(x)        \
    do {                        \
        style()->unpolish((x)); \
        style()->polish((x));   \
    } while (false)

template <typename QEnum>
QString QtEnumToString(const QEnum value)
{
    return QString::fromUtf8(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

PlayerWnd::PlayerWnd(QWidget *parent) : QWidget(parent), vlcInstance_(nullptr), vlcMediaListPlayer_(nullptr), pushButtonPuaseResume_(nullptr), pushButtonStop_(nullptr)
{
    setupUi();
    connect(pushButtonStop_, &QPushButton::clicked, this, &PlayerWnd::onStopButtonClicked);
    connect(pushButtonAdd_, &QPushButton::clicked, this, &PlayerWnd::onAddButtonClicked);
    connect(pushButtonDelete_, &QPushButton::clicked, this, &PlayerWnd::onDeleteButtonClicked);
    connect(pushButtonClear_, &QPushButton::clicked, this, &PlayerWnd::onClearButtonClicked);
    connect(pushButtonPuaseResume_, &QPushButton::clicked, this, &PlayerWnd::onPuaseResumeButtonClicked);
    connect(playlistWidget_, &QListWidget::itemDoubleClicked, this, &PlayerWnd::onPlaylistItemDoubleClicked);
    connect(spinBoxPlayRate_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PlayerWnd::onPlayRateSpinBoxValueChanged);
    connect(cmbPlaybackMode_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PlayerWnd::onPlaybackModeCurrentIndexChanged);
    connect(cmbAspectRatio_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PlayerWnd::onAspectRatioCurrentIndexChanged);
    connect(cmbCropRatio_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PlayerWnd::onCropRatioCurrentIndexChanged);

#ifndef Q_OS_DARWIN
    VlcCommon::setPluginPath(QString(LIBVLC_PLUGINS_DIR));
#endif

    vlcInstance_ = new VlcInstance(VlcCommon::args(), this);
    vlcInstance_->setLogLevel(Vlc::DebugLevel);

    vlcMediaList_ = new VlcMediaList(vlcInstance_);

    vlcMediaPlayer_ = new VlcMediaPlayer(vlcInstance_);
    vlcMediaPlayer_->setVideoWidget(vlcWidgetVideo_);
    vlcWidgetVideo_->setMediaPlayer(vlcMediaPlayer_);

    vlcWidgetVolSlider_->setMediaPlayer(vlcMediaPlayer_);
    vlcWidgetSeek_->setMediaPlayer(vlcMediaPlayer_);

    vlcMediaListPlayer_ = new VlcMediaListPlayer(vlcMediaPlayer_, vlcInstance_);
    vlcMediaListPlayer_->setMediaList(vlcMediaList_);
    VlcMediaPlayer *mediaPlayer = vlcMediaListPlayer_->mediaPlayer();
    VlcMediaList *mediaList = vlcMediaListPlayer_->currentMediaList();
    connect(vlcMediaListPlayer_, QOverload<VlcMedia *>::of(&VlcMediaListPlayer::nextItemSet), this, &PlayerWnd::onMediaListPlayerNextItemSet);
    connect(mediaPlayer, &VlcMediaPlayer::stateChanged, this, &PlayerWnd::onMediaPlayerStateChanged);
    connect(mediaList, &VlcMediaList::itemAdded, this, &PlayerWnd::onMediaListItemAdded);
    connect(mediaList, &VlcMediaList::itemDeleted, this, &PlayerWnd::onMediaListItemDeleted);
}

PlayerWnd::~PlayerWnd()
{
    if (vlcInstance_->status()) {
        vlcMediaListPlayer_->stop();
    }
}

void PlayerWnd::setupUi()
{
    this->setObjectName("PlayerWnd");
    this->setWindowTitle("Media Player");
    this->setWindowIcon(QIcon(":/svg/Resource/logo.svg"));

    vlcWidgetVideo_ = new VlcWidgetVideo();
    vlcWidgetVideo_->setObjectName("vlcWidgetVideo");
    vlcWidgetVideo_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    vlcWidgetSeek_ = new VlcWidgetSeek();
    vlcWidgetSeek_->setObjectName("vlcWidgetSeek");
    vlcWidgetSeek_->setFixedHeight(26);

    vlcWidgetVolSlider_ = new VlcWidgetVolumeSlider();
    vlcWidgetVolSlider_->setObjectName("vlcWidgetVolSlider");
    vlcWidgetVolSlider_->setFixedSize(100, 26);
    vlcWidgetVolSlider_->setOrientation(Qt::Orientation::Horizontal);
    vlcWidgetVolSlider_->setValue(50);

    pushButtonPuaseResume_ = new QPushButton();
    pushButtonPuaseResume_->setObjectName("pushButtonPuaseResume");
    pushButtonPuaseResume_->setCursor(QCursor(Qt::PointingHandCursor));
    pushButtonPuaseResume_->setFixedSize(22, 22);
    pushButtonPuaseResume_->setProperty("paused", "Y");

    pushButtonStop_ = new QPushButton();
    pushButtonStop_->setObjectName("pushButtonStop");
    pushButtonStop_->setCursor(QCursor(Qt::PointingHandCursor));
    pushButtonStop_->setFixedSize(22, 22);

    QHBoxLayout *hlControl = new QHBoxLayout();
    hlControl->setSpacing(6);
    hlControl->setContentsMargins(5, 0, 5, 0);
    hlControl->addWidget(pushButtonPuaseResume_);
    hlControl->addWidget(pushButtonStop_);
    hlControl->addWidget(vlcWidgetSeek_);
    hlControl->addWidget(vlcWidgetVolSlider_);

    QLabel *labelAspectRatio = new QLabel("Aspect:");

    cmbAspectRatio_ = new QComboBox();
    cmbAspectRatio_->setObjectName("cmbAspectRatio");
    cmbAspectRatio_->setFixedSize(72, 20);
    cmbAspectRatio_->addItem("Default", Vlc::Ratio::Original);
    cmbAspectRatio_->addItem("Stretch", Vlc::Ratio::KeepStretch);
    cmbAspectRatio_->addItem("16:10", Vlc::Ratio::R_16_10);
    cmbAspectRatio_->addItem("16:9", Vlc::Ratio::R_16_9);
    cmbAspectRatio_->addItem("1.85:1", Vlc::Ratio::R_185_100);
    cmbAspectRatio_->addItem("1:1", Vlc::Ratio::R_1_1);
    cmbAspectRatio_->addItem("2.21:1", Vlc::Ratio::R_221_100);
    cmbAspectRatio_->addItem("2.35:1", Vlc::Ratio::R_235_100);
    cmbAspectRatio_->addItem("2.39:1", Vlc::Ratio::R_239_100);
    cmbAspectRatio_->addItem("4:3", Vlc::Ratio::R_4_3);
    cmbAspectRatio_->addItem("5:3", Vlc::Ratio::R_5_3);
    cmbAspectRatio_->addItem("5:4", Vlc::Ratio::R_5_4);

    QLabel *labelCropRatio = new QLabel("Crop:");

    cmbCropRatio_ = new QComboBox();
    cmbCropRatio_->setObjectName("cmbCropRatio");
    cmbCropRatio_->setFixedSize(72, 20);
    cmbCropRatio_->addItem("Default", Vlc::Ratio::Original);
    cmbCropRatio_->addItem("16:10", Vlc::Ratio::R_16_10);
    cmbCropRatio_->addItem("16:9", Vlc::Ratio::R_16_9);
    cmbCropRatio_->addItem("1.85:1", Vlc::Ratio::R_185_100);
    cmbCropRatio_->addItem("1:1", Vlc::Ratio::R_1_1);
    cmbCropRatio_->addItem("2.21:1", Vlc::Ratio::R_221_100);
    cmbCropRatio_->addItem("2.35:1", Vlc::Ratio::R_235_100);
    cmbCropRatio_->addItem("2.39:1", Vlc::Ratio::R_239_100);
    cmbCropRatio_->addItem("4:3", Vlc::Ratio::R_4_3);
    cmbCropRatio_->addItem("5:3", Vlc::Ratio::R_5_3);
    cmbCropRatio_->addItem("5:4", Vlc::Ratio::R_5_4);

    spinBoxPlayRate_ = new QDoubleSpinBox();
    spinBoxPlayRate_->setObjectName("spinBoxPlayRate");
    spinBoxPlayRate_->setRange(0.f, 50.f);
    spinBoxPlayRate_->setValue(1.f);
    spinBoxPlayRate_->setSingleStep(0.1f);
    spinBoxPlayRate_->setFixedSize(60, 20);

    labelStatusBar_ = new QLabel();
    labelStatusBar_->setObjectName("labelStatusBar");
    labelStatusBar_->setFixedHeight(20);
    labelStatusBar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *hlStatus = new QHBoxLayout();
    hlStatus->setSpacing(6);
    hlStatus->setContentsMargins(5, 0, 5, 0);
    hlStatus->addWidget(labelStatusBar_);
    hlStatus->addWidget(labelAspectRatio);
    hlStatus->addWidget(cmbAspectRatio_);
    hlStatus->addWidget(labelCropRatio);
    hlStatus->addWidget(cmbCropRatio_);
    hlStatus->addWidget(spinBoxPlayRate_);

    QVBoxLayout *vlLeft = new QVBoxLayout();
    vlLeft->setSpacing(5);
    vlLeft->setContentsMargins(0, 0, 0, 0);
    vlLeft->addWidget(vlcWidgetVideo_);
    vlLeft->addLayout(hlControl);
    vlLeft->addLayout(hlStatus);

    QWidget *widgetLeft = new QWidget();
    widgetLeft->setLayout(vlLeft);

    playlistWidget_ = new QListWidget();
    playlistWidget_->setObjectName("playlistWidget");
    playlistWidget_->setSelectionMode(QAbstractItemView::SingleSelection);

    pushButtonAdd_ = new QPushButton();
    pushButtonAdd_->setObjectName("pushButtonAdd");
    pushButtonAdd_->setFixedSize(18, 18);
    pushButtonAdd_->setCursor(QCursor(Qt::PointingHandCursor));

    pushButtonDelete_ = new QPushButton();
    pushButtonDelete_->setObjectName("pushButtonDelete");
    pushButtonDelete_->setFixedSize(18, 18);
    pushButtonDelete_->setCursor(QCursor(Qt::PointingHandCursor));

    pushButtonClear_ = new QPushButton();
    pushButtonClear_->setObjectName("pushButtonClear");
    pushButtonClear_->setFixedSize(18, 18);
    pushButtonClear_->setCursor(QCursor(Qt::PointingHandCursor));

    cmbPlaybackMode_ = new QComboBox();
    cmbPlaybackMode_->setObjectName("cmbPlaybackMode");
    cmbPlaybackMode_->setFixedSize(106, 20);
    cmbPlaybackMode_->addItem("Playlist Once", Vlc::PlaybackMode::DefaultPlayback);
    cmbPlaybackMode_->addItem("Playlist Loop", Vlc::PlaybackMode::Loop);
    cmbPlaybackMode_->addItem("File Loop", Vlc::PlaybackMode::Repeat);
    cmbPlaybackMode_->setCurrentIndex(0);

    QHBoxLayout *hlPlaylistControl = new QHBoxLayout();
    hlPlaylistControl->setSpacing(5);
    hlPlaylistControl->addStretch();
    hlPlaylistControl->addWidget(pushButtonAdd_);
    hlPlaylistControl->addWidget(pushButtonDelete_);
    hlPlaylistControl->addWidget(pushButtonClear_);
    hlPlaylistControl->addWidget(cmbPlaybackMode_);

    QVBoxLayout *vlRight = new QVBoxLayout();
    vlRight->setSpacing(0);
    vlRight->setContentsMargins(0, 0, 0, 0);
    vlRight->addWidget(playlistWidget_);
    vlRight->addLayout(hlPlaylistControl);

    QWidget *widgetRight = new QWidget();
    widgetRight->setLayout(vlRight);

    QSplitter *splitter = new QSplitter();
    splitter->setObjectName("splitter");
    splitter->addWidget(widgetLeft);
    splitter->addWidget(widgetRight);

    splitter->setStretchFactor(0, 4);
    splitter->setStretchFactor(1, 1);

    QHBoxLayout *hlMain = new QHBoxLayout();
    hlMain->setSpacing(0);
    hlMain->setContentsMargins(0, 0, 0, 3);
    hlMain->addWidget(splitter);

    this->setLayout(hlMain);

    QFile qssFile(":/qss/Resource/Player.qss");
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
        QString qss = QString::fromUtf8(qssFile.readAll());
        if (!qss.isEmpty())
            this->setStyleSheet(qss);
        qssFile.close();
    }

    this->resize(960, 600);
}

void PlayerWnd::onPuaseResumeButtonClicked()
{
    Vlc::State state = vlcMediaListPlayer_->mediaPlayer()->state();
    if (state == Vlc::State::Playing) {
        vlcMediaListPlayer_->mediaPlayer()->pause();
    } else if (state == Vlc::State::Paused) {
        vlcMediaListPlayer_->mediaPlayer()->resume();
    } else {
        vlcMediaListPlayer_->play();
    }
}

void PlayerWnd::onStopButtonClicked()
{
    vlcWidgetVideo_->setScale(Vlc::Scale::NoScale);

    if (vlcMediaListPlayer_->mediaPlayer()->state() != Vlc::State::Stopped) {
        vlcMediaListPlayer_->stop();
    }
}

void PlayerWnd::onAddButtonClicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select Files", "", "All(*.*);;");
    if (!filePaths.isEmpty()) {
        foreach (QString item, filePaths) {
            bool isLocalFile = false;
            QFile file(item);
            if (file.open(QFile::ReadOnly)) {
                isLocalFile = true;
                file.close();
            }

            VlcMedia *vlcMedia = new VlcMedia(item, isLocalFile, vlcInstance_);
            vlcMediaListPlayer_->currentMediaList()->addMedia(vlcMedia);
        }
    }
}

void PlayerWnd::onDeleteButtonClicked()
{
    if (vlcMediaList_ && playlistWidget_->currentRow() >= 0) {
        vlcMediaList_->removeMedia(playlistWidget_->currentRow());
    }
}

void PlayerWnd::onClearButtonClicked()
{
    if (vlcMediaList_) {
        vlcMediaList_->removeAllMedia();
    }
}

void PlayerWnd::onPlayRateSpinBoxValueChanged(double value)
{
    VlcMediaPlayer *mediaPlayer = vlcMediaListPlayer_->mediaPlayer();
    if (mediaPlayer)
        mediaPlayer->setPlaybackRate(value);
}

void PlayerWnd::onPlaybackModeCurrentIndexChanged(int index)
{
    if (vlcMediaListPlayer_) {
        vlcMediaListPlayer_->setPlaybackMode((Vlc::PlaybackMode)cmbPlaybackMode_->itemData(index).toInt());
    }
}

void PlayerWnd::onAspectRatioCurrentIndexChanged(int index)
{
    if (vlcWidgetVideo_) {
        vlcWidgetVideo_->setAspectRatio((Vlc::Ratio)cmbAspectRatio_->itemData(index).toInt());
    }
}

void PlayerWnd::onCropRatioCurrentIndexChanged(int index)
{
    if (vlcWidgetVideo_) {
        vlcWidgetVideo_->setCropRatio((Vlc::Ratio)cmbCropRatio_->itemData(index).toInt());
    }
}

void PlayerWnd::onMediaListItemAdded(VlcMedia &item, int index)
{
    if (playlistWidget_) {
        VlcMetaManager metaManager(&item);
        playlistWidget_->insertItem(index, metaManager.title());
    }
}

void PlayerWnd::onMediaListItemDeleted(VlcMedia &item, int index)
{
    Q_UNUSED(item);
    if (playlistWidget_) {
        QListWidgetItem *pListItem = playlistWidget_->takeItem(index);
        if (pListItem)
            delete pListItem;
    }
}

void PlayerWnd::onMediaListPlayerNextItemSet(VlcMedia *media)
{
    if (playlistWidget_) {
        VlcMetaManager metaManager(media);
        QString title = metaManager.title();
        for (int i = 0; i < playlistWidget_->count(); i++) {
            if (playlistWidget_->item(i)->text() == title) {
                playlistWidget_->setCurrentRow(i);
                break;
            }
        }
    }
}

void PlayerWnd::onPlaylistItemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    int index = playlistWidget_->currentRow();
    vlcMediaListPlayer_->itemAt(index);
}

void PlayerWnd::onMediaPlayerStateChanged()
{
    VlcMediaPlayer *mediaPlayer = vlcMediaListPlayer_->mediaPlayer();
    if (!mediaPlayer)
        return;
    Vlc::State state = mediaPlayer->state();

    QString strStatus = QString("%1").arg(QtEnumToString(state));
    labelStatusBar_->setText(strStatus);

    if (state == Vlc::Playing) {
        pushButtonPuaseResume_->setProperty("paused", "N");
    } else if (state == Vlc::Paused) {
        pushButtonPuaseResume_->setProperty("paused", "Y");
    } else if (state == Vlc::Stopped || state == Vlc::Ended) {
        pushButtonPuaseResume_->setProperty("paused", "Y");
    }
    REFRESH_STYLE(pushButtonPuaseResume_);
}
