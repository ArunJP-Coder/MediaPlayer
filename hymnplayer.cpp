#include "hymnplayer.h"
#include "ui_hymnplayer.h"
#include <QTime>
#include <QFileDialog>
#include <fstream>
#include <QDir>

HymnPlayer::HymnPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HymnPlayer)
{
    ui->setupUi(this);
    ui->SubStartEndButton->setVisible(false);
    currentPath = QDir::homePath() + "/Music";
    if (!QDir(currentPath).exists())
        currentPath = QDir::homePath();

    load_graphics();

    player = new QMediaPlayer(this);
    PlayerEnabled = true;
    CaptionOnState = false;

    connect(player, &QMediaPlayer::positionChanged, this, &HymnPlayer::on_positionChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &HymnPlayer::on_mediaStatusChanged);
}

HymnPlayer::~HymnPlayer()
{
    delete ui;
}

void HymnPlayer::setMedia()
{
    QString path = qApp->arguments().at(1);
    playList << path;
}

void HymnPlayer::load_player()
{
    ui->CaptionButton->setEnabled(false);
    ui->CaptionEditButton->setEnabled(false);
    if ( 0 == playList.size() )
    {
        if(PlayerEnabled)
            Enable_player(false);
        Currentplay = -1;
        player->setMedia(QUrl::fromLocalFile(""));

        if (ui->CaptionButton->isChecked())
            ui->CaptionButton->toggle();

        update_status_message();
        return;
    }

    if (0 > Currentplay)
        Currentplay = playList.size() - 1;
    else if (playList.size() <= Currentplay)
        Currentplay = 0;

    QString mediaPath = playList.at(Currentplay);

    mediaName = mediaPath.section("/",-1);
    mediaName = mediaName.section("\\",-1);
    QString mediaExt = "." + mediaName.section(".",-1);
    mediaName = mediaName.section(mediaExt,0,0);

    player->setMedia(QUrl::fromLocalFile(mediaPath));

    currentPath  = mediaPath.section(mediaName,0,0);
    QString subPath = currentPath + mediaName + ".srt";
    QString textPath = currentPath + mediaName + ".txt";
    LoadSubTitle(subPath.toStdString());

    if ( IsFileExist(textPath.toStdString()) && !IsFileExist(subPath.toStdString()))
        ui->CaptionEditButton->setEnabled(true);
}

void HymnPlayer::LoadSubTitle(std::string subPath)
{
    ui->SubTitleLabel->setText("");

    if (!IsFileExist(subPath))
    {
        if (ui->CaptionButton->isChecked())
            ui->CaptionButton->toggle();
        ui->CaptionButton->setEnabled(false);
        return;
    }

    ui->CaptionButton->setEnabled(true);
    if(CaptionOnState && !ui->CaptionButton->isChecked())
        ui->CaptionButton->toggle();
    SubtitleParserFactory *subParserFactory = new SubtitleParserFactory(subPath);
    SubtitleParser *parser = subParserFactory->getParser();

    sub = parser->getSubtitles();
    ui->SubTitleLabel->move(10,10);
}

