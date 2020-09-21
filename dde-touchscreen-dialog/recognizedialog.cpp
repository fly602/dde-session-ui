/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Zhihsian Que <i@zhihsian.me>
 *
 * Maintainer: Zhihsian Que <i@zhihsian.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "recognizedialog.h"

#include <QApplication>
#include <QPainter>
#include <QTimer>
#include <QPainterPath>

#include <DFontSizeManager>

const int AutoHideDelay = 1000 * 5;
const int FontSize = 20;
const int Radius = 18;
const int MiniWidth = 200;
const int VerticalMargin = 12;
const int HorizentalMargin = 22;
const int yoffset = 220;

RecognizeDialog::RecognizeDialog(Monitor *monitor, QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_monitor(monitor)
    , m_text("")
{
    setWindowFlags(Qt::ToolTip | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
    setAttribute(Qt::WA_TranslucentBackground);

    setRadius(Radius);
    setMinimumWidth(MiniWidth);
    onScreenRectChanged();

    // set auto hide timer
    QTimer *autoHideTimer = new QTimer(this);
    autoHideTimer->setInterval(AutoHideDelay);
    connect(autoHideTimer, &QTimer::timeout, this, &RecognizeDialog::hide);
    autoHideTimer->start();
}

void RecognizeDialog::setText(const QString text)
{
    m_text = text;
    onScreenRectChanged();
}

void RecognizeDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font;
    font.setStyle(QFont::StyleNormal);
    font.setPixelSize(FontSize);
    const QFontMetrics fm(font);

    QPainterPath path;
    path.addText((m_rect.width() - fm.width(m_text)) / 2, m_rect.height() - VerticalMargin - fm.height() / 4, font, m_text);

    QPalette palette;
    QColor brushCorlor;
    brushCorlor = palette.color(QPalette::BrightText);

    painter.setPen(Qt::NoPen);
    painter.setBrush(brushCorlor);
    painter.drawPath(path);
}

void RecognizeDialog::onScreenRectChanged()
{
    const auto ratio = devicePixelRatioF();
    QRect displayRect(m_monitor->x(), m_monitor->y(), m_monitor->width(), m_monitor->height());
    displayRect = QRect(displayRect.topLeft(), displayRect.size() / ratio);

    QFont font;
    font.setStyle(QFont::StyleNormal);
    font.setPixelSize(FontSize);
    const QFontMetrics fm(font);
    int width = fm.width(m_text) + 2 * HorizentalMargin > MiniWidth ? fm.width(m_text) + 2 * HorizentalMargin : MiniWidth;
    int height = fm.height() +2 * VerticalMargin;

    const int x = displayRect.center().x() - width / 2;
    const int y = displayRect.height() - height - yoffset;
    m_rect = QRect(x, y, width, height );

    setGeometry(m_rect);
    update();
}
