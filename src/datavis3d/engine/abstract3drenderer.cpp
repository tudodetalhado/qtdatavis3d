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

#include "abstract3drenderer_p.h"
#include "qvalueaxis.h"

QT_DATAVIS3D_BEGIN_NAMESPACE

Abstract3DRenderer::Abstract3DRenderer(Abstract3DController *controller)
    : QObject(controller),
      m_controller(controller),
      m_hasNegativeValues(false),
      m_drawer(new Drawer(m_cachedTheme, m_cachedFont, m_cachedLabelTransparency)),
      m_autoScaleAdjustment(1.0f)
{
}

void Abstract3DRenderer::initializePreOpenGL()
{
    QObject::connect(m_drawer, &Drawer::drawerChanged, this, &Abstract3DRenderer::updateTextures);

    QObject::connect(m_controller, &Abstract3DController::themeChanged, this,
                     &Abstract3DRenderer::updateTheme);
    QObject::connect(m_controller, &Abstract3DController::fontChanged, this,
                     &Abstract3DRenderer::updateFont);
    QObject::connect(m_controller, &Abstract3DController::labelTransparencyUpdated, this,
                     &Abstract3DRenderer::updateLabelTransparency);
    QObject::connect(m_controller, &Abstract3DController::boundingRectChanged, this,
                     &Abstract3DRenderer::updateBoundingRect);
    QObject::connect(m_controller, &Abstract3DController::sizeChanged, this,
                     &Abstract3DRenderer::updateBoundingRect);
    QObject::connect(m_controller, &Abstract3DController::shadowQualityChanged, this,
                     &Abstract3DRenderer::updateShadowQuality);
    QObject::connect(m_controller, &Abstract3DController::axisTypeChanged, this,
                     &Abstract3DRenderer::updateAxisType);
    QObject::connect(m_controller, &Abstract3DController::axisTitleChanged, this,
                     &Abstract3DRenderer::updateAxisTitle);
    QObject::connect(m_controller, &Abstract3DController::axisLabelsChanged, this,
                     &Abstract3DRenderer::updateAxisLabels);
    QObject::connect(m_controller, &Abstract3DController::axisRangeChanged, this,
                     &Abstract3DRenderer::updateAxisRange);
    QObject::connect(m_controller, &Abstract3DController::axisSegmentCountChanged, this,
                     &Abstract3DRenderer::updateAxisSegmentCount);
    QObject::connect(m_controller, &Abstract3DController::axisSubSegmentCountChanged, this,
                     &Abstract3DRenderer::updateAxisSubSegmentCount);

    updateTheme(m_controller->theme());
    updateFont(m_controller->font());
    updateLabelTransparency(m_controller->labelTransparency());
}

void Abstract3DRenderer::initializeOpenGL()
{
    // OpenGL is initialized, safe to call these.
    updateBoundingRect(m_controller->boundingRect());
    updateShadowQuality(m_controller->shadowQuality());

    initializeAxisCache(QAbstractAxis::AxisOrientationX, m_controller->axisX());
    initializeAxisCache(QAbstractAxis::AxisOrientationY, m_controller->axisY());
    initializeAxisCache(QAbstractAxis::AxisOrientationZ, m_controller->axisZ());
}

void Abstract3DRenderer::updateBoundingRect(const QRect boundingRect)
{
    m_cachedBoundingRect = boundingRect;
    handleResize();
}

void Abstract3DRenderer::updatePosition(const QRect boundingRect)
{
    m_cachedBoundingRect = boundingRect;
}

void Abstract3DRenderer::updateTheme(Theme theme)
{
    m_cachedTheme.setFromTheme(theme);

    m_drawer->setTheme(m_cachedTheme);
    // Re-initialize shaders
    handleShadowQualityChange();
}

void Abstract3DRenderer::handleShadowQualityChange()
{
#if !defined(QT_OPENGL_ES_2)
    if (m_cachedShadowQuality > ShadowNone) {
        if (!m_cachedTheme.m_uniformColor) {
            initShaders(QStringLiteral(":/shaders/vertexShadow"),
                        QStringLiteral(":/shaders/fragmentShadowNoTexColorOnY"));
        } else {
            initShaders(QStringLiteral(":/shaders/vertexShadow"),
                        QStringLiteral(":/shaders/fragmentShadowNoTex"));
        }
        initBackgroundShaders(QStringLiteral(":/shaders/vertexShadow"),
                              QStringLiteral(":/shaders/fragmentShadowNoTex"));
    } else {
        if (!m_cachedTheme.m_uniformColor) {
            initShaders(QStringLiteral(":/shaders/vertex"),
                        QStringLiteral(":/shaders/fragmentColorOnY"));
        } else {
            initShaders(QStringLiteral(":/shaders/vertex"),
                        QStringLiteral(":/shaders/fragment"));
        }
        initBackgroundShaders(QStringLiteral(":/shaders/vertex"),
                              QStringLiteral(":/shaders/fragment"));
    }
#else
    if (!m_cachedTheme.m_uniformColor) {
        initShaders(QStringLiteral(":/shaders/vertexES2"),
                    QStringLiteral(":/shaders/fragmentColorOnYES2"));
    } else {
        initShaders(QStringLiteral(":/shaders/vertexES2"),
                    QStringLiteral(":/shaders/fragmentES2"));
    }
    initBackgroundShaders(QStringLiteral(":/shaders/vertexES2"),
                          QStringLiteral(":/shaders/fragmentES2"));
#endif
}

