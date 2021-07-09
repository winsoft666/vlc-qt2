#ifndef PLAYER_WND_H__
#define PLAYER_WND_H__

#include <QtWidgets/QtWidgets>

class VlcInstance;
class VlcMediaPlayer;
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

private:
    VlcWidgetVideo *vlcWidgetVideo_;
    VlcWidgetSeek *vlcWidgetSeek_;
    VlcWidgetVolumeSlider *vlcWidgetVolSlider_;

private:
    VlcInstance *vlcInstance_;
    VlcMediaPlayer *vlcPlayer_;
};

#endif // PLAYER_WND_H__
