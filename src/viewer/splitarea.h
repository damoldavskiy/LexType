#ifndef SPLITAREA_H
#define SPLITAREA_H

#include <QImage>

namespace Viewer {

struct SplitArea
{
    QImage image;
    int dx;
    int dy;
    int page;
};

}

#endif // SPLITAREA_H
