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

#include "declarativesurface_p.h"
#include "qvalue3daxis.h"
#include "qitemmodelsurfacedataproxy.h"
#include "declarativescene_p.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

DeclarativeSurface::DeclarativeSurface(QQuickItem *parent)
    : AbstractDeclarative(parent),
      m_surfaceController(0)
{
    setAcceptedMouseButtons(Qt::AllButtons);

    // TODO: These seem to have no effect; find a way to activate anti-aliasing
    setAntialiasing(true);
    setSmooth(true);

    // Create the shared component on the main GUI thread.
    m_surfaceController = new Surface3DController(boundingRect().toRect(), new Declarative3DScene);
    setSharedController(m_surfaceController);

    // TODO: Uncomment when doing QTRD-2669
//    connect(m_surfaceController, &Surface3DController::axisXChanged,
//            this, &DeclarativeBars::axisXChanged);
//    connect(m_surfaceController, &Surface3DController::axisYChanged,
//            this, &DeclarativeBars::axisYChanged);
//    connect(m_surfaceController, &Surface3DController::axisZChanged,
//            this, &DeclarativeBars::axisZChanged);
}

DeclarativeSurface::~DeclarativeSurface()
{
    delete m_surfaceController;
}

QValue3DAxis *DeclarativeSurface::axisX() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisX());
}

void DeclarativeSurface::setAxisX(QValue3DAxis *axis)
{
    m_surfaceController->setAxisX(axis);
}

QValue3DAxis *DeclarativeSurface::axisY() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisY());
}

void DeclarativeSurface::setAxisY(QValue3DAxis *axis)
{
    m_surfaceController->setAxisY(axis);
}

QValue3DAxis *DeclarativeSurface::axisZ() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisZ());
}

void DeclarativeSurface::setAxisZ(QValue3DAxis *axis)
{
    m_surfaceController->setAxisZ(axis);
}

QQmlListProperty<QSurface3DSeries> DeclarativeSurface::seriesList()
{
    return QQmlListProperty<QSurface3DSeries>(this, this,
                                          &DeclarativeSurface::appendSeriesFunc,
                                          &DeclarativeSurface::countSeriesFunc,
                                          &DeclarativeSurface::atSeriesFunc,
                                          &DeclarativeSurface::clearSeriesFunc);
}

void DeclarativeSurface::appendSeriesFunc(QQmlListProperty<QSurface3DSeries> *list, QSurface3DSeries *series)
{
    reinterpret_cast<DeclarativeSurface *>(list->data)->addSeries(series);
}

int DeclarativeSurface::countSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    return reinterpret_cast<DeclarativeSurface *>(list->data)->m_surfaceController->surfaceSeriesList().size();
}

QSurface3DSeries *DeclarativeSurface::atSeriesFunc(QQmlListProperty<QSurface3DSeries> *list, int index)
{
    return reinterpret_cast<DeclarativeSurface *>(list->data)->m_surfaceController->surfaceSeriesList().at(index);
}

void DeclarativeSurface::clearSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    DeclarativeSurface *declSurface = reinterpret_cast<DeclarativeSurface *>(list->data);
    QList<QSurface3DSeries *> realList = declSurface->m_surfaceController->surfaceSeriesList();
    int count = realList.size();
    for (int i = 0; i < count; i++)
        declSurface->removeSeries(realList.at(i));
}

void DeclarativeSurface::addSeries(QSurface3DSeries *series)
{
    m_surfaceController->addSeries(series);
}

void DeclarativeSurface::removeSeries(QSurface3DSeries *series)
{
    m_surfaceController->removeSeries(series);
    series->setParent(this); // Reparent as removing will leave series parentless
}

QT_DATAVISUALIZATION_END_NAMESPACE
