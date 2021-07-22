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

#ifndef VLCQT_WIDGETVIDEO_H_
#define VLCQT_WIDGETVIDEO_H_

#include <QtCore/QTimer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QFrame>
#else
#include <QtGui/QFrame>
#endif

#include "core/Enums.h"
#include "core/VideoDelegate.h"

#include "SharedExportWidgets.h"

class VlcMediaPlayer;

/*!
    \defgroup VLCQtWidgets VLC-Qt Widgets (VLCQtWidgets)
    \brief Widget classes for faster media player developement
 */

/*!
    \class VlcWidgetVideo WidgetVideo.h VLCQtWidgets/WidgetVideo.h
    \ingroup VLCQtWidgets
    \brief Video widget

    This is one of VLC-Qt GUI classes.
    It provides video display and mouse control.
*/
class VLCQT_WIDGETS_EXPORT VlcWidgetVideo : public QFrame, public VlcVideoDelegate
{
    Q_OBJECT
public:
    /*!
        \brief VlcWidgetVideo constructor
        \param player media player
        \param parent video widget's parent GUI widget
    */
    explicit VlcWidgetVideo(VlcMediaPlayer *player,
                            QWidget *parent = 0);

    /*!
        \brief VlcWidgetVideo constructor
        \param parent video widget's parent GUI widget
    */
    explicit VlcWidgetVideo(QWidget *parent = 0);

    /*!
        \brief VlcWidgetVideo destructor
    */
    ~VlcWidgetVideo();

    /*!
        \brief Get current aspect ratio setting
        \return current aspect ratio
    */
    inline Vlc::Ratio currentAspectRatio() const { return _currentAspectRatio; }

    /*!
        \brief Get current crop ratio setting
        \return current crop ratio
    */
    inline Vlc::Ratio currentCropRatio() const { return _currentCropRatio; }

    /*!
        \brief Get current deinterlacing filter setting
        \return current deinterlacing filter
    */
    inline Vlc::Deinterlacing currentDeinterlacing() const { return _currentDeinterlacing; }

    /*!
        \brief Get current scale ratio setting
        \return current scale ratio
    */
    inline Vlc::Scale currentScale() const { return _currentScale; }

    /*!
        \brief Set media player if initialised without it
        \param player media player
    */
    void setMediaPlayer(VlcMediaPlayer *player);

    /*!
        \brief Request new video frame.

        Request new video frame and its widget ID to set it in the instance.
        \return widget ID
        \sa VlcMediaPlayer::VlcMediaPlayer()
    */
    WId request();

    /*!
        \brief Release current video frame.
    */
    void release();

public slots:
    /*!
        \brief Set aspect ratio
        \param ratio aspect ratio
    */
    void setAspectRatio(const Vlc::Ratio &ratio);

    /*!
    \brief Set custom aspect ratio
    \param ratio aspect ratio string, such as 500:400
    */
    void setCustomAspectRatio(const QString &ratio);

    /*!
        \brief Set crop ratio
        \param ratio crop ratio
    */
    void setCropRatio(const Vlc::Ratio &ratio);

    /*!
        \brief Set deinterlace filter
        \param deinterlacing deinterlacing filter
    */
    void setDeinterlacing(const Vlc::Deinterlacing &deinterlacing);

    /*!
        \brief Set scale ratio
        \param scale scale ratio
    */
    void setScale(const Vlc::Scale &scale);

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
    void applyPreviousSettings();

private:
    void initWidgetVideo();
    void sync();

    VlcMediaPlayer *_vlcMediaPlayer;

    QWidget *_video;
    QLayout *_layout;

    Vlc::Ratio _currentAspectRatio;
    QString _currentCustomAspectRatio;
    Vlc::Ratio _currentCropRatio;
    Vlc::Deinterlacing _currentDeinterlacing;
    Vlc::Scale _currentScale;
};

#endif // VLCQ_WIDGETVIDEO_H_
