#include <QDebug>
#include "private/qmenu_p.h"

/* 为了使用部分Qt没有输出的Private类函数 */

QAction *QMenuPrivate::actionAt(QPoint p) const
{
    if (!q_func()->rect().contains(p))     //sanity check
       return 0;

    for(int i = 0; i < actionRects.count(); i++) {
        if (actionRects.at(i).contains(p))
            return actions.at(i);
    }
    return 0;
}
