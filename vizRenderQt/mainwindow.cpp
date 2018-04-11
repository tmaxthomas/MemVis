#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFileButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load viz file"), "", tr("Viz File (*.vzf)"));
    std::ifstream stream(file.toUtf8().constData());
    if (!mFile.read(stream)) {
        QMessageBox::critical(this, "Could not load file", "Selected file is not in the correct format");
        return;
    }

    printf("Loaded file with %d bytes read\n", mFile.bytes.size());
}
