#ifndef XACTIONMENU_H
#define XACTIONMENU_H

#include <QMenu>

class XActionMenu : public QMenu
{
    Q_OBJECT
public:
    explicit XActionMenu(QWidget *parent = 0);
    explicit XActionMenu(const QString &title, QWidget *parent = 0);
    virtual ~XActionMenu()  {}

public:
    enum {
        XButtonSize = 8,
        XButtonMargin = 1
    };

signals:
    void actionClose(QAction*);

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QVector<QRect> _xbuttonRects;
};

#endif // XACTIONMENU_H
