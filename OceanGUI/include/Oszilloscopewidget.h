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

    void    paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);     // overriding paint() - react on update

};




#endif // OSZILLOSCOPEWIDGET_H
