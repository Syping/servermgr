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

#ifndef KPTLABEL_H
#define KPTLABEL_H

#include <QLabel>

class KPTLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KPTLabel(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent* ev);

signals:
    void mouseMove(QMouseEvent* ev);
    void mousePress(QMouseEvent* ev);
    void mouseRelease(QMouseEvent* ev);

};

#endif // KPTLABEL_H
