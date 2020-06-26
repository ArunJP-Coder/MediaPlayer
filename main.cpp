#include "hymnplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HymnPlayer w;
    w.setWindowTitle("Hymn Player");

    if(argc >= 2)
        w.setMedia();
    w.load_player();

    w.show();
    return a.exec();
}
