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
 * keyboard_dialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



// C++ Project
// https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene_QGraphicsItems.php

#include <include/Oszilloscopewidget.h>
#include <qpainter.h>


QRectF OszilloscopeWidget::boundingRect() const
{
    return QRectF( drawregion );
}

OszilloscopeWidget::OszilloscopeWidget( interface_t* ifd, QRectF rectangle)
{
    drawregion  = { 0,0,rectangle.width()-10, rectangle.height()-10 };

    height 		= drawregion.height();
    shiftY 		= height / 2;
	data_scale	= height / max_data_amp / 2.0;

    this->sds   = ifd;
    this->update();
};

void OszilloscopeWidget::paint(QPainter* painter,
                               const QStyleOptionGraphicsItem* option,
                               QWidget* widget)
{

	auto CursorLine = [ this, painter ]( Qt::GlobalColor color, uint16_t cursor )
	{
    	painter->setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap));
    	painter->drawLine( cursor, 0, cursor, height);
	};

    QGraphicsPolygonItem 	polygon_item;
    painter->drawRect		( drawregion );
    polygon_item.setPolygon	( polygon);
    polygon_item.paint		( painter, option, widget);

    uint16_t cursor = (int)sds->WD_state.cursor.cur;
    if ( cursor > 0  )
    {
    	CursorLine( Qt::black, cursor );
    }
    if ( sds->WD_state.wd_mode == wavedisplay_t::CURSORID )
    {
    	CursorLine( Qt::red, sds->WD_state.cursor.min );
    	CursorLine( Qt::red, sds->WD_state.cursor.max );
    }

}

void OszilloscopeWidget::read_polygon_data(  )
{
    // init polygon
	polygon.clear();
    polygon << QPoint(0,0);

    Data_t x = 0;
    for ( auto y : sds->wavedata )
    {
        polygon << QPoint(x, - y * data_scale );
        x++;
    }

    polygon << QPoint(x,0);

    // shift polygon upward
    polygon.translate(QPoint( 0, height-shiftY ));
    this->update(); // redraw scene
}
