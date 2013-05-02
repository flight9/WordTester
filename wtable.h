#ifndef WTABLE_H
#define WTABLE_H

#include <QTableWidget>

#include "WSheet.h"

class WTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit WTable(QWidget *parent = 0);
    void setSheet( WSheet* wsheet);
    void refillTable();
    void resetStatusAll();

signals:
    void modified();

public slots:
    void handleItemChanged(QTableWidgetItem* item);
    void deletew();
    void addWord();
    void changeStatus();
    void handleStatusChanged(int id);

private:
    void createContextMenu();
    QTableWidgetSelectionRange selectedRange();
    int addRow();
    void changeStatusByRange(QTableWidgetSelectionRange range, int status);

private:
    //WordArray* wordArray;
    QAction* notestAction;
    QAction* forgottenAction;
    QAction* rememberedAction;
    WSheet* _wsheet;

};

#endif // WTABLE_H
