#ifndef HYMNPLAYER_H
#define HYMNPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QDebug>
#include "srtparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HymnPlayer; }
QT_END_NAMESPACE

class HymnPlayer : public QMainWindow
{
    Q_OBJECT

public:
    HymnPlayer(QWidget *parent = nullptr);
    ~HymnPlayer();

private slots:

    void on_PlayButton_pressed();

    void on_PlayButton_toggled(bool checked);

    void on_StopButton_clicked();

    void on_StopButton_pressed();

    void on_positionChanged(qint64 position);

    void on_mediaStatusChanged(QMediaPlayer::MediaStatus status);

    void on_ProgressSlider_sliderReleased();

    void on_VolumeButton_toggled(bool checked);

    void on_VolumeSlider_sliderMoved(int position);

    void on_PreviousButton_pressed();

    void on_PreviousButton_clicked();

    void on_NextButton_pressed();

    void on_NextButton_clicked();

    void on_PlayListButton_pressed();

    void on_PlayListButton_clicked();

    void on_RemoveButton_pressed();

    void on_RemoveButton_clicked();

    void on_AutoplayButton_toggled(bool checked);

    void on_CaptionButton_pressed();

    void on_CaptionButton_toggled(bool checked);

    void on_CaptionEditButton_pressed();

    void on_CaptionEditButton_toggled(bool checked);

    void on_SubStartEndButton_toggled(bool checked);

private:
    Ui::HymnPlayer *ui;
    QMediaPlayer* player;
    QString mediaName;
    QStringList playList;

    QIcon StopIcon;
    QIcon PlayIcon;
    QIcon PauseIcon;
    QIcon VolumeIcon;
    QIcon MuteIcon;
    QIcon PreviousIcon;
    QIcon NextIcon;
    QIcon PlayListIcon;
    QIcon RemoveIcon;
    QIcon AutoplayOnIcon;
    QIcon AutoplayOffIcon;
    QIcon CaptionOffIcon;
    QIcon CaptionOnIcon;
    QIcon CaptionEditIcon;


    qint16 IconSize;
    qint16 Currentplay;
    bool PlayerEnabled;
    QString currentPath;
    QStringList SubTitleTextList;
    QStringList SubTitleList;
    int SubTitileIndex;
    QString SubTitleText, SubStartTime, SubEndTime;
    bool CaptionOnState;

    void load_graphics();
    void update_status_message();
    void Enable_player(bool state);
    void LoadSubTitle(std::string subPath);
    bool IsFileExist(std::string fileName);
    void CaptionEditMode(bool State);

    std::vector<SubtitleItem*> sub;

public:
    void setMedia();
    void load_player();
};
#endif // HYMNPLAYER_H
