#ifndef MDIALOG_H
#define MDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QAction>

#include "WSheet.h"
//#include "../KanboxPlugin/KanboxInterface.h"

class QTableWidgetItem;
class QTableWidgetSelectionRange;

namespace Ui {
    class MDialog;
}

class MDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MDialog(QWidget *parent = 0);
    ~MDialog();

signals:
    void openSignal(const QString& fileName);

private slots:
    void open(const QString& fileName);
    void handleTableModified();
    void openRecentFile();
    void clearRecentActions();
    void handleKanboxDownloaded(const QString&);
    void handleRecentActionClose(QAction*);

private:
    bool okToContinue();
    void loadFile( const QString& fileName, bool append);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    bool saveFile(const QString &fileName);
    bool save();
    bool saveAs();
    void replaceFile(const QString& fileName);
    void appendFile(const QString& fileName);
    void updateRecentActions();
    void writeRegSettings();
    void readRegSettings();
    void createButtonMenus(int);

protected:
    virtual void closeEvent( QCloseEvent * event ) ;
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent* event);

private:
    Ui::MDialog *ui;
    QString _curFile;
    WSheet _wsheet;

private:
    QStringList _recentFiles;
    QList<QAction*> _recentActions;
    QAction* _recentSeparator;
    QAction* _clearRecentAction;
//    KanboxInterface* _kanbox;

private slots:
    void on_addButton_clicked();
    void on_saveButton_clicked();
    void on_testButton_clicked();
    void on_openButton_clicked();
    void on_resetButton_clicked();
    void on_kanboxExecLabel_linkActivated(const QString &link);
};

#endif // MDIALOG_H
