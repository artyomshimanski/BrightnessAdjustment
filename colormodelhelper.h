#ifndef COLORMODELHELPER_H
#define COLORMODELHELPER_H

#include <QRgb>
#include <QColor>
#include <math.h>

typedef struct
{
    double l;
    double a;
    double b;
} LAB;

typedef struct
{
    double x;
    double y;
    double z;
} XYZ;

XYZ RGB2XYZ(const QRgb &rgb)
{
    QColor rgbColor(rgb);

    double r = rgbColor.red() / 255.0;
    double g = rgbColor.green() / 255.0;
    double b = rgbColor.blue() / 255.0;

    if (r > 0.04045)
        r = pow((r + 0.055) / 1.055, 2.4);
    else
        r /= 12.92;

    if (g > 0.04045)
        g = pow((g + 0.055) / 1.055, 2.4);
    else
        g /= 12.92;

    if (b > 0.04045)
        b = pow((b + 0.055) / 1.055, 2.4);
    else
        b /= 12.92f;

    r *= 100;
    g *= 100;
    b *= 100;

    XYZ xyzColor;
    xyzColor.x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    xyzColor.y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    xyzColor.z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    return xyzColor;
}

LAB XYZ2LAB(const XYZ &xyz)
{
    const double REF_X = 95.047;
    const double REF_Y = 100.000;
    const double REF_Z = 108.883;

    double x = xyz.x / REF_X;
    double y = xyz.y / REF_Y;
    double z = xyz.z / REF_Z;

    if (x > 0.008856)
        x = pow(x, 1.0 / 3.0);
    else
        x = (7.787 * x) + (16.0 / 116.0);

    if (y > 0.008856)
        y = pow(y, 1.0 / 3.0);
    else
        y = (7.787 * y) + (16.0 / 116.0);

    if (z > 0.008856)
        z = pow(z, 1.0 / 3.0);
    else
        z = (7.787 * z) + (16.0 / 116.0);

    LAB labColor;
    labColor.l = (116 * y) - 16;
    labColor.a = 500 * (x - y);
    labColor.b = 200 * (y - z);

    return labColor;
}

XYZ LAB2XYZ(const LAB &lab)
{
    const double REF_X = 95.047;
    const double REF_Y = 100.000;
    const double REF_Z = 108.883;

    double y = (lab.l + 16) / 116.0;
    double x = lab.a / 500.0 + y;
    double z = y - lab.b / 200.0;

    if (pow(x, 3) > 0.008856)
        x = pow(x, 3);
    else
        x = (x - 16.0 / 116.0) / 7.787;

    if (pow(y, 3) > 0.008856)
        y = pow(y, 3);
    else
        y = (y - 16.0 / 116.0) / 7.787;

    if (pow(z, 3) > 0.008856)
        z = pow(z, 3);
    else
        z = (z - 16.0 / 116.0) / 7.787;

    XYZ xyzColor;
    xyzColor.x = REF_X * x;
    xyzColor.y = REF_Y * y;
    xyzColor.z = REF_Z * z;

    return xyzColor;
}

QRgb XYZ2RGB(const XYZ &xyz)
{
    double x = xyz.x / 100.0;
    double y = xyz.y / 100.0;
    double z = xyz.z / 100.0;

    double r = x * 3.2406 + y * (-1.5372) + z * (-0.4986);
    double g = x * (-0.9689) + y * 1.8758 + z * 0.0415;
    double b = x * 0.0557 + y * (-0.2040) + z * 1.0570;

    if (r > 0.0031308)
        r = 1.055 * pow(r, (1.0 / 2.4)) - 0.055;
    else
        r *= 12.92;

    if (g > 0.0031308)
        g = 1.055 * pow(g, (1.0 / 2.4)) - 0.055;
    else
        g *= 12.92;

    if (b > 0.0031308)
        b = 1.055 * pow(b, (1.0 / 2.4)) - 0.055;
    else
        b *= 12.92;

    // FIX
    if (r > 1.0)
        r = 1.0;
    else if (r < 0.0)
        r = 0.0;

    if (g > 1.0)
        g = 1.0;
    else if (g < 0.0)
        g = 0.0;

    if (b > 1.0)
        b = 1.0;
    else if (b < 0.0)
        b = 0.0;
    //

    QColor rgbColor;
    rgbColor.setRedF(r);
    rgbColor.setGreenF(g);
    rgbColor.setBlueF(b);

    return rgbColor.rgb();
}

inline LAB RGB2LAB(const QRgb &rgb)
{
    XYZ xyzColor = RGB2XYZ(rgb);
    return XYZ2LAB(xyzColor);
}

inline QRgb LAB2RGB(const LAB &lab)
{
    XYZ xyzColor = LAB2XYZ(lab);
    return XYZ2RGB(xyzColor);
}

#endif // COLORMODELHELPER_H
