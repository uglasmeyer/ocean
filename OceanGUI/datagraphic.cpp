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
 * datagraphic.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



// C++ Project
// https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene_QGraphicsItems.php

#include <qpainter.h>
#include <include/DataGraphicClass.h>

/**************************************************
 * DataGraphic_class
 *************************************************/
DataGraphic_class::DataGraphic_class( interface_t* ifd, QGraphicsView* view )
	: Logfacility_class		( "DataGraphic_class" )

{
    this->sds   			= ifd;
	this->graphicsview		= view;
	this->drawregion		= boundingRect();
    this->height 			= drawregion.height();
    this->shiftY 			= height / 2;
    this->data_scale		= height / max_data_amp / 2.0;
    this->ReadWaveData	();		// draw scene on init
};

QRectF DataGraphic_class::boundingRect() const
{
	const QRectF rect 		= this->graphicsview->geometry();
    const QRectF region		= { 0, 0, rect.width()-10, rect.height()-10 };
    return region;
}

void DataGraphic_class::paint(	QPainter* painter,
								const QStyleOptionGraphicsItem* option,
								QWidget* widget )
{

	auto CursorLine = [ this, painter ]( Qt::GlobalColor color, uint16_t cursor )
	{
    	painter->setPen		( QPen(color, 1, Qt::SolidLine, Qt::RoundCap) );
    	painter->drawLine	( cursor, 0, cursor, height );
	};

	Comment( DEBUG, "DataGraphic_class::paint" );

    QGraphicsPolygonItem 	polygon_item;
    painter->drawRect		( drawregion );
    polygon_item.setPolygon	( polygon );
    polygon_item.paint		( painter, option, widget );

    uint16_t cursor 		= sds->WD_state.cursor.cur;
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

Data_t DataGraphic_class::readDataEntry()
{
	polygon.clear();
    polygon << QPoint( 0, 0 );
    return 0;
}
void DataGraphic_class::readDataExit( Data_t x )
{
    polygon << QPoint( x, 0 );// horizontal line

    // shift polygon upward
    polygon.translate( QPoint( 0, height-shiftY ) );
    this->update(); // redraw scene
}

void DataGraphic_class::ReadAdsrData()
{
	Data_t x = readDataEntry();
    for ( auto y : sds->adsrdata )
    {
        polygon << QPoint( x, - y * data_scale );
        x++;
    }
    readDataExit( x );
}

void DataGraphic_class::ReadWaveData()
{

    Data_t x = readDataEntry();
    for ( auto y : sds->wavedata )
    {
        polygon << QPoint( x, - y * data_scale );
        x++;
    }
    readDataExit( x );
}
