/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Oszilloscopewidget.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSZILLOSCOPEWIDGET_H
#define OSZILLOSCOPEWIDGET_H
// C++ Project

#include <Ocean.h>
#include <data/Interface.h>

// Qt
#include <QPolygon>
#include <QGraphicsWidget>


// https://doc.qt.io/qt-6/eventsandfilters.html
// https://doc.qt.io/qt-6/qgraphicsview.html
// https://forum.qt.io/topic/86500/understanding-paintevent



class OszilloscopeWidget : public QGraphicsItem
{

public:
    interface_t*          sds;

    OszilloscopeWidget( interface_t*, QRectF );
    ~OszilloscopeWidget(){};

    void    read_polygon_data();

private:
    QRectF          drawregion;
    QPolygonF       polygon;
    uint 			shiftY 		= 0;
    uint			height		= 0;
    float			data_scale	= 0;

    QRectF  boundingRect() const; // implement virtual method boundingRect, need by qgraphicsitem.h

    void    paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);     // overriding paint() - react on update

};




#endif // OSZILLOSCOPEWIDGET_H
