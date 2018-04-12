#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vizFile/vizFile.hpp"
#include <QGraphicsScene>

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

private:
    Ui::MainWindow *ui;
    VizFile mFile;
	QImage memImage;
	QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
