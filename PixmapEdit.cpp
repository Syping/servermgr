/*****************************************************************************
* servermgr Syping Gaming Team Server Manager
* Copyright (C) 2015 Syping Gaming Team
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*****************************************************************************/

#include "PixmapEdit.h"
#include <QPainter>
#include <QPixmap>

PixmapEdit::PixmapEdit(QObject *parent) : QObject(parent)
{
    minSquareSize = 16;
    maxSquareSize = 128;
    pixmapToEdit = QPixmap();
}

void PixmapEdit::setPixmap(QPixmap pixmap)
{
    pixmapToEdit = pixmap;
}

bool PixmapEdit::centerPixmapAtSquare(int squareSize, bool forceSquareSize)
{
    bool squareSizeOk = forceSquareSize;
    if (squareSize >= minSquareSize && squareSize <= maxSquareSize) squareSizeOk = true;
    if (squareSizeOk)
    {
        QPixmap tempEditPixmap = pixmapToEdit;
        if (tempEditPixmap.size().height() < tempEditPixmap.size().width())
        {
            QPixmap scaledWorkPixmap = tempEditPixmap.scaledToWidth(squareSize, Qt::SmoothTransformation);
            if (scaledWorkPixmap.size() != QSize(squareSize ,squareSize))
            {
                int _mx2 = scaledWorkPixmap.size().height() - squareSize;
                int _px2 = abs(_mx2);
                double _pxd = _px2 / 2;
                int _px = floor(_pxd);

                QPixmap *finishWorkPixmap = new QPixmap(squareSize, squareSize);
                finishWorkPixmap->fill(Qt::transparent);
                QPainter drawPainter(finishWorkPixmap);
                drawPainter.drawPixmap(0, _px, squareSize, scaledWorkPixmap.size().height(), scaledWorkPixmap);
                tempEditPixmap = QPixmap(*(finishWorkPixmap));
            }
            else
            {
                tempEditPixmap = scaledWorkPixmap;
            }
            pixmapToEdit = tempEditPixmap;
            return true;
        }
        else
        {
            QPixmap scaledWorkPixmap = tempEditPixmap.scaledToHeight(squareSize, Qt::SmoothTransformation);
            if (scaledWorkPixmap.size() != QSize(squareSize, squareSize))
            {
                int _mx2 = scaledWorkPixmap.size().width() - squareSize;
                int _px2 = abs(_mx2);
                double _pxd = _px2 / 2;
                int _px = floor(_pxd);

                QPixmap *finishWorkPixmap = new QPixmap(squareSize, squareSize);
                finishWorkPixmap->fill(Qt::transparent);
                QPainter drawPainter(finishWorkPixmap);
                drawPainter.drawPixmap(_px, 0, scaledWorkPixmap.size().width(), squareSize, scaledWorkPixmap);
                tempEditPixmap = QPixmap(*(finishWorkPixmap));
            }
            else
            {
                tempEditPixmap = scaledWorkPixmap;
            }
            pixmapToEdit = tempEditPixmap;
            return true;
        }
    }
    return false;
}

QPixmap PixmapEdit::getPixmap()
{
    return pixmapToEdit;
}

int PixmapEdit::getMinSquareSize()
{
    return minSquareSize;
}

int PixmapEdit::getMaxSquareSize()
{
    return maxSquareSize;
}

void PixmapEdit::setMinSquareSize(int _minSquareSize)
{
    minSquareSize = _minSquareSize;
}

void PixmapEdit::setMaxSquareSize(int _maxSquareSize)
{
    maxSquareSize = _maxSquareSize;
}
