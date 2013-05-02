#include "sdelegate.h"

#include "word.h"
#include <QComboBox>

SDelegate::SDelegate(int statuscolumn, QObject *parent) :
    QItemDelegate(parent)
{
    this->statusColumn = statuscolumn;
}

void SDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.column() == statusColumn) {
        // display in view
        int status = index.model()->data(index, Qt::DisplayRole).toInt();
        if( status > 0)
            status = Word::Remembered;
        else if( status < 0)
            status = Word::NonTest;
        else
            status = Word::Forgotten;
        QString text = Word::statusName( status);

        // draw as plain text
        QStyleOptionViewItem myOption = option;
        drawDisplay( painter, myOption, myOption.rect, text);
        drawFocus( painter, myOption, myOption.rect);
    }
    else {
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget* SDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.column() == statusColumn) {
        // create my editor, but without data
        QComboBox* combox = new QComboBox(parent);
        combox->addItem( Word::statusName( Word::NonTest), Word::NonTest);
        combox->addItem( Word::statusName( Word::Forgotten), Word::Forgotten);
        combox->addItem( Word::statusName( Word::Remembered), Word::Remembered);
        connect( combox, SIGNAL(currentIndexChanged(int)),
                 this, SLOT(commitAndCloseEditor()));

        return combox;
    }
    else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void SDelegate::commitAndCloseEditor()
{
    QComboBox* editor = qobject_cast<QComboBox*>( sender());
    emit commitData( editor);  //write to model
    emit closeEditor( editor); //close this editor
}

void SDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if( index.column() == statusColumn) {
        // fill my editor with data
        int status = index.model()->data(index, Qt::DisplayRole).toInt();
        if( status > 0)
            status = Word::Remembered;
        else if( status < 0)
            status = Word::NonTest;
        else
            status = Word::Forgotten;

        QComboBox* combox = qobject_cast<QComboBox*>( editor);
        combox->setCurrentIndex( status+1);
    }
    else {
        QItemDelegate::setEditorData( editor, index);
    }
}

void SDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( index.column() == statusColumn) {
        QComboBox* combox = qobject_cast<QComboBox*>( editor);
        int cursel = combox->currentIndex();
        int status = cursel- 1;
        model->setData(index, status, Qt::DisplayRole);
    }
    else {
        QItemDelegate::setModelData( editor, model, index);
    }
}
