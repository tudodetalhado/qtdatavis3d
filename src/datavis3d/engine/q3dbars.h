/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef Q3DBARS_H
#define Q3DBARS_H

#include "QtDataVis3D/qdatavis3dglobal.h"
#include "q3dwindow.h"

class QOpenGLShaderProgram;

QTCOMMERCIALDATAVIS3D_BEGIN_NAMESPACE

class Q3DBarsPrivate;
class QDataItem;
class QDataRow;
class QDataSet;

class QTCOMMERCIALDATAVIS3D_EXPORT Q3DBars : public Q3DWindow
{
    Q_OBJECT
public:
    enum BarStyle {
        Bars = 0,
        Pyramids,
        Cones,
        Cylinders
    };

    enum CameraPreset {
        PresetFrontLow = 0,
        PresetFront,
        PresetFrontHigh,
        PresetLeftLow,
        PresetLeft,
        PresetLeftHigh,
        PresetRightLow,
        PresetRight,
        PresetRightHigh,
        PresetBehindLow,
        PresetBehind,
        PresetBehindHigh,
        PresetIsometricLeft,
        PresetIsometricLeftHigh,
        PresetIsometricRight,
        PresetIsometricRightHigh,
        PresetDirectlyAbove,
        PresetDirectlyAboveCW45,
        PresetDirectlyAboveCCW45
    };

    enum ColorTheme {
        ThemeSystem = 0,
        ThemeBlueCerulean,
        ThemeBlueIcy,
        ThemeBlueNcs,
        ThemeBrownSand,
        ThemeDark,
        ThemeHighContrast,
        ThemeLight
    };

    enum LabelTransparency {
        TransparencyNone = 0,       // Full solid, using colors from theme
        TransparencyFromTheme,      // Use colors and transparencies from theme
        TransparencyNoBackground    // Draw just text on transparent background
    };

    enum SelectionMode {
        None = 0,
        Bar,
        BarAndRow,
        BarAndColumn,
        BarRowAndColumn,
        ZoomRow,
        ZoomColumn
    };

    // TODO: Should this be moved to Q3DBarsPrivate? Not for use via API directly?
    enum LabelPosition {
        LabelBelow = 0,
        LabelLow,
        LabelMid,
        LabelHigh,
        LabelOver,
        LabelBottom,    // Absolute positions from here onward, used for axes (QDataItem is ignored)
        LabelTop,
        LabelLeft,
        LabelRight
    };

public:
    explicit Q3DBars();
    ~Q3DBars();

    void initialize();
    void render();
    void render(QPainter *painter);

    // Add a row of data. Each new row is added to the front of the sample space, moving previous
    // rows back (if sample space is more than one row deep)
    void addDataRow(const QVector<GLfloat> &dataRow,
                    const QString &labelRow = QString(),
                    const QVector<QString> &labelsColumn = QVector<QString>());
    // ownership of dataItems is transferred
    void addDataRow(const QVector<QDataItem*> &dataRow,
                    const QString &labelRow = QString(),
                    const QVector<QString> &labelsColumn = QVector<QString>());
    // ownership of dataRow is transferred
    void addDataRow(QDataRow *dataRow);

    // Add complete data set at a time, as a vector of data rows
    void addDataSet(const QVector< QVector<GLfloat> > &data,
                    const QVector<QString> &labelsRow = QVector<QString>(),
                    const QVector<QString> &labelsColumn = QVector<QString>());

    // ownership of dataItems is transferred
    void addDataSet(const QVector< QVector<QDataItem*> > &data,
                    const QVector<QString> &labelsRow = QVector<QString>(),
                    const QVector<QString> &labelsColumn = QVector<QString>());
    // ownership of dataSet is transferred
    void addDataSet(QDataSet* dataSet);

    // bar thickness, spacing between bars, and is spacing relative to thickness or absolute
    // y -component sets the thickness/spacing of z -direction
    // With relative 0.0f means side-to-side, 1.0f = one thickness in between
    void setBarSpecs(QPointF thickness = QPointF(1.0f, 1.0f),
                     QPointF spacing = QPointF(1.0f, 1.0f),
                     bool relative = true);

    // bar type; bars (=cubes), pyramids, cones, cylinders, etc.
    void setBarType(BarStyle style, bool smooth = false);

    // override bar type with own mesh
    void setMeshFileName(const QString &objFileName);

    // how many samples per row and column, and names for axes
    void setupSampleSpace(QPoint sampleCount, const QString &labelRow = QString(),
                          const QString &labelColumn = QString(),
                          const QString &labelHeight = QString());

    // Select preset camera placement
    void setCameraPreset(CameraPreset preset);

    // Set camera rotation if you don't want to use the presets (in horizontal (-180...180) and
    // vertical (0...90) angles and distance in percentage (10...500))
    void setCameraPosition(GLfloat horizontal, GLfloat vertical, GLint distance = 100);

    // Set theme (bar colors, shaders, window color, background colors, light intensity and text colors are affected)
    void setTheme(ColorTheme theme);

    // Set color if you don't want to use themes. Set uniform to false if you want the (height) color to change from bottom to top
    void setBarColor(QColor baseColor, QColor heightColor, QColor depthColor, bool uniform = true);

    // TODO: valon siirto / asetus
    // Change selection mode; single bar, bar and row, bar and column, or all
    void setSelectionMode(SelectionMode mode);

    // Set window title
    void setWindowTitle(const QString &title);

    // Font size adjustment (should it be in enum (smallest, smaller, small, normal, large, larger, largest), or just GLfloat?
    void setFontSize(GLfloat fontsize);

    // Set font
    void setFont(const QFont &font);

    // Label transparency adjustment
    void setLabelTransparency(LabelTransparency transparency);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void drawZoomScene();
    void drawScene();
    void drawLabel(const QDataItem &item, GLuint textureId, QSize textureSize,
                   const QMatrix4x4 &viewmatrix, const QMatrix4x4 &projectionmatrix,
                   bool useDepth = false, qreal rotation = 0.0f,
                   Q3DBars::LabelPosition position = Q3DBars::LabelOver);
    QScopedPointer<Q3DBarsPrivate> d_ptr;
    Q_DISABLE_COPY(Q3DBars)
};

QTCOMMERCIALDATAVIS3D_END_NAMESPACE

#endif