bool HymnPlayer::IsFileExist(std::string fileName)
{
    if (FILE *file = std::fopen(fileName.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
        return false;
}

void HymnPlayer::load_graphics()
{
    IconSize = 50;

    QPixmap pixmap(":/images/icons/control_stop.png");
    StopIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_play.png");
    PlayIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_pause.png");
    PauseIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_volume.png");
    VolumeIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_mute.png");
    MuteIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_previous.png");
    PreviousIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_next.png");
    NextIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_playlist.png");
    PlayListIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_remove.png");
    RemoveIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_autoplayon.png");
    AutoplayOnIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_autoplayoff.png");
    AutoplayOffIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_captionoff.png");
    CaptionOffIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_captionon.png");
    CaptionOnIcon = pixmap;

    pixmap = QPixmap(":/images/icons/control_captionedit.png");
    CaptionEditIcon = pixmap;

    ui->PlayButton->setIcon(PlayIcon);
    ui->PlayButton->setIconSize(QSize(IconSize + 5,IconSize + 5));

    ui->VolumeButton->setIcon(VolumeIcon);
    ui->VolumeButton->setIconSize(QSize(IconSize,IconSize));

    ui->StopButton->setIcon(StopIcon);
    ui->StopButton->setIconSize(QSize(IconSize,IconSize));

    ui->PreviousButton->setIcon(PreviousIcon);
    ui->PreviousButton->setIconSize(QSize(IconSize,IconSize));

    ui->NextButton->setIcon(NextIcon);
    ui->NextButton->setIconSize(QSize(IconSize,IconSize));

    ui->PlayListButton->setIcon(PlayListIcon);
    ui->PlayListButton->setIconSize(QSize(IconSize,IconSize));

    ui->RemoveButton->setIcon(RemoveIcon);
    ui->RemoveButton->setIconSize(QSize(IconSize,IconSize));

    ui->AutoplayButton->setIcon(AutoplayOnIcon);
    ui->AutoplayButton->setIconSize(QSize(IconSize,IconSize));

    ui->CaptionButton->setIcon(CaptionOffIcon);
    ui->CaptionButton->setIconSize(QSize(IconSize,IconSize));

    ui->CaptionEditButton->setIcon(CaptionEditIcon);
    ui->CaptionEditButton->setIconSize(QSize(IconSize,IconSize));

}

void HymnPlayer::Enable_player(bool State)
{
    ui->PlayButton->setEnabled(State);
    ui->StopButton->setEnabled(State);
    ui->PreviousButton->setEnabled(State);
    ui->NextButton->setEnabled(State);
    ui->RemoveButton->setEnabled(State);
    ui->ProgressSlider->setEnabled(State);
    ui->ProgressSlider->setEnabled(State);

    PlayerEnabled = State;
}

void HymnPlayer::CaptionEditMode(bool State)
{
    ui->SubStartEndButton->setVisible(State);
    ui->SubStartEndButton->setEnabled(State);
    State = !State;
    ui->PreviousButton->setVisible(State);
    ui->NextButton->setVisible(State);
    ui->RemoveButton->setVisible(State);
    ui->PlayListButton->setVisible(State);
    ui->StopButton->setVisible(State);
    ui->CaptionButton->setVisible(State);
    ui->AutoplayButton->setVisible(State);

    ui->ProgressSlider->setEnabled(State);
    ui->AutoplayButton->setEnabled(State);
}

void HymnPlayer::update_status_message()
{
    ui->statusbar->clearMessage();
    if (QMediaPlayer::NoMedia != player->mediaStatus())
    {
        if (QMediaPlayer::PlayingState == player->state())
            ui->statusbar->showMessage("Playing \"" + mediaName + "\".");
        else if (QMediaPlayer::PausedState == player->state())
            ui->statusbar->showMessage("Paused \"" + mediaName + "\".");
        else if (QMediaPlayer::StoppedState == player->state())
            ui->statusbar->showMessage("Loaded \"" + mediaName + "\".");
            //ui->statusBar->showMessage("Media Stoped.", 3000);
    }
    else
        ui->statusbar->showMessage("No Media Loaded.");

}

void HymnPlayer::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status)
    {
    case QMediaPlayer::LoadingMedia :
    {
        if(PlayerEnabled)
            Enable_player(false);

        ui->DurationLable->setText("--:--");
        ui->statusbar->showMessage("Loading...");

        break;
    }
    case QMediaPlayer::LoadedMedia :
    {
        ui->ProgressSlider->setMaximum(player->duration());

        if(!PlayerEnabled)
            Enable_player(true);

        ui->TimeLable->setText("00:00");
        QTime t(0,0,0);
        t = t.addMSecs(player->duration());
        ui->DurationLable->setText( t.toString("mm:ss") );

        if (ui->AutoplayButton->isChecked())
            ui->PlayButton->toggle();

        update_status_message();
        break;
    }
    case QMediaPlayer::EndOfMedia :
    {
        if( ui->CaptionEditButton->isChecked() )
            ui->CaptionEditButton->click();
        else
            ui->NextButton->click();
        ui->PlayButton->toggle();
        break;
    }
    default :
        break;
    }
}

