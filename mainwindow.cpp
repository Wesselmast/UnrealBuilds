#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QDir dir(qApp->applicationDirPath());
    if(dir.exists()) dir.mkdir("saved");
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::fetch_directory() {
    QFileDialog::Options dialogOptions;
    dialogOptions|= QFileDialog::ShowDirsOnly;
    dialogOptions|= QFileDialog::DontResolveSymlinks;

    return QFileDialog::getExistingDirectory(this, tr("Open Directory"), "C:/", dialogOptions);
}

QString MainWindow::fetch_file() {
    return QFileDialog::getOpenFileName(this, tr("Open UProject"), "C:/", tr("UProject Files (*.uproject)"));
}

void MainWindow::on_pushButton_2_clicked() {
    ui->engineDir->setText(fetch_directory());
}

void MainWindow::on_pushButton_4_clicked() {
    ui->projectLoc->setText(fetch_file());
}

void MainWindow::on_pushButton_5_clicked() {
    ui->outputDir->setText(fetch_directory());
}

void MainWindow::on_pushButton_clicked() {
    QProcess exec;

    QString correctedProj = ui->projectLoc->text().replace('/', '\\');
    QString correctedEng  = ui->engineDir->text().replace('/', '\\');
    QString correctedOut  = ui->outputDir->text().replace('/', '\\');

    exec.setWorkingDirectory(qApp->applicationDirPath());
    QStringList arguments;
    arguments << "/c start wbuild.bat " + correctedEng  + "#"
                                        + correctedProj + "#"
                                        + correctedOut  + "#";

    exec.start("cmd.exe", arguments);
    exec.waitForFinished();
}
