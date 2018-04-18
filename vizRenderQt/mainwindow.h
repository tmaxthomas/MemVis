#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vizFile/vizFile.hpp"
#include "memDrawing.hpp"
#include <QGraphicsScene>
#include <QThread>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void scaleImage(float scale);

private slots:
    void on_loadFileButton_clicked();
    void on_horizontalSlider_sliderReleased();

    void updateImage(QImage image);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    VizFile mFile;
	QImage memImage;
	QGraphicsScene *scene;
	MemDrawing *drawer;
	QStandardItemModel *model;

	float mScale;

	void startDrawing();

	void hoverAt(QPointF pt);

	void hoverEvent(QHoverEvent *e);
	bool event(QEvent *e);

	QThread *drawThread;
};

#endif // MAINWINDOW_H
