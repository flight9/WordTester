#ifndef SDELEGATE_H
#define SDELEGATE_H

#include <QItemDelegate>

class SDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SDelegate(int statuscolumn, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

public slots:
    void commitAndCloseEditor();

private:
    int statusColumn;

};

#endif // SDELEGATE_H
