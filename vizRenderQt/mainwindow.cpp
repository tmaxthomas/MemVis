#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QScreen>
#include "memDrawing.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	scene = nullptr;
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_loadFileButton_clicked() {
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

	int imgWidth = 1024;

	setGeometry(0, 0, imgWidth + 40, 640);

	MemDrawing *drawer = new MemDrawing(&mFile, imgWidth * ratio);

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
	scaleImage(1.0f);
}

void MainWindow::scaleImage(float scale) {
	qreal ratio = QApplication::screens().at(0)->devicePixelRatio();

	int newW = memImage.size().width();
	int newH = memImage.size().height();

	newW *= (scale / ratio);
	newH *= (scale / ratio);

	QRect rect = memImage.rect();
	rect.setSize(QSize(newW, newH));

	QGraphicsScene *newScene = new QGraphicsScene(this);
	ui->graphicsView->setScene(newScene);

	if (scene) {
		delete scene;
		scene = nullptr;
	}

	scene = newScene;
	scene->setSceneRect(rect);

	QImage scaled = memImage.scaled(newW * ratio, newH * ratio);

	QPixmap map = QPixmap::fromImage(scaled);
	map.setDevicePixelRatio(ratio);
	scene->addPixmap(map);
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    int value = ui->horizontalSlider->value();
    static float range[2] = {1.0f, 10.0f};
    float scale = range[0] + float(range[1] - range[0]) * (float(value - ui->horizontalSlider->minimum()) / float(ui->horizontalSlider->maximum() - ui->horizontalSlider->minimum()));

    scaleImage(scale);
}
