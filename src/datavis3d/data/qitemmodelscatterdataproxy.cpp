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

#include "qitemmodelscatterdataproxy_p.h"
#include "scatteritemmodelhandler_p.h"
#include <QTimer>

QT_DATAVIS3D_BEGIN_NAMESPACE

/*!
 * \class QItemModelScatterDataProxy
 * \inmodule QtDataVis3D
 * \brief Proxy class for Q3DScatter data model mapping.
 * \since 1.0.0
 *
 * QItemModelScatterDataProxy allows you to use QAbstractItemModel derived models as a data source
 * for Q3DScatter. It maps roles defined in QItemModelScatterDataMapping to roles in the model.
 */

/*!
 * Constructs QItemModelScatterDataProxy.
 */
QItemModelScatterDataProxy::QItemModelScatterDataProxy() :
    QScatterDataProxy(new QItemModelScatterDataProxyPrivate(this))
{
}

/*!
 * Constructs QItemModelScatterDataProxy with \a itemModel and \a mapping. Does not take ownership
 * of the model or the mapping, but does connect to them to listen for changes.
 */
QItemModelScatterDataProxy::QItemModelScatterDataProxy(const QAbstractItemModel *itemModel,
                                                       QItemModelScatterDataMapping *mapping) :
    QScatterDataProxy(new QItemModelScatterDataProxyPrivate(this))
{
    dptr()->m_itemModelHandler->setItemModel(itemModel);
    dptr()->m_itemModelHandler->setActiveMapping(mapping);
}

/*!
 * Destroys QItemModelScatterDataProxy.
 */
QItemModelScatterDataProxy::~QItemModelScatterDataProxy()
{
}

/*!
 * \property QItemModelScatterDataProxy::itemModel
 *
 * Defines item model. Does not take ownership of the model, but does connect to it to listen for
 * changes.
 */
void QItemModelScatterDataProxy::setItemModel(const QAbstractItemModel *itemModel)
{
    dptr()->m_itemModelHandler->setItemModel(itemModel);
}

const QAbstractItemModel *QItemModelScatterDataProxy::itemModel() const
{
    return dptrc()->m_itemModelHandler->itemModel();
}

/*!
 * \property QItemModelScatterDataProxy::activeMapping
 *
 * Defines data mapping. Does not take ownership of the mapping, but does connect to it to listen
 * for changes. Modifying a mapping that is set to the proxy will trigger data set re-resolving.
 */
void QItemModelScatterDataProxy::setActiveMapping(QItemModelScatterDataMapping *mapping)
{
    dptr()->m_itemModelHandler->setActiveMapping(mapping);
}

QItemModelScatterDataMapping *QItemModelScatterDataProxy::activeMapping() const
{
    return static_cast<QItemModelScatterDataMapping *>(dptrc()->m_itemModelHandler->activeMapping());
}

void QItemModelScatterDataProxy::addMapping(QItemModelScatterDataMapping *mapping)
{
    dptr()->m_itemModelHandler->addMapping(mapping);
}

void QItemModelScatterDataProxy::releaseMapping(QItemModelScatterDataMapping *mapping)
{
    dptr()->m_itemModelHandler->releaseMapping(mapping);
}

QList<QItemModelScatterDataMapping *> QItemModelScatterDataProxy::mappings() const
{
    QList<QItemModelScatterDataMapping *> retList;
    QList<QAbstractDataMapping *> abstractList = dptrc()->m_itemModelHandler->mappings();
    foreach (QAbstractDataMapping *mapping, abstractList)
        retList.append(static_cast<QItemModelScatterDataMapping *>(mapping));

    return retList;
}

/*!
 * \internal
 */
QItemModelScatterDataProxyPrivate *QItemModelScatterDataProxy::dptr()
{
    return static_cast<QItemModelScatterDataProxyPrivate *>(d_ptr.data());
}

const QItemModelScatterDataProxyPrivate *QItemModelScatterDataProxy::dptrc() const
{
    return static_cast<const QItemModelScatterDataProxyPrivate *>(d_ptr.data());
}

// QItemModelScatterDataProxyPrivate

QItemModelScatterDataProxyPrivate::QItemModelScatterDataProxyPrivate(QItemModelScatterDataProxy *q)
    : QScatterDataProxyPrivate(q),
      m_itemModelHandler(new ScatterItemModelHandler(q))
{
}

QItemModelScatterDataProxyPrivate::~QItemModelScatterDataProxyPrivate()
{
}

QItemModelScatterDataProxy *QItemModelScatterDataProxyPrivate::qptr()
{
    return static_cast<QItemModelScatterDataProxy *>(q_ptr);
}

QT_DATAVIS3D_END_NAMESPACE
