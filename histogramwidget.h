#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QComboBox>

class HWBaseWidget : public QWidget
{
public:
    enum Channel
    {
        RED,
        GREEN,
        BLUE,
        RGB
    };

    explicit HWBaseWidget(QWidget *parent = 0);

    void setChannel(const Channel cID);
    Channel channel() const;

private:
    Channel channelID;
};




class HWPlottingArea : public HWBaseWidget
{
public:
    explicit HWPlottingArea(QWidget *parent = 0);

    void plotFromImage(const QImage &image);

protected:
    void paintEvent(QPaintEvent *);

private:
    int maxHistHeight(const Channel cID) const;
    bool binsAreEmpty() const;
    void clearBins();

    int bins[3][256];
};




class HWColorBar : public HWBaseWidget
{
public:
    explicit HWColorBar(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};




class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = 0);

    void drawFromPixmap(const QPixmap &pixmap);

private slots:
    void updatePlottingArea();

private:
    void createSubWidgets();
    void configureSubWidgets();
    void layoutSubWidgets();

    HWColorBar *hwColorBar;
    HWPlottingArea *hwPlottingArea;

    QComboBox *channelComboBox;
};

#endif // HISTOGRAMWIDGET_H
