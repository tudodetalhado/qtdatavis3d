/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVis3D module.
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
// This file is not part of the QtDataVis3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QITEMMODELSCATTERDATAPROXY_P_H
#define QITEMMODELSCATTERDATAPROXY_P_H

#include "qitemmodelscatterdataproxy.h"
#include "qscatterdataproxy_p.h"
#include <QPointer>
#include <QTimer>

QT_DATAVIS3D_BEGIN_NAMESPACE

class ScatterItemModelHandler;

class QItemModelScatterDataProxyPrivate : public QScatterDataProxyPrivate
{
    Q_OBJECT
public:
    QItemModelScatterDataProxyPrivate(QItemModelScatterDataProxy *q);
    virtual ~QItemModelScatterDataProxyPrivate();

private:
    QItemModelScatterDataProxy *qptr();

    ScatterItemModelHandler *m_itemModelHandler;

    friend class QItemModelScatterDataProxy;
};

QT_DATAVIS3D_END_NAMESPACE

#endif
