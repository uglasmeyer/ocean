/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * DataGraphicClass.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSZILLOSCOPEWIDGET_H
#define OSZILLOSCOPEWIDGET_H

// Qt
#include <QObject>
#include <QPolygon>
#include <QGraphicsWidget>
#include <QGraphicsView>

// Ocean
#include <data/Interface.h>


// https://doc.qt.io/qt-6/eventsandfilters.html
// https://doc.qt.io/qt-6/qgraphicsview.html
// https://forum.qt.io/topic/86500/understanding-paintevent


/**************************************************
 * DataGraphic_class
 *************************************************/
//QT_BEGIN_NAMESPACE
//namespace Ui
//{
//	class DataGraphic_class;
//}
//QT_END_NAMESPACE


class DataGraphic_class
	: public QGraphicsItem,
//		public QObject,
	  Logfacility_class
{
//	Q_OBJECT

    QRectF          drawregion;
    QGraphicsView*	graphicsview;
    QPolygonF       polygon;
    uint 			shiftY 		= 0;
    uint			height		= 0;
    float			data_scale	= 0;

public:
    interface_t*          sds;

    		DataGraphic_class	( interface_t*, QGraphicsView* );
    virtual	~DataGraphic_class	() = default;

    void    ReadWaveData();
    void    ReadAdsrData();

private:
    Data_t 	readDataEntry();
    void 	readDataExit( Data_t x );
    QRectF  boundingRect() const; // implement virtual method boundingRect, need by qgraphicsitem.h
    void    paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);     // overriding paint() - react on update

};




#endif // OSZILLOSCOPEWIDGET_H
