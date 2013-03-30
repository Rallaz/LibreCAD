#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

extern QTextStream readLog;

class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    bool save();
    void reload();

private:
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    QAction *openAct;
    QAction *saveAct;
    QAction *reopenAct;
    QToolBar *fileToolBar;
    QTextEdit *textEdit;
    QString curFile;
};

#endif // MAINWINDOW_H
