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

#include "drawer_p.h"
#include "shaderhelper_p.h"
#include "objecthelper_p.h"
#include "qdataitem.h"
#include "qdataitem_p.h"
#include "utils_p.h"
#include "texturehelper_p.h"

QTCOMMERCIALDATAVIS3D_BEGIN_NAMESPACE

Drawer::Drawer(const Theme &theme, const QFont &font, Q3DBars::LabelTransparency transparency)
    : m_theme(theme),
      m_font(font),
      m_transparency(transparency),
      m_textureHelper(new TextureHelper())
{
    initializeOpenGLFunctions();
}

Drawer::~Drawer()
{

}

void Drawer::setTheme(const Theme &theme)
{
    m_theme = theme;
    emit drawerChanged();
}

void Drawer::setFont(const QFont &font)
{
    m_font = font;
    emit drawerChanged();
}

void Drawer::setTransparency(Q3DBars::LabelTransparency transparency)
{
    m_transparency = transparency;
    emit drawerChanged();
}

void Drawer::drawObject(ShaderHelper *shader, ObjectHelper *object, bool textured,
                        GLuint textureId)
{
    if (textured) {
        // Activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader->setUniformValue(shader->texture(), 0);
    }

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(shader->posAtt());
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuf());
    glVertexAttribPointer(shader->posAtt(), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(shader->normalAtt());
    glBindBuffer(GL_ARRAY_BUFFER, object->normalBuf());
    glVertexAttribPointer(shader->normalAtt(), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if (textured) {
        // 3rd attribute buffer : UVs
        glEnableVertexAttribArray(shader->uvAtt());
        glBindBuffer(GL_ARRAY_BUFFER, object->uvBuf());
        glVertexAttribPointer(shader->uvAtt(), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->elementBuf());

    // Draw the triangles
    glDrawElements(GL_TRIANGLES, object->indexCount(), GL_UNSIGNED_SHORT, (void*)0);

    // Free buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (textured) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableVertexAttribArray(shader->uvAtt());
    }
    glDisableVertexAttribArray(shader->normalAtt());
    glDisableVertexAttribArray(shader->posAtt());
}

void Drawer::generateLabelTexture(QDataItem *item)
{
    LabelItem labelItem = item->d_ptr->label();
    generateLabelItem(&labelItem, item->d_ptr->valueStr());
    item->d_ptr->setLabel(labelItem);
}

void Drawer::generateLabelItem(LabelItem *item, const QString &text)
{
    // Delete previous texture, if there is one
    GLuint labelTexture = item->textureId();
    if (labelTexture)
        glDeleteTextures(1, &labelTexture);

    // Create labels
    // Print label into a QImage using QPainter
    QImage label = Utils::printTextToImage(m_font,
                                           text,
                                           m_theme.m_textBackgroundColor,
                                           m_theme.m_textColor,
                                           m_transparency);

    // Set label size
    item->setSize(label.size());
    // Insert text texture into label
    item->setTextureId(m_textureHelper->create2DTexture(label, true, true));
}

QTCOMMERCIALDATAVIS3D_END_NAMESPACE