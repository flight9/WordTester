#include "XActionMenu.h"

#include <QPainter>
#include <QDebug>
#include "private/qmenu_p.h"


XActionMenu::XActionMenu(QWidget *parent) :
    QMenu(parent)
{
}

XActionMenu::XActionMenu(const QString &title, QWidget *parent) :
    QMenu( title, parent)
{
}

void XActionMenu::paintEvent(QPaintEvent *e)
{
    //need base draw
    QMenu::paintEvent(e);

    QMenuPrivate* d = reinterpret_cast<QMenuPrivate*>(qGetPtrHelper(d_ptr));
    QPainter p(this);

    //draw the X button
    _xbuttonRects.clear();
    for (int i = 0; i < d->actions.count(); ++i) {
        QAction *action = d->actions.at(i);
        QRect adjustedActionRect = d->actionRects.at(i);
        bool hasXButton = action->property("HasXButton").toBool();
        if (!e->rect().intersects(adjustedActionRect)
                || d->widgetItems.value(action)
                || !hasXButton )
           continue;

        QRect buttonRect(adjustedActionRect.right()-XButtonMargin-XButtonSize,
                         adjustedActionRect.top()+XButtonMargin,
                         XButtonSize, XButtonSize);
        //set the clip region to be extra safe (and adjust for the scrollers)
        QRegion adjustedActionReg(buttonRect);
        p.setClipRegion(adjustedActionReg);

        QRect adjustedBtnRect = buttonRect.adjusted(0,0,-1,-1);
        p.drawRect(adjustedBtnRect);
        p.drawLine(adjustedBtnRect.topLeft(), adjustedBtnRect.bottomRight());
        p.drawLine(adjustedBtnRect.bottomLeft()+QPoint(0,1),
                   adjustedBtnRect.topRight()+QPoint(0,1));

        _xbuttonRects.append(buttonRect);
    }
}

void XActionMenu::mouseReleaseEvent(QMouseEvent *e)
{
    const QMenuPrivate* d = reinterpret_cast<const QMenuPrivate*>(qGetPtrHelper(d_ptr));
    for(int i=0; i< _xbuttonRects.size(); ++i) {
        QRect r = _xbuttonRects.at(i);
        if( r.contains(e->pos())) {
            QAction* a = d->actionAt(e->pos());
            if( a != NULL)
                emit actionClose( a);
            return;
        }
    }
    QMenu::mouseReleaseEvent(e);
}
