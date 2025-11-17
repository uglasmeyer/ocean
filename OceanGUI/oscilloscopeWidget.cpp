//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
    QGraphicsPolygonItem polygon_item;
    painter->drawRect( drawregion );
    polygon_item.setPolygon( polygon);
    polygon_item.paint( painter, option, widget);

    int cursor = (int)sds->WD_status.cursor;
    if ( cursor > 0  )
    {
    	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    	painter->drawLine( cursor, 0, cursor, height);
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
