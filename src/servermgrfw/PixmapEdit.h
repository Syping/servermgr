/*****************************************************************************
* servermgrfw Syping Gaming Team Server Manager Framework
* Copyright (C) 2015-2016 Syping Gaming Team
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

#ifndef PIXMAPEDIT_H
#define PIXMAPEDIT_H

#include <QObject>
#include <QPixmap>

class PixmapEdit : public QObject
{
    Q_OBJECT

public:
    explicit PixmapEdit(QObject *parent = 0);
    void setPixmap(QPixmap pixmap);
    bool centerPixmapAtSquare(int squareSize, bool forceSquareSize = false);
    QPixmap getPixmap();
    int getMinSquareSize();
    int getMaxSquareSize();
    void setMinSquareSize(int minSquareSize);
    void setMaxSquareSize(int maxSquareSize);

private:
    QPixmap pixmapToEdit;
    int minSquareSize;
    int maxSquareSize;

};

#endif // PIXMAPEDIT_H
