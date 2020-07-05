/******************************************************************************
    VideoRendererTypes: type id and manually id register function
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV (from 2015)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#include "global.h"
#include <QApplication>
#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTextBrowser>

#include "WidgetRenderer.h"

#include "QtAV/private/factory.h"
#include "QtAV/private/mkid.h"

VideoRendererId VideoRendererId_Widget = mkid::id32base36_6<'W', 'i', 'd', 'g', 'e', 't'>::value;

void registerRenderers()
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
    // factory.h does not check whether an id is registered
    if (VideoRenderer::name(VideoRendererId_Widget))
        return;

    VideoRenderer::Register<WidgetRenderer>(VideoRendererId_Widget, "Widget");
}
