/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVisualization module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVisualization API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Q3DBARSCONTROLLER_p_H
#define Q3DBARSCONTROLLER_p_H

#include "datavisualizationglobal_p.h"
#include "abstract3dcontroller_p.h"

//#define DISPLAY_RENDER_SPEED

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class Bars3DRenderer;
class QBarDataProxy;

struct Bars3DChangeBitField {
    bool slicingActiveChanged     : 1;
    bool sampleSpaceChanged       : 1;
    bool barSpecsChanged          : 1;
    bool selectedBarPosChanged    : 1;

    Bars3DChangeBitField() :
        slicingActiveChanged(true),
        sampleSpaceChanged(true),
        barSpecsChanged(true),
        selectedBarPosChanged(true)
    {
    }
};

class QT_DATAVISUALIZATION_EXPORT Bars3DController : public Abstract3DController
{
    Q_OBJECT

private:
    Bars3DChangeBitField m_changeTracker;

    // Data
    int m_rowCount;
    int m_columnCount;

    // Interaction
    QPoint m_selectedBarPos;     // Points to row & column in data window.

    // Look'n'feel
    bool m_isBarSpecRelative;
    GLfloat m_barThicknessRatio;
    QSizeF m_barSpacing;

    // Rendering
    Bars3DRenderer *m_renderer;

public:
    explicit Bars3DController(QRect rect);
    ~Bars3DController();

    void initializeOpenGL();
    virtual void synchDataToRenderer();

    int columnCount();
    int rowCount();

    // bar thickness, spacing between bars, and is spacing relative to thickness or absolute
    // y -component sets the thickness/spacing of z -direction
    // With relative 0.0f means side-to-side, 1.0f = one thickness in between
    void setBarSpecs(GLfloat thicknessRatio = 1.0f,
                     const QSizeF &spacing = QSizeF(1.0, 1.0),
                     bool relative = true);
    GLfloat barThickness();
    QSizeF barSpacing();
    bool isBarSpecRelative();

    // bar type; bars (=cubes), pyramids, cones, cylinders, etc.
    void setBarType(QDataVis::MeshStyle style, bool smooth = false);

    // how many samples per row and column, and names for axes
    void setDataWindow(int samplesRow, int samplesColumn);

    // Change selection mode; single bar, bar and row, bar and column, or all
    void setSelectionMode(QDataVis::SelectionMode mode);

    void setSelectedBarPos(const QPoint &position);
    QPoint selectedBarPos() const;

    virtual void setActiveDataProxy(QAbstractDataProxy *proxy);

    virtual void handleAxisAutoAdjustRangeChangedInOrientation(Q3DAbstractAxis::AxisOrientation orientation, bool autoAdjust);

    static QPoint noSelectionPoint();

public slots:
    void handleArrayReset();
    void handleRowsAdded(int startIndex, int count);
    void handleRowsChanged(int startIndex, int count);
    void handleRowsRemoved(int startIndex, int count);
    void handleRowsInserted(int startIndex, int count);
    void handleItemChanged(int rowIndex, int columnIndex);

    void handleSelectedBarPosChanged(const QPoint &position);

signals:
    void selectedBarPosChanged(QPoint position);

protected:
    virtual Q3DAbstractAxis *createDefaultAxis(Q3DAbstractAxis::AxisOrientation orientation);

private:
    void adjustValueAxisRange();

    Q_DISABLE_COPY(Bars3DController)

};


QT_DATAVISUALIZATION_END_NAMESPACE

#endif