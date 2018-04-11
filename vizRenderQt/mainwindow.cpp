#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QScreen>
#include "memDrawing.hpp"

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
	//Get their file selection and load it
    QString file = QFileDialog::getOpenFileName(this, tr("Load viz file"), "", tr("Viz File (*.vzf)"));
    std::ifstream stream(file.toUtf8().constData());
    if (!mFile.read(stream)) {
        QMessageBox::critical(this, "Could not load file", "Selected file is not in the correct format");
        return;
    }

	printf("Loaded file with %lu bytes\n", mFile.bytes.size());

	//Don't ask rpi about this
	qreal ratio = QApplication::screens().at(0)->devicePixelRatio();

	MemDrawing *drawer = new MemDrawing(&mFile, 1024 * ratio);

	//Connect it to a background thread and update the screen when it finishes rendering
	QThread *thread = new QThread;
	drawer->moveToThread(thread);
	connect(thread, SIGNAL(started()), drawer, SLOT(draw()));
	connect(drawer, SIGNAL(done(QImage)), this, SLOT(updateImage(QImage)));
	connect(drawer, SIGNAL(cleanup()), thread, SLOT(quit()));
	connect(thread, SIGNAL(finished()), drawer, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();
}

void MainWindow::updateImage(QImage image) {
	memImage = image;

	qreal ratio = QApplication::screens().at(0)->devicePixelRatio();

	scene = new QGraphicsScene(this);
	QPixmap map = QPixmap::fromImage(image);
	map.setDevicePixelRatio(ratio);
	scene->addPixmap(map);
	QRect rect = map.rect();
	rect.setSize(rect.size() / ratio);
	scene->setSceneRect(rect);

	ui->graphicsView->setScene(scene);
}
