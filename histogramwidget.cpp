#include "histogramwidget.h"

#include <QPainter>
#include <QVBoxLayout>

HWBaseWidget::HWBaseWidget(QWidget *parent) :
    QWidget(parent)
{
    channelID = RED;
}

void HWBaseWidget::setChannel(const Channel cID)
{
    if (channelID != cID)
    {
        channelID = cID;
        update();
    }
}

HWBaseWidget::Channel HWBaseWidget::channel() const
{
    return channelID;
}





HWPlottingArea::HWPlottingArea(QWidget *parent) :
    HWBaseWidget(parent)
{
    clearBins();

    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    setMinimumSize(256, 120);
}

void HWPlottingArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::darkGray);

    if (!binsAreEmpty())
    {
        double colHeight = 0.0;
        double colWidth = width() / 256.0;

        double maxHeight = maxHistHeight(channel());

        for (int x = 0; x < 256; x++)
        {
            if (channel() != RGB)
                colHeight = (bins[channel()][x] / maxHeight) * height();
            else
            {
                int meanHeight = (bins[0][x] + bins[1][x] + bins[2][x]) / 3;
                colHeight = (meanHeight / maxHeight) * height();
            }

            if (colWidth == 1.0)
                painter.drawLine(x, height() - colHeight, x, height());
            else
                painter.drawRect(x * colWidth, height() - colHeight, colWidth, height());
        }

        painter.setPen(Qt::black);
        painter.drawRect(0, 0, width() - 1, height() - 1);
    }
}

void HWPlottingArea::plotFromImage(const QImage &image)
{
    clearBins();

    for (int y = 0; y < image.height(); y++)
    {
        for (int x = 0; x < image.width(); x++)
        {
            QColor pixelColor(image.pixel(x, y));
            bins[0][pixelColor.red()]++;
            bins[1][pixelColor.green()]++;
            bins[2][pixelColor.blue()]++;
        }
    }

    update();
}

int HWPlottingArea::maxHistHeight(const Channel cID) const
{
    int max = 0;

    for (int c = 0; c < 3; c++)
    {
        if (cID != RGB)
            max = 0;

        for (int x = 0; x < 256; x++)
        {
            if (bins[c][x] > max)
                max = bins[c][x];
        }

        if (cID == c)
            break;
    }

    return max;
}

bool HWPlottingArea::binsAreEmpty() const
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 256; j++)
            if (bins[i][j] != 0)
                return false;

    return true;
}

void HWPlottingArea::clearBins()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 256; j++)
            bins[i][j] = 0;
}




HWColorBar::HWColorBar(QWidget *parent) :
    HWBaseWidget(parent)
{
    setMinimumWidth(256);
    setFixedHeight(20);
}

void HWColorBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    double colWidth = width() / 256.0;

    for (int i = 0; i < 256; i++)
    {
        int r = 0, g = 0, b = 0;

        switch(channel())
        {
            case RED:
                r = i;
                break;
            case GREEN:
                g = i;
                break;
            case BLUE:
                b = i;
                break;
            case RGB:
                r = i;
                g = i;
                b = i;
                break;
        }

        painter.setPen(QColor(r, g, b));
        painter.drawRect(i * colWidth, 0, colWidth, height());
    }

    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width() - 1, height() - 1);
}




HistogramWidget::HistogramWidget(QWidget *parent) :
    QWidget(parent)
{
    createSubWidgets();
    configureSubWidgets();
    layoutSubWidgets();
}

void HistogramWidget::createSubWidgets()
{
    hwColorBar = new HWColorBar(this);
    hwPlottingArea = new HWPlottingArea(this);
    channelComboBox = new QComboBox;
}

void HistogramWidget::configureSubWidgets()
{
    channelComboBox->addItem("Red");
    channelComboBox->addItem("Green");
    channelComboBox->addItem("Blue");
    channelComboBox->addItem("RGB");

    connect(channelComboBox, SIGNAL(activated(int)), this, SLOT(updatePlottingArea()));
}

void HistogramWidget::layoutSubWidgets()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(channelComboBox);
    mainLayout->addWidget(hwPlottingArea);
    mainLayout->addWidget(hwColorBar);
}

void HistogramWidget::updatePlottingArea()
{
    HWBaseWidget::Channel cID = (HWBaseWidget::Channel)channelComboBox->currentIndex();
    hwPlottingArea->setChannel(cID);
    hwColorBar->setChannel(cID);
}

void HistogramWidget::drawFromPixmap(const QPixmap &pixmap)
{
    hwPlottingArea->plotFromImage(pixmap.toImage());
}
