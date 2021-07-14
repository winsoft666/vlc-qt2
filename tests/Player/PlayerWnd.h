#ifndef PLAYER_WND_H__
#define PLAYER_WND_H__

#include <QtWidgets/QtWidgets>

class VlcInstance;
class VlcMedia;
class VlcMediaList;
class VlcMediaPlayer;
class VlcMediaListPlayer;
class VlcWidgetVideo;
class VlcWidgetSeek;
class VlcWidgetVolumeSlider;

class PlayerWnd : public QWidget
{
    Q_OBJECT
public:
    PlayerWnd(QWidget *parent = 0);
    ~PlayerWnd();

protected:
    void setupUi();
private slots:
    void onPuaseResumeButtonClicked();
    void onStopButtonClicked();
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onClearButtonClicked();
    void onPlayRateSpinBoxValueChanged(double value);
    void onPlaybackModeCurrentIndexChanged(int index);
    void onAspectRatioCurrentIndexChanged(int index);
    void onCropRatioCurrentIndexChanged(int index);

    void onPlaylistItemDoubleClicked(QListWidgetItem *item);
private slots:
    void onMediaPlayerStateChanged();
    void onMediaListItemAdded(VlcMedia &item, int index);
    void onMediaListItemDeleted(VlcMedia &item, int index);
    void onMediaListPlayerNextItemSet(VlcMedia *media);
private:
private:
    VlcWidgetVideo *vlcWidgetVideo_;
    VlcWidgetSeek *vlcWidgetSeek_;
    VlcWidgetVolumeSlider *vlcWidgetVolSlider_;
    QPushButton *pushButtonPuaseResume_;
    QPushButton *pushButtonStop_;

    QPushButton *pushButtonAdd_;
    QPushButton* pushButtonDelete_;
    QPushButton *pushButtonClear_;
    QLabel *labelStatusBar_;

    QDoubleSpinBox *spinBoxPlayRate_;
    QComboBox *cmbAspectRatio_;
    QComboBox *cmbCropRatio_;
    QComboBox *cmbPlaybackMode_;
    QListWidget *playlistWidget_;

private:
    VlcInstance *vlcInstance_;
    VlcMediaList *vlcMediaList_;
    VlcMediaPlayer *vlcMediaPlayer_;
    VlcMediaListPlayer *vlcMediaListPlayer_;
};

#endif // PLAYER_WND_H__
