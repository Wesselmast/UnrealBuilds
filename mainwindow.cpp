#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardPaths>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QDir dir(qApp->applicationDirPath());
    if(dir.exists()) dir.mkdir("saved");
    saveFile = new QFile(qApp->applicationDirPath() + "\\saved\\saved.txt");
    load_data();


    QFile dFile(QStandardPaths::locate(QStandardPaths::AppDataLocation, "..\\Unreal Engine\\UnrealBuildTool\\BuildConfiguration.xml"));
    QFile sFile(qApp->applicationDirPath() + "\\saved\\BuildConfiguration.xml");
    if(dFile.exists() && sFile.exists()) {
        dFile.open(QIODevice::WriteOnly);
        sFile.open(QIODevice::ReadOnly);

        QTextStream dStream(&dFile);
        QTextStream sStream(&sFile);

        while(!sStream.atEnd()) {
            dStream << sStream.readLine() << endl;
        }
    }
}

MainWindow::~MainWindow() {
    delete saveFile;
    delete ui;
}

void MainWindow::load_data() {
    if(saveFile->open(QIODevice::ReadOnly)) {
        QTextStream stream(saveFile);
        ui->engineDir->setText(stream.readLine());
        ui->projectLoc->setText(stream.readLine());
        ui->outputDir->setText(stream.readLine());
    }
    saveFile->close();
}

void MainWindow::save_data() {
    if(saveFile->open(QIODevice::WriteOnly)) {
        QTextStream stream(saveFile);
        stream << ui->engineDir->text()  << endl <<
                  ui->projectLoc->text() << endl <<
                  ui->outputDir->text()  << endl;
    }
    saveFile->close();
}

QString MainWindow::fetch_directory(QString from) {
    QFileDialog::Options dialogOptions;
    dialogOptions|= QFileDialog::ShowDirsOnly;
    dialogOptions|= QFileDialog::DontResolveSymlinks;

    return QFileDialog::getExistingDirectory(this, tr("Open Directory"), from, dialogOptions);
}

QString MainWindow::fetch_file(QString from) {
    return QFileDialog::getOpenFileName(this, tr("Open UProject"), from, tr("UProject Files (*.uproject)"));
}

void MainWindow::on_pushButton_2_clicked() {
    QString fetchResult = fetch_directory(ui->engineDir->text());
    if(!fetchResult.isEmpty()) {
        ui->engineDir->setText(fetchResult);
    }
    save_data();
}

void MainWindow::on_pushButton_4_clicked() {
    QString fetchResult = fetch_file(ui->projectLoc->text());
    if(!fetchResult.isEmpty()) {
        ui->projectLoc->setText(fetchResult);
    }
    save_data();
}

void MainWindow::on_pushButton_5_clicked() {
    QString fetchResult = fetch_directory(ui->outputDir->text());
    if(!fetchResult.isEmpty()) {
        ui->outputDir->setText(fetchResult);
    }
    save_data();
}

void MainWindow::execute(QString wbuild) {
    QProcess exec;

    QString correctedEng  = ui->engineDir->text().replace('/', '\\');
    QString correctedProj = ui->projectLoc->text().replace('/', '\\');
    QString correctedOut  = ui->outputDir->text().replace('/', '\\');

    exec.setWorkingDirectory(qApp->applicationDirPath());
    QStringList arguments;
    arguments << "/c start " + wbuild + ".bat " + correctedEng  + "#"
                                        + correctedProj + "#"
                                        + correctedOut  + "#";

    exec.start("cmd.exe", arguments);
    exec.waitForFinished();
    save_data();
}

void MainWindow::on_pushButton_clicked() {
    execute("wbuild_dev");
}

void MainWindow::on_pushButton_3_clicked() {
    execute("wbuild_shipping");
}
