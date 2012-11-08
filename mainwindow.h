#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QDialog>

class ImageView;
class HistogramWidget;

class QPushButton;

class MainWindow : public QDialog
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openImage();
    void processImage();

private:
    void createSubWidgets();
    void configureSubWidgets();
    void layoutSubWidgets();

    ImageView *lightView;
    ImageView *targetView;
    ImageView *resultView;

    HistogramWidget *lightImgHist;
    HistogramWidget *targetImgHist;
    HistogramWidget *resultImgHist;

    QPushButton *processButton;
};

#endif // MAINWINDOW_H
