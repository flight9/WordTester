#include "wtable.h"

#include "sdelegate.h"
#include <QAction>
#include <QMessageBox>

WTable::WTable(QWidget *parent) :
    QTableWidget(parent)
{
    _wsheet = NULL;

    setColumnCount( 3);
    setColumnWidth(0, 110);
    setColumnWidth(1, 85);
    setColumnWidth(2, 120);
    setAlternatingRowColors( true);
    setItemDelegate( new SDelegate(1));  // use my delegate
    setEditTriggers( QAbstractItemView::DoubleClicked);
    setHorizontalHeaderLabels(
            QStringList()<< tr("Word")<< tr("Status")<< tr("Answer"));
    connect( this, SIGNAL(itemChanged(QTableWidgetItem*)),
             this, SLOT(handleItemChanged(QTableWidgetItem*)));

    //
    setSelectionMode( QAbstractItemView::ContiguousSelection);
    setSelectionBehavior( QAbstractItemView::SelectRows);
    //
    createContextMenu();

}

void WTable::setSheet( WSheet* wsheet)
{
    this->_wsheet = wsheet;
}

void WTable::handleItemChanged(QTableWidgetItem* item)
{
    if( item->column() == 0) {
        int row = item->row();
        QString text = item->text();
        if( !text.isEmpty() && _wsheet->textAt(row) != text ) {
            _wsheet->setText(text, row);
            emit modified(); //
        }
    }
    else if( item->column() == 1) {
        int row = item->row();
        QString text = item->text();
        int status = item->text().toInt();
        if( !text.isEmpty() && (int)_wsheet->statusAt(row) != status ) {
            _wsheet->setStatus((Word::WordStatus)status, row);
            emit modified(); //
        }
    }
    else if( item->column() == 2) {
        int row = item->row();
        QString text = item->text();
        if( !text.isEmpty() && _wsheet->answerAt(row) != text ) {
            _wsheet->setAnswer(text, row);
            emit modified(); //
        }
    }
}

void WTable::deletew()
{
    QTableWidgetSelectionRange range = selectedRange();
    if( range.rowCount()<= 0)
        return;

    int re = QMessageBox::question(this, tr("Delete"),
                          tr("Are you sure to delete the word(s)?"),
                          QMessageBox::Yes| QMessageBox::No, QMessageBox::No);
    if( re == QMessageBox::No)
        return;

    for(int i= range.bottomRow(); i>= range.topRow(); --i) {
        removeRow( i);
        //
        _wsheet->eraseAt(i);
    }

    emit modified();
}

void WTable::refillTable()
{
    setRowCount( 0);  //clear rows

    for(int i=0; i< _wsheet->size(); ++i) {
        addRow();
        //
        item( i, 0)->setText( _wsheet->textAt(i));
        item( i, 1)->setText( QString::number((int)_wsheet->statusAt(i)));
        item( i, 2)->setText( _wsheet->answerAt(i));
    }
}

void WTable::resetStatusAll()
{
    QTableWidgetSelectionRange all(0,0,model()->rowCount()-1,0);
    changeStatusByRange(all, Word::NonTest);
}

int WTable::addRow()
{
    int row = rowCount();
    insertRow( row);

    QTableWidgetItem* item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    setItem( row, 0, item0);

    QTableWidgetItem* item1 = new QTableWidgetItem;
    item1->setTextAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    setItem( row, 1, item1);

    QTableWidgetItem* item2 = new QTableWidgetItem;
    item2->setTextAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    setItem( row, 2, item2);

    return row;
}

void WTable::addWord()
{
    Word wd( tr("New word"));
    _wsheet->pushBack( wd);

    int row = addRow();
    item(row, 0)->setText( wd.text());
    item(row, 1)->setText( QString::number(wd.status()));
    item(row, 2)->setText( wd.answer());
    scrollToBottom();
    setCurrentCell( row, 0);
    editItem( item(row, 0));

    emit modified();
}

void WTable::changeStatus()
{
    QTableWidgetSelectionRange range = selectedRange();
    if( range.rowCount()<= 0)
        return;

    int status = Word::NonTest;
    if( sender() == forgottenAction)
        status = Word::Forgotten;
    if( sender() == rememberedAction)
        status = Word::Remembered;

    changeStatusByRange(range, status);
}

void WTable::handleStatusChanged(int id)
{
    if( id>=0 && id< _wsheet->size()) {
        //change display
        item(id, 1)->setData( Qt::DisplayRole, (int)_wsheet->statusAt(id));
        emit modified();
    }
}

void WTable::createContextMenu()
{
    QAction* deleteAction = new QAction(tr("&Delete"), this);
    connect( deleteAction, SIGNAL(triggered()), this, SLOT(deletew()));

    QAction* seperatorAction = new QAction(this);
    seperatorAction->setSeparator(true);

    notestAction = new QAction( tr("Change to &No-Test"), this);
    forgottenAction = new QAction(tr("Change to &Forgotten"), this);
    rememberedAction = new QAction(tr("Change to &Remembered"), this);
    connect( notestAction, SIGNAL(triggered()), this, SLOT(changeStatus()));
    connect( forgottenAction, SIGNAL(triggered()), this, SLOT(changeStatus()));
    connect( rememberedAction, SIGNAL(triggered()), this, SLOT(changeStatus()));

    addAction( deleteAction);
    addAction( seperatorAction);
    addAction( notestAction);
    addAction( forgottenAction);
    addAction( rememberedAction);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

QTableWidgetSelectionRange WTable::selectedRange()
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if( ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();
}

void WTable::changeStatusByRange(QTableWidgetSelectionRange range, int status)
{
    if( range.rowCount()<=0)  return;
    bool changed = false;

    for(int i= range.bottomRow(); i>= range.topRow(); --i) {
        int oldstat = _wsheet->statusAt(i);

        if( oldstat != status) {
            item( i, 1)->setText( QString::number( status));
            _wsheet->setStatus((Word::WordStatus)status, i);
            changed = true;
        }
    }

    if( changed)
        emit modified();
}

