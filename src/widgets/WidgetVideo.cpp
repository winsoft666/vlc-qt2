/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolBar>
#endif

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#include <qx11info_x11.h>
#endif

#include "core/Error.h"
#include "core/MediaPlayer.h"
#include "core/Video.h"

#include "widgets/WidgetVideo.h"

VlcWidgetVideo::VlcWidgetVideo(VlcMediaPlayer *player,
                               QWidget *parent)
    : QFrame(parent)
{
    _vlcMediaPlayer = player;

    if (_vlcMediaPlayer) {
        connect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
    }

    initWidgetVideo();
}

VlcWidgetVideo::VlcWidgetVideo(QWidget *parent)
    : QFrame(parent),
      _vlcMediaPlayer(Q_NULLPTR)
{
    initWidgetVideo();
}

VlcWidgetVideo::~VlcWidgetVideo()
{
    release();
}

void VlcWidgetVideo::initWidgetVideo()
{
    _currentAspectRatio = Vlc::Original;
    _currentCropRatio = Vlc::Original;
    _currentDeinterlacing = Vlc::Disabled;
    _currentScale = Vlc::NoScale;
    _currentCustomAspectRatio = "UNUSED";

    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = Q_NULLPTR;

    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    setPalette(plt);
}

void VlcWidgetVideo::setMediaPlayer(VlcMediaPlayer *player)
{
    if (_vlcMediaPlayer) {
        disconnect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
    }

    _vlcMediaPlayer = player;

    if (_vlcMediaPlayer) {
        connect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
    }
}

void VlcWidgetVideo::applyPreviousSettings()
{
    if (!_vlcMediaPlayer)
        return;

    if (_currentCustomAspectRatio != "UNUSED") {
        _vlcMediaPlayer->video()->setCustomAspectRatio(_currentCustomAspectRatio);
    } else {
        if (_currentAspectRatio == Vlc::Ratio::KeepStretch) {
            QString strRatio = QString("%1:%2").arg(this->width()).arg(this->height());
            _vlcMediaPlayer->video()->setCustomAspectRatio(strRatio);
        } else {
            _vlcMediaPlayer->video()->setAspectRatio(_currentAspectRatio);
        }
    }

    _vlcMediaPlayer->video()->setCropGeometry(_currentCropRatio);
    _vlcMediaPlayer->video()->setScale(_currentScale);
    _vlcMediaPlayer->video()->setDeinterlace(_currentDeinterlacing);
}

void VlcWidgetVideo::setAspectRatio(const Vlc::Ratio &ratio)
{
    if (_vlcMediaPlayer) {
        _currentAspectRatio = ratio;
        if (ratio == Vlc::Ratio::KeepStretch) {
            QString strRatio = QString("%1:%2").arg(this->width()).arg(this->height());
            _vlcMediaPlayer->video()->setCustomAspectRatio(strRatio);
        } else {
            _vlcMediaPlayer->video()->setAspectRatio(ratio);
        }
    }
}

void VlcWidgetVideo::setCustomAspectRatio(const QString &ratio)
{
    if (_vlcMediaPlayer) {
        _currentCustomAspectRatio = ratio;
        _vlcMediaPlayer->video()->setCustomAspectRatio(ratio);
    }
}

void VlcWidgetVideo::setCropRatio(const Vlc::Ratio &ratio)
{
    if (_vlcMediaPlayer) {
        _currentCropRatio = ratio;
        _vlcMediaPlayer->video()->setCropGeometry(ratio);
    }
}

void VlcWidgetVideo::setDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    if (_vlcMediaPlayer) {
        _currentDeinterlacing = deinterlacing;
        _vlcMediaPlayer->video()->setDeinterlace(deinterlacing);
    }
}

void VlcWidgetVideo::setScale(const Vlc::Scale &scale)
{
    if (_vlcMediaPlayer) {
        _currentScale = scale;
        _vlcMediaPlayer->video()->setScale(scale);
    }
}

void VlcWidgetVideo::resizeEvent(QResizeEvent *event)
{
    if (_currentAspectRatio == Vlc::Ratio::KeepStretch && _vlcMediaPlayer) {
        if (_vlcMediaPlayer->video()) {
            QString strRatio = QString("%1:%2").arg(this->width()).arg(this->height());
            _vlcMediaPlayer->video()->setCustomAspectRatio(strRatio);
        }
    }
    QWidget::resizeEvent(event);
}

void VlcWidgetVideo::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId VlcWidgetVideo::request()
{
    if (_video)
        return 0;

    _video = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    _video->setPalette(plt);
    _video->setAutoFillBackground(true);
    _video->setMouseTracking(true);
/* Indicates that the widget wants to draw directly onto the screen.
       Widgets with this attribute set do not participate in composition
       management */
/* This is currently disabled on X11 as it does not seem to improve
     * performance, but causes the video widget to be transparent... */
#if QT_VERSION < 0x050000 && !defined(Q_WS_X11)
    _video->setAttribute(Qt::WA_PaintOnScreen, true);
#endif

    _layout->addWidget(_video);

    sync();
    return _video->winId();
}

void VlcWidgetVideo::release()
{
    if (_video) {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }

    updateGeometry();
}
