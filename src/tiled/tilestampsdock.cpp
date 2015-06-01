/*
 * tilestampdock.cpp
 * Copyright 2015, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tilestampsdock.h"

#include "documentmanager.h"
#include "quickstampmanager.h"
#include "tilestamp.h"
#include "tilestampmodel.h"

#include <QListView>
#include <QVBoxLayout>

namespace Tiled {
namespace Internal {

TileStampsDock::TileStampsDock(QuickStampManager *stampManager, QWidget *parent)
    : QDockWidget(parent)
    , mTileStampModel(stampManager->tileStampModel())
{
    setObjectName(QLatin1String("TileStampsDock"));
    retranslateUi();

    QListView *stampsView = new QListView(this);
    stampsView->setModel(mTileStampModel);

    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setMargin(5);
    layout->addWidget(stampsView);

    QItemSelectionModel *selectionModel = stampsView->selectionModel();
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentRowChanged(QModelIndex)));

    setWidget(widget);
}

void TileStampsDock::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

void TileStampsDock::currentRowChanged(const QModelIndex &index)
{
    MapDocument *mapDocument = DocumentManager::instance()->currentDocument();
    if (!mapDocument)
        return;

    if (TileStamp *stamp = mTileStampModel->stampAt(index)) {
        // todo: set TileStamp as the brush, so that variations can be chosen while painting
        if (Map *map = stamp->randomVariation()) {
            mapDocument->unifyTilesets(map);
            emit setStampBrush(static_cast<TileLayer*>(map->layerAt(0)));
        }
    }
}

void TileStampsDock::retranslateUi()
{
    setWindowTitle(tr("Tile Stamps"));
}

} // namespace Internal
} // namespace Tiled

