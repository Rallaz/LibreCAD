#include "mainwindow.h"
#include "filterdxfrw.h"

#include <QTextEdit>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QStatusBar>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    textEdit = new QTextEdit;
    setCentralWidget(textEdit);


    openAct = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Parse an existing DWG file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the log to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    reopenAct = new QAction(tr("&Reload"), this);
    reopenAct->setShortcuts(QKeySequence::Refresh);
    reopenAct->setStatusTip(tr("Reload last opened file"));
    connect(reopenAct, SIGNAL(triggered()), this, SLOT(reload()));

    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(reopenAct);

    statusBar()->showMessage(tr("Ready"), 2000);
    this->resize(800,300);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::open()
{
    QSettings    stg;
    stg.beginGroup("dwgdevel");
    
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open File"),
                stg.value("crt_file",QDir::currentPath()).toString(),
                tr("dwg file (*.dwg)"));
    if (!fileName.isEmpty()) {
        if (!textEdit->document()->isEmpty()) {
            textEdit->document()->clear();
        }
        loadFile(fileName);
        stg.setValue("crt_file",fileName);
    }
    stg.endGroup();
}

bool MainWindow::save()
{
    return saveFile(curFile);
}

void MainWindow::reload()
{
    QSettings    stg;
    stg.beginGroup("dwgdevel");
    QString fileName = stg.value("crt_file").toString();
    if ( QFile::exists(fileName) )
    {
        loadFile(fileName);
    }
    else
    {
        statusBar()->showMessage(tr("File %1 was not found").arg(fileName), 5000);
    }
    stg.endGroup();
}

void MainWindow::loadFile(const QString &fileName)
{

    QFile file(fileName);
/*    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }*/
    this->setWindowTitle(file.fileName() + " DWGdevel");
//    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //leo el dibujo y in debe ser global
    FilterDXFRW *dwg;
    dwg = new FilterDXFRW();
    /*bool success =*/ dwg->fileImport(fileName);
    delete dwg;
//    textEdit->setPlainText(in.readAll());
    textEdit->setPlainText(readLog.readAll());
    QApplication::restoreOverrideCursor();
    curFile = fileName;
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFileInfo fileLog(fileName);
    QString saveName(fileLog.canonicalFilePath()+ ".log");
    QFile file(saveName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    file.close();
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

