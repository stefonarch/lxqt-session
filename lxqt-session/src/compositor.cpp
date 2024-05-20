/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org/
 *
 * Copyright: 2010-2011 LXQt team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "compositor.h"

#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <LXQt/Globals>
#include <LXQt/Settings>
#include <QDebug>


bool findCompositor(const QString &compositor)
{
    QFileInfo fi(compositor);
    if (fi.isExecutable())
        return true;

    const QStringList paths = QFile::decodeName(qgetenv("PATH")).split(QL1C(':'));
    for(const QString &dir : paths)
    {
        QFileInfo fi= QFileInfo(dir + QDir::separator() + compositor);
        if (fi.isExecutable() )
            return true;
    }
    return false;
}

CompositorList getCompositorList(bool onlyAvailable)
{
    LXQt::Settings cfg(QSL("compositors"));
    cfg.beginGroup(QSL("KnownCompositors"));
    const QStringList names = cfg.childGroups();

    CompositorList ret;

    for (const QString &name : names)
    {
        bool exists = findCompositor(name);
        if (!onlyAvailable || exists)
        {
            cfg.beginGroup(name);
            Compositor compositor;
            compositor.command = name;
            compositor.name = cfg.localizedValue(QSL("Name"), compositor.command).toString();
            compositor.comment = cfg.localizedValue(QSL("Comment")).toString();
            compositor.exists = exists;
            ret << compositor;
            cfg.endGroup();
        }
    }

    return ret;
}