void HymnPlayer::on_positionChanged(qint64 position)
{
    ui->ProgressSlider->setValue(position);

    QTime t(0,0,0);
    t = t.addMSecs(position);
    ui->TimeLable->setText( t.toString("mm:ss") );

    if (ui->CaptionButton->isChecked())
    {
        for(SubtitleItem * element : sub)
        {
            double startTime = element->getStartTime();
            double endTime = element->getEndTime();
            if( (startTime <= position) && (position <= endTime))
            {
                ui->SubTitleLabel->setText(QString::fromStdString(element->getText()));
                break;
            }
            else
                ui->SubTitleLabel->setText("");
        }
    }
}

void HymnPlayer::on_ProgressSlider_sliderReleased()
{
    qint64 position = ui->ProgressSlider->sliderPosition();
    player->setPosition(position);
}

void HymnPlayer::on_PlayButton_pressed()
{
    ui->PlayButton->setIconSize(QSize(IconSize + 2,IconSize + 2));
}

void HymnPlayer::on_PlayButton_toggled(bool checked)
{
    QIcon Icon;
    if (checked)
    {
        Icon = PauseIcon;
        player->play();
        update_status_message();
    }
    else
    {
        Icon = PlayIcon;
        if (QMediaPlayer::PlayingState == player->state())
            player->pause();
        update_status_message();
    }

    ui->PlayButton->setIcon(Icon);
    ui->PlayButton->setIconSize(QSize(IconSize + 5,IconSize + 5));
}

void HymnPlayer::on_AutoplayButton_toggled(bool checked)
{
    QIcon Icon;
    if (checked)
        Icon = AutoplayOnIcon;
    else
        Icon = AutoplayOffIcon;
    ui->AutoplayButton->setIcon(Icon);
}

void HymnPlayer::on_StopButton_pressed()
{
    ui->StopButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
}

void HymnPlayer::on_StopButton_clicked()
{
    ui->StopButton->setIconSize(QSize(IconSize,IconSize));

    if (QMediaPlayer::PlayingState == player->state())
        ui->PlayButton->toggle();

    player->stop();

    if (0 < ui->ProgressSlider->sliderPosition())
    {
        ui->ProgressSlider->setValue(0);
        player->setPosition(0);
    }
}

void HymnPlayer::on_VolumeButton_toggled(bool checked)
{
    QIcon Icon;
    if (checked)
    {
        Icon = MuteIcon;
        if ( 0 != player->volume())
        {
            player->setMuted(true);
            ui->VolumeSlider->setEnabled(false);
        }
    }
    else
    {
        Icon = VolumeIcon;
        player->setMuted(false);
        ui->VolumeSlider->setEnabled(true);
    }
    ui->VolumeButton->setIcon(Icon);
}

void HymnPlayer::on_VolumeSlider_sliderMoved(int position)
{
    player->setVolume(position);

    if(0 == position && ! ui->VolumeButton->isChecked())
        ui->VolumeButton->toggle();
    else if (0 < position && ui->VolumeButton->isChecked())
        ui->VolumeButton->toggle();
}

void HymnPlayer::on_PreviousButton_pressed()
{
    ui->PreviousButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
}

void HymnPlayer::on_PreviousButton_clicked()
{
    ui->StopButton->click();

    ui->PreviousButton->setIconSize(QSize(IconSize,IconSize));
    Currentplay--;
    load_player();
}

void HymnPlayer::on_NextButton_pressed()
{
    ui->NextButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
}

void HymnPlayer::on_NextButton_clicked()
{
    ui->StopButton->click();

    ui->NextButton->setIconSize(QSize(IconSize,IconSize));
    Currentplay++;
    load_player();
}

void HymnPlayer::on_PlayListButton_pressed()
{
    ui->PlayListButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
}

void HymnPlayer::on_PlayListButton_clicked()
{
    ui->PlayListButton->setIconSize(QSize(IconSize,IconSize));

    QString a = currentPath;
    QString MediaPath = QFileDialog::getOpenFileName(this, "Choose Media", currentPath, "Audio(*.mp3)" );
    if (MediaPath == "")
        return;

    playList << MediaPath;

    if (QMediaPlayer::NoMedia == player->mediaStatus())
        load_player();
}

void HymnPlayer::on_RemoveButton_pressed()
{
    ui->RemoveButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
}

