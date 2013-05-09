#include "mdialog.h"
#include "ui_mdialog.h"

#include <QMessageBox>
#include <QTableWidgetSelectionRange>
#include <QSettings>
#include <QPluginLoader>
#include <QUrl>
#include <QFileDialog>
#include <QProgressDialog>
#include <QCloseEvent>

#include "dialog.h"
#include "sdelegate.h"
#include "menu/XActionMenu.h"
#include "main.h"

MDialog::MDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MDialog)
{
    ui->setupUi(this);

    //
    setWindowFlags( Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    setWindowTitle(tr("Word Tester[*]"));

    //
    QSettings set("WordTester.cfg", QSettings::IniFormat);
    set.beginGroup("MDialog");
    int MaxRecentNum = set.value("MaxRecentNum", 4).toInt();
    set.endGroup();
    createButtonMenus(MaxRecentNum);

    //
    readRegSettings();

    //
    ui->tableWidget->setSheet(&_wsheet);
    connect( ui->tableWidget, SIGNAL(modified()),
             this, SLOT(handleTableModified()));
    connect( this, SIGNAL(openSignal(QString)),
             this, SLOT(open(QString)), Qt::QueuedConnection);

    //
    setAcceptDrops( true);

    //
//    _kanbox = NULL;

}

MDialog::~MDialog()
{
    delete ui;
}

bool MDialog::okToContinue()
{
    if( isWindowModified()) {
        int r= QMessageBox::warning(this, tr("Word Tester"),
                                    tr("The list has been modified. \n"
                                       "Do you want to save your changes?"),
                                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if( r== QMessageBox::Yes) {
            return save();
        }
        else if( r== QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MDialog::on_openButton_clicked()
{
    if( okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName( this,
                                     tr("Open List"), ".",
                                     tr("Word list file (*.list)\n"
                                        "Text file (*.txt)\n"
                                        "All files (*.*)"));
        if( !fileName.isEmpty())
            open( fileName);
    }
}

void MDialog::open(const QString& fileName)
{
    if( 0 == QString::compare( _curFile, fileName, Qt::CaseInsensitive))
        return;

    if( _wsheet.size() > 0) {
            int ap = QMessageBox::question(
                        this, tr("Word Tester"),
                        tr("Do you want to append new contents to this list?<br>"
                           "<font color=grey>(If you don't want to mix these two list to "
                           "test, please select 'No'.)</font>"),
                        QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
            if( ap == QMessageBox::Yes)
                appendFile( fileName);
            else
                replaceFile( fileName);
    }
    else {
        replaceFile( fileName);
    }
}

void MDialog::replaceFile(const QString& fileName)
{
    loadFile( fileName, false);
}

void MDialog::appendFile(const QString& fileName)
{
    loadFile( fileName, true);
}

void MDialog::loadFile( const QString& fileName, bool append)
{
    bool loaded = _wsheet.loadFile(fileName, append);
    if( loaded) {
        if( !append)
            setCurrentFile( fileName);
        else
            setCurrentFile( QString::null);
        ui->tableWidget->refillTable();
    }
    else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Open file failed!"));
    }
}

void MDialog::setCurrentFile(const QString &fileName)
{
    _curFile = fileName;
    QString showName = tr("Untitled");
    if( !fileName.isEmpty()) {
        showName = strippedName( fileName);
        setWindowModified(false);
        //
        if( !_recentFiles.contains(_curFile, Qt::CaseInsensitive)) {
            _recentFiles.prepend( _curFile);
            while( _recentFiles.size()> _recentActions.size())  //MaxRecentFiles
                _recentFiles.removeLast();
            updateRecentActions();
        }
    }
    else {
        setWindowModified(true);
    }
    setWindowTitle(tr("%1[*] - %2")
                   .arg(showName)
                   .arg(tr("Word Tester")));
}

QString MDialog::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MDialog::updateRecentActions()
{   //borrowed from Page 48
    QMutableStringListIterator ir(_recentFiles);
    while( ir.hasNext()) {
        if( !QFile::exists(ir.next()))
            ir.remove();
    }
    for(int i=0; i<_recentActions.size(); i++) {  //<MaxRecentFiles
        if( i< _recentFiles.size()) {
            QString text = tr("&%1 %2")
                    .arg(i+1)
                    .arg(this->strippedName(_recentFiles[i]));
            _recentActions[i]->setText( text);
            _recentActions[i]->setData( _recentFiles[i]);
            _recentActions[i]->setVisible( true);
        }
        else {
            _recentActions[i]->setVisible( false);
        }
    }
    _recentSeparator->setVisible(_recentFiles.size()>0);
    _clearRecentAction->setVisible(_recentFiles.size()>0);
}

void MDialog::writeRegSettings()
{
    QSettings setti("Liangzm Software", "WordTester");
    setti.setValue("recentFiles", _recentFiles);
}

void MDialog::readRegSettings()
{
    QSettings setti("Liangzm Software", "WordTester");
    _recentFiles = setti.value("recentFiles").toStringList();
    updateRecentActions();
}

void MDialog::createButtonMenus(int MaxRecentNum)
{
    if( MaxRecentNum<2)  MaxRecentNum = 2;
    if( MaxRecentNum>10) MaxRecentNum = 10;

    XActionMenu* recentMenu = new XActionMenu(tr("&Recent Files"), this);
    for(int i=0; i<MaxRecentNum; i++) {
        QAction* action = new QAction(this);
        action->setVisible(false);
        action->setProperty("HasXButton", true);
        connect(action, SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
        recentMenu->addAction( action);
        _recentActions.append( action);
    }
    _recentSeparator =  recentMenu->addSeparator();
    _recentSeparator->setVisible(false);
    _clearRecentAction = recentMenu->addAction(tr("&Clear"), this, SLOT(clearRecentActions()));
    _clearRecentAction->setVisible(false);
    connect(recentMenu, SIGNAL(actionClose(QAction*)),
            this, SLOT(handleRecentActionClose(QAction*)));
    QMenu* openMenu = new QMenu(this);
    openMenu->addAction(tr("&Open File ..."), this, SLOT(on_openButton_clicked()));
    openMenu->addMenu(recentMenu);
    ui->openButton->setMenu(openMenu);
}

void MDialog::on_testButton_clicked()
{
    if( _wsheet.size()<= 0) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Please open a list first!"));
        return;
    }

    //
    Dialog dlg;
    int order = ( ui->radio1->isChecked())?
                Word::OrderShuffle: Word::OrderInturn;
    dlg.setSheetAndTable(&_wsheet, ui->tableWidget, order);
    dlg.exec();
}

void MDialog::on_saveButton_clicked()
{
    save();
}

bool MDialog::save()
{
    if( _wsheet.size()<= 0) {
        QMessageBox::warning(this, tr("Error"),
                             tr("No word to save!"));
        return false;
    }

    if( _curFile.isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile( _curFile);
    }
}

bool MDialog::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save List"), ".",
                                                     tr("Word list file (*.list)\n"
                                                        "Text file (*.txt)"));
    if( fileName.isEmpty())
        return false;

    return saveFile( fileName);
}

bool MDialog::saveFile(const QString &fileName)
{
    bool saved = _wsheet.saveFile(fileName);
    if( saved) {
        // let user feel process
        QProgressDialog progress(this);
        progress.setLabelText(tr("Saving file..."));
        progress.setRange(0, _wsheet.size());
        progress.setModal(true);
        for(int step=0; step<_wsheet.size(); step++) {
            progress.setValue( step);
            qApp->processEvents();
        }
        // change filename
        setCurrentFile( fileName);
        return true;
    }
    else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Save file failed!"));
        return false;
    }
}
//bool MDialog::saveFile(const QString &fileName)
//{
//    QFile file( fileName);
//    if( file.open(QIODevice::WriteOnly)) {
//        QProgressDialog progress(this);
//        progress.setLabelText(tr("Saving file..."));
//        progress.setRange(0, wordArray.size());
//        progress.setModal(true);
//        int progstep = 0;
//        //
//        QTextStream out(&file);
//        out.setCodec("UTF-8");
//        WordArray::iterator it;
//        for(it= wordArray.begin(); it!= wordArray.end(); ++it) {
//            out<< it->text();
//            if( it->hasAnswer())
//                out<<"\\\\"<< it->answer();
//            out<< ", ";
//            out<< QString::number(it->status())<< endl;
//            //
//            progress.setValue( ++progstep);
//            qApp->processEvents();
//            if( progress.wasCanceled()) {
//                file.remove();  //this remove operation is acceptable
//                return false;
//            }
//        }
//        setCurrentFile( fileName);
//        return true;
//    }
//    else {
//        QMessageBox::critical(this, tr("Error"),
//                              tr("Save file failed!"));
//        return false;
//    }
//}

void MDialog::closeEvent( QCloseEvent * event )
{
    if( okToContinue()) {
        writeRegSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MDialog::on_addButton_clicked()
{
    ui->tableWidget->addWord();
}

void MDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MDialog::dropEvent(QDropEvent* event)
{
   QList<QUrl> urls = event->mimeData()->urls();
   if( urls.isEmpty())
       return;

   QString fileName = urls.first().toLocalFile();
   if( fileName.isEmpty())
       return;

   emit openSignal( fileName);
}

void MDialog::handleTableModified()
{
    setWindowModified(true);
}

void MDialog::handleKanboxDownloaded(const QString& file)
{
    if( QFileInfo(file).suffix() != "list")
        return;

    if( QMessageBox::question(this, tr("Open"),
                              tr("Do you want to open new downloaded list with WordTester?"),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::Yes) {
        emit openSignal( file);
    }
}

void MDialog::handleRecentActionClose(QAction* a)
{
    QString fname = a->data().toString();
    _recentFiles.removeAll(fname);
    a->setVisible( false);

    if( _recentFiles.size()<=0) {
        _recentSeparator->setVisible(false);
        _clearRecentAction->setVisible(false);
    }
}

void MDialog::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if( action) {
        open( action->data().toString());
    }
}

void MDialog::clearRecentActions()
{
    if( _recentFiles.size()> 0) {
        _recentFiles.clear();
        updateRecentActions();
    }
}


void MDialog::on_resetButton_clicked()
{
    ui->tableWidget->resetStatusAll();
}

void MDialog::on_kanboxExecLabel_linkActivated(const QString&)
{
    // Load .qm and .dll
//    if( !_kanbox) {
//        loadLanguage("KanboxPlugin");
//        QPluginLoader loader("KanboxPlugin.dll");
//        QObject* obj = loader.instance();
//        _kanbox = qobject_cast<KanboxInterface*>(obj);
//        if( obj && _kanbox) {
//            connect(obj, SIGNAL(downloadFinished(QString)),
//                    this, SLOT(handleKanboxDownloaded(QString)));
//        }
//    }

//    // Invoke
//    if( _kanbox) {
//        if( !_curFile.isEmpty())
//            _kanbox->setAboutToUpload(_curFile);
//        _kanbox->execDialog();
//    }
}
