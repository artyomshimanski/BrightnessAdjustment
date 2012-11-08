#include "mainwindow.h"
#include "histogramwidget.h"
#include "imageprocessor.h"
#include "imageview.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    createSubWidgets();
    configureSubWidgets();
    layoutSubWidgets();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createSubWidgets()
{
    lightView = new ImageView;
    targetView = new ImageView;
    resultView = new ImageView;

    lightImgHist = new HistogramWidget;
    targetImgHist = new HistogramWidget;
    resultImgHist = new HistogramWidget;

    processButton = new QPushButton("Process");
}

void MainWindow::configureSubWidgets()
{
    lightView->setText("Light");
    targetView->setText("Target");
    resultView->setText("Result");

    connect(lightView, SIGNAL(clicked()), this, SLOT(openImage()));
    connect(targetView, SIGNAL(clicked()), this, SLOT(openImage()));
    connect(processButton, SIGNAL(clicked()), this, SLOT(processImage()));
}

void MainWindow::layoutSubWidgets()
{
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(lightView);
    leftLayout->addWidget(lightImgHist);

    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addWidget(targetView);
    centerLayout->addWidget(targetImgHist);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(resultView);
    rightLayout->addWidget(resultImgHist);

    QHBoxLayout *basicLayout = new QHBoxLayout;
    basicLayout->addLayout(leftLayout);
    basicLayout->addLayout(centerLayout);
    basicLayout->addLayout(rightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(basicLayout);
    mainLayout->addWidget(processButton);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

    if (!fileName.isEmpty())
    {
        QPixmap pixmap(fileName);

        if (!pixmap.isNull())
        {
            if (sender() == lightView)
            {
                lightView->setPixmap(pixmap);
                lightImgHist->drawFromPixmap(pixmap);
            }
            else if (sender() == targetView)
            {
                targetView->setPixmap(pixmap);
                targetImgHist->drawFromPixmap(pixmap);
            }
        }
        else
        {
            QMessageBox msgBox(QMessageBox::Critical, "Error", "Invalid file format");
            msgBox.exec();
        }
    }
}

void MainWindow::processImage()
{
    if (lightView->pixmap() != 0 && targetView->pixmap() != 0)
    {
        ImageProcessor *imageProcessor = new ImageProcessor;
        QImage resultImg = imageProcessor->colorTransfer(lightView->pixmap()->toImage(), targetView->pixmap()->toImage());
        resultView->setPixmap(QPixmap::fromImage(resultImg));
        resultImgHist->drawFromPixmap(QPixmap::fromImage(resultImg));
        delete imageProcessor;
    }
}