void HymnPlayer::on_RemoveButton_clicked()
{
    ui->RemoveButton->setIconSize(QSize(IconSize,IconSize));
    playList.removeAt(Currentplay);
    ui->StopButton->click();

    if (ui->AutoplayButton->isChecked())
        ui->AutoplayButton->toggle();

    ui->NextButton->click();
}

void HymnPlayer::on_CaptionButton_pressed()
{
    ui->CaptionButton->setIconSize(QSize(IconSize - 3,IconSize - 3));
    if(!ui->CaptionButton->isChecked())
        CaptionOnState = true;
    else
        CaptionOnState = false;
}

void HymnPlayer::on_CaptionButton_toggled(bool checked)
{
    ui->CaptionButton->setIconSize(QSize(IconSize,IconSize));
    QIcon Icon;
    if (checked)
        Icon = CaptionOnIcon;
    else
    {

        Icon = CaptionOffIcon;
        ui->SubTitleLabel->setText("");
    }
    ui->CaptionButton->setIcon(Icon);
}

void HymnPlayer::on_CaptionEditButton_pressed()
{
    ui->CaptionEditButton->setIconSize(QSize(IconSize-3,IconSize-3));
}

void HymnPlayer::on_CaptionEditButton_toggled(bool checked)
{
    ui->CaptionEditButton->setIconSize(QSize(IconSize,IconSize));
    if(ui->AutoplayButton->isChecked())
        ui->AutoplayButton->click();
    ui->StopButton->click();
    if (checked)
    {
        QFile SubTxtFile(currentPath + mediaName + ".txt");
        if (SubTxtFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&SubTxtFile);
            in.setCodec("UTF-8");
            bool PreLineEmpty = true;
            while(!in.atEnd())
            {
                QString line = in.readLine();
                line = line.simplified();
                line = line.remove( "\t" );
                if (!line.isEmpty())
                {
                    if (PreLineEmpty)
                        SubTitleTextList.append(line);
                    else
                        SubTitleTextList[SubTitleTextList.size()-1].append(line);
                    PreLineEmpty = false;
                }
                else
                    PreLineEmpty = true;
            }
            SubTxtFile.close();
        }
        CaptionEditMode(true);

        SubTitileIndex = 0;
        ui->SubTitleLabel->setText(SubTitleTextList[SubTitileIndex]);
    }
    else
    {
        ui->SubTitleLabel->setText("");
        if (4 <= SubTitleList.size())
        {
            QString srtFile = currentPath + mediaName + ".srt";
            QFile file( srtFile );
            if ( file.open(QIODevice::ReadWrite) )
            {
                QTextStream stream( &file );
                for (int index = 0; SubTitleList.size() > index; index++)
                {
                    stream << SubTitleList[index] << endl;
                }
            }
        }
        SubTitleList.clear();
        SubTitleTextList.clear();
        CaptionEditMode(false);
        ui->CaptionEditButton->setEnabled(false);
        CaptionOnState = true;
        load_player();
    }
}

void HymnPlayer::on_SubStartEndButton_toggled(bool checked)
{
    qint64 StartTime, EndTime;

    if (checked)
    {
        StartTime = ui->ProgressSlider->sliderPosition();
        QTime t(0,0,0);
        t = t.addMSecs(StartTime);
        SubStartTime = t.toString("hh:mm:ss,zzz");

        ui->SubStartEndButton->setText("End");
        SubTitleText = SubTitleTextList[SubTitileIndex];
    }
    else
    {
        EndTime = ui->ProgressSlider->sliderPosition();
        QTime t(0,0,0);
        t = t.addMSecs(EndTime);
        SubEndTime = t.toString("hh:mm:ss,zzz");

        if(SubTitileIndex + 1 >= SubTitleTextList.size())
        {
            ui->SubStartEndButton->setEnabled(false);
            ui->SubStartEndButton->setText("Done");
        }
        else
        {
            ui->SubTitleLabel->setText(SubTitleTextList[++SubTitileIndex]);
            ui->SubStartEndButton->setText("Start");
        }

        SubTitleList.append(QString::number(SubTitileIndex));
        SubTitleList.append(SubStartTime + " --> " + SubEndTime);
        SubTitleList.append(SubTitleText);
        SubTitleList.append("");
    }
}
