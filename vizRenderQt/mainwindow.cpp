#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QScreen>
#include <QtGui>
#include <QtWidgets>
#include <QWizard>
#include "memDrawing.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	scene = nullptr;
	drawer = nullptr;

	ui->comboBox->setCurrentIndex(MemDrawing::DrawSettings::ReadsVsWrites);
	ui->comboBox_2->setCurrentIndex(MemDrawing::DrawSettings::Accesses);
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
	mScale = 1.0f;

	setGeometry(0, 0, imgWidth + 40, 640);

	drawer = new MemDrawing(&mFile, imgWidth * ratio);
	drawer->settings.hueAxis = (MemDrawing::DrawSettings::Axis)ui->comboBox->currentIndex();
	drawer->settings.brightnessAxis = (MemDrawing::DrawSettings::Axis)ui->comboBox->currentIndex();

	drawThread = new QThread;
	drawer->moveToThread(drawThread);

	startDrawing();

	setAttribute(Qt::WA_Hover, true);
	connect(ui->graphicsView, SIGNAL(event(QEvent *)), this, SLOT(hoverEvent(QEvent *)));
}

void MainWindow::startDrawing() {
	if (drawer == nullptr) {
		return;
	}
	//Connect it to a background thread and update the screen when it finishes rendering
	connect(drawThread, SIGNAL(started()), drawer, SLOT(draw()));
	connect(drawer, SIGNAL(done(QImage)), this, SLOT(updateImage(QImage)));
	drawThread->start();
}

void MainWindow::updateImage(QImage image) {
	memImage = image;
	scaleImage(1.0f);
}

void MainWindow::scaleImage(float scale) {
	mScale = scale;
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

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
	if (drawer) {
		drawer->settings.hueAxis = (MemDrawing::DrawSettings::Axis)index;
		startDrawing();
	}
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
	if (drawer) {
		drawer->settings.brightnessAxis = (MemDrawing::DrawSettings::Axis)index;
		startDrawing();
	}
}

void MainWindow::hoverAt(QPointF pt) {
	printf("%f %f\n", pt.x(), pt.y());
}

void MainWindow::hoverEvent(QHoverEvent *e) {
	//Map to relative to the graphics view
	QPoint gp = ui->graphicsView->mapFrom(this, e->pos());
	//And then to relative to the image in the scene... this can be used for memory addresses
	QPointF lp = ui->graphicsView->mapToScene(gp);
	lp /= mScale;

	if (lp.x() >= 0 && lp.y() >= 0 &&
	    lp.x() <= ui->graphicsView->contentsRect().width() &&
		lp.y() <= ui->graphicsView->contentsRect().height()) {
		hoverAt(lp);
	}
}

bool MainWindow::event(QEvent *e) {
	switch (e->type()) {
		case QEvent::HoverMove:
			hoverEvent(static_cast<QHoverEvent *>(e));
			break;
		case QEvent::Scroll:
			//TODO Cast to QScrollEvent and find position relative to the graphics view
			break;
		default: //ignore
			break;
	}
	return QMainWindow::event(e);
}