void Abstract3DRenderer::updateFont(const QFont &font)
{
    m_cachedFont = font;
    m_drawer->setFont(font);
}

void Abstract3DRenderer::updateLabelTransparency(LabelTransparency transparency)
{
    m_cachedLabelTransparency = transparency;
    m_drawer->setTransparency(transparency);
}

void Abstract3DRenderer::handleResize()
{
    if (m_cachedBoundingRect.width() == 0 || m_cachedBoundingRect.height() == 0)
        return;
    qDebug() << __FUNCTION__ << m_cachedBoundingRect.width() << "x" << m_cachedBoundingRect.height();
    // Calculate zoom level based on aspect ratio
    GLfloat div;
    GLfloat zoomAdjustment;
    div = qMin(m_cachedBoundingRect.width(), m_cachedBoundingRect.height());
    zoomAdjustment = defaultRatio * ((m_cachedBoundingRect.width() / div)
                                     / (m_cachedBoundingRect.height() / div));
    //qDebug() << "zoom adjustment" << zoomAdjustment;
    m_autoScaleAdjustment = qMin(zoomAdjustment, 1.0f); // clamp to 1.0f

    // Re-init selection buffer
    initSelectionBuffer();

#if !defined(QT_OPENGL_ES_2)
    // Re-init depth buffer
    updateDepthBuffer();
#endif
}

void Abstract3DRenderer::updateAxisType(QAbstractAxis::AxisOrientation orientation, QAbstractAxis::AxisType type)
{
    axisCacheForOrientation(orientation).setType(type);
}

void Abstract3DRenderer::updateAxisTitle(QAbstractAxis::AxisOrientation orientation, const QString &title)
{
    axisCacheForOrientation(orientation).setTitle(title);
}

void Abstract3DRenderer::updateAxisLabels(QAbstractAxis::AxisOrientation orientation, const QStringList &labels)
{
    axisCacheForOrientation(orientation).setLabels(labels);
}

void Abstract3DRenderer::updateAxisRange(QAbstractAxis::AxisOrientation orientation, qreal min, qreal max)
{
    AxisRenderCache &cache = axisCacheForOrientation(orientation);
    cache.setMin(min);
    cache.setMax(max);
}

void Abstract3DRenderer::updateAxisSegmentCount(QAbstractAxis::AxisOrientation orientation, int count)
{
    axisCacheForOrientation(orientation).setSegmentCount(count);
}

void Abstract3DRenderer::updateAxisSubSegmentCount(QAbstractAxis::AxisOrientation orientation, int count)
{
    axisCacheForOrientation(orientation).setSubSegmentCount(count);
}

void Abstract3DRenderer::initializeAxisCache(QAbstractAxis::AxisOrientation orientation, const QAbstractAxis *axis)
{
    axisCacheForOrientation(orientation).setDrawer(m_drawer);

    if (axis) {
        updateAxisType(orientation, axis->type());
        updateAxisTitle(orientation, axis->title());
        updateAxisLabels(orientation, axis->labels());
        if (axis->type() & QAbstractAxis::AxisTypeValue) {
            const QValueAxis *valueAxis = static_cast<const QValueAxis *>(axis);
            updateAxisRange(orientation, valueAxis->min(), valueAxis->max());
            updateAxisSegmentCount(orientation, valueAxis->segmentCount());
            updateAxisSubSegmentCount(orientation, valueAxis->subSegmentCount());
        }
    }
}

AxisRenderCache &Abstract3DRenderer::axisCacheForOrientation(QAbstractAxis::AxisOrientation orientation)
{
    switch (orientation) {
    case QAbstractAxis::AxisOrientationX:
        return m_axisCacheX;
    case QAbstractAxis::AxisOrientationY:
        return m_axisCacheY;
    case QAbstractAxis::AxisOrientationZ:
        return m_axisCacheZ;
    default:
        qFatal(__FUNCTION__);
        return m_axisCacheX;
    }
}


QT_DATAVIS3D_END_NAMESPACE
