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

#include "qitemmodelbardataproxy_p.h"
#include "baritemmodelhandler_p.h"
#include <QTimer>

QT_DATAVIS3D_BEGIN_NAMESPACE

/*!
 * \class QItemModelBarDataProxy
 * \inmodule QtDataVis3D
 * \brief Proxy class for Q3DBars data model mapping.
 * \since 1.0.0
 *
 * QItemModelBarDataProxy allows you to use QAbstractItemModel derived models as a data source
 * for Q3DBars. It maps roles defined in QItemModelBarDataMapping to roles in the model.
 */

/*!
 * Constructs QItemModelBarDataProxy.
 */
QItemModelBarDataProxy::QItemModelBarDataProxy() :
    QBarDataProxy(new QItemModelBarDataProxyPrivate(this))
{
}

/*!
 * Constructs QItemModelBarDataProxy with \a itemModel and \a mapping. Proxy takes ownership of the
 * \a mapping, but doesn't take ownership of the \a itemModel, as typically item models are owned
 * by other controls.
 */
QItemModelBarDataProxy::QItemModelBarDataProxy(const QAbstractItemModel *itemModel,
                                               QItemModelBarDataMapping *mapping) :
    QBarDataProxy(new QItemModelBarDataProxyPrivate(this))
{
    dptr()->m_itemModelHandler->setItemModel(itemModel);
    dptr()->m_itemModelHandler->setActiveMapping(mapping);
}

/*!
 * Destroys QItemModelBarDataProxy.
 */
QItemModelBarDataProxy::~QItemModelBarDataProxy()
{
}

/*!
 * \property QItemModelBarDataProxy::itemModel
 *
 * Defines item model. Does not take ownership of the model, but does connect to it to listen for
 * changes.
 */
void QItemModelBarDataProxy::setItemModel(const QAbstractItemModel *itemModel)
{
    dptr()->m_itemModelHandler->setItemModel(itemModel);
}

const QAbstractItemModel *QItemModelBarDataProxy::itemModel() const
{
    return dptrc()->m_itemModelHandler->itemModel();
}

/*!
 * \property QItemModelBarDataProxy::mapping
 *
 * Defines data mapping. Does not take ownership of the mapping, but does connect to it to listen
 * for changes. Modifying a mapping that is set to the proxy will trigger data set re-resolving.
 */
void QItemModelBarDataProxy::setActiveMapping(QItemModelBarDataMapping *mapping)
{
    dptr()->m_itemModelHandler->setActiveMapping(mapping);
}

QItemModelBarDataMapping *QItemModelBarDataProxy::activeMapping() const
{
    return static_cast<QItemModelBarDataMapping *>(dptrc()->m_itemModelHandler->activeMapping());
}

void QItemModelBarDataProxy::addMapping(QItemModelBarDataMapping *mapping)
{
    dptr()->m_itemModelHandler->addMapping(mapping);
}

void QItemModelBarDataProxy::releaseMapping(QItemModelBarDataMapping *mapping)
{
    dptr()->m_itemModelHandler->releaseMapping(mapping);
}

QList<QItemModelBarDataMapping *> QItemModelBarDataProxy::mappings() const
{
    QList<QItemModelBarDataMapping *> retList;
    QList<QAbstractDataMapping *> abstractList = dptrc()->m_itemModelHandler->mappings();
    foreach (QAbstractDataMapping *mapping, abstractList)
        retList.append(static_cast<QItemModelBarDataMapping *>(mapping));

    return retList;
}

/*!
 * \internal
 */
QItemModelBarDataProxyPrivate *QItemModelBarDataProxy::dptr()
{
    return static_cast<QItemModelBarDataProxyPrivate *>(d_ptr.data());
}

const QItemModelBarDataProxyPrivate *QItemModelBarDataProxy::dptrc() const
{
    return static_cast<const QItemModelBarDataProxyPrivate *>(d_ptr.data());
}

// QItemModelBarDataProxyPrivate

QItemModelBarDataProxyPrivate::QItemModelBarDataProxyPrivate(QItemModelBarDataProxy *q)
    : QBarDataProxyPrivate(q),
      m_itemModelHandler(new BarItemModelHandler(q))
{
}

QItemModelBarDataProxyPrivate::~QItemModelBarDataProxyPrivate()
{
}

QItemModelBarDataProxy *QItemModelBarDataProxyPrivate::qptr()
{
    return static_cast<QItemModelBarDataProxy *>(q_ptr);
}

QT_DATAVIS3D_END_NAMESPACE
