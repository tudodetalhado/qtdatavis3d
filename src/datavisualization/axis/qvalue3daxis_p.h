/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd
** All rights reserved.
** For any questions to The Qt Company, please use contact form at http://qt.io
**
** This file is part of the Qt Data Visualization module.
**
** Licensees holding valid commercial license for Qt may use this file in
** accordance with the Qt License Agreement provided with the Software
** or, alternatively, in accordance with the terms contained in a written
** agreement between you and The Qt Company.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.io
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

#ifndef QVALUE3DAXIS_P_H
#define QVALUE3DAXIS_P_H

#include "qvalue3daxis.h"
#include "qabstract3daxis_p.h"

QT_BEGIN_NAMESPACE_DATAVISUALIZATION

class QValue3DAxisPrivate : public QAbstract3DAxisPrivate
{
    Q_OBJECT

public:
    QValue3DAxisPrivate(QValue3DAxis *q);
    virtual ~QValue3DAxisPrivate();

    virtual void setRange(float min, float max, bool suppressWarnings = false);
    virtual void setMin(float min);
    virtual void setMax (float max);

    void emitLabelsChanged();

Q_SIGNALS:
    void formatterDirty();

protected:
    virtual void updateLabels();

    virtual bool allowZero();
    virtual bool allowNegatives();
    virtual bool allowMinMaxSame();

    int m_segmentCount;
    int m_subSegmentCount;
    QString m_labelFormat;
    bool m_labelsDirty;
    QValue3DAxisFormatter *m_formatter;
    bool m_reversed;

private:
    QValue3DAxis *qptr();

    friend class QValue3DAxis;
    friend class Abstract3DController;
};

QT_END_NAMESPACE_DATAVISUALIZATION

#endif
