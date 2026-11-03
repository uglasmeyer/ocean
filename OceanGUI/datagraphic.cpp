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

#include <DataGraphic.h>
#include <GUIcommon.h>
#include <QPainter>

/**************************************************
 * DataGraphic_class
 *************************************************/
DataGraphic_class::DataGraphic_class( interface_t* ifd
									, QGraphicsView* view
									, datagraphics_param_t _param )
	: Logfacility_class		( "DataGraphic_class" )
{
	this->graphicsview		= view;
	this->drawregion		= boundingRect();
    this->height 			= drawregion.height();
	Set_parameter			( _param );
	Set_sds					( ifd );
};

QRectF DataGraphic_class::boundingRect() const
{
	const QRectF rect 		= this->graphicsview->geometry();
    const QRectF region		= { 0, 0, rect.width()-10, rect.height()-10 };
    return region;
}

void DataGraphic_class::Set_sds( interface_t* _sds )
{
	this->sds = _sds;
}

void DataGraphic_class::Set_parameter( datagraphics_param_t _param )
{
	this->param				= _param;
	this->shiftY			= height / 2 * param.centerY;
	this->param.scale		= param.scale*height;
}

void DataGraphic_class::wheelEvent( QGraphicsSceneWheelEvent* event )
{
	float zoom = sds->WD_state.zoom;
	// delta() is the vertical scroll amount int +/- 120
	int delta = event->delta();

	if (delta > 0)
	{
		zoom *= zoomFactor; // Zoom in
	}
	if (delta < 0)
	{
		zoom /= zoomFactor; // Zoom out
	}
	// Clamp zoom to avoid extreme values
	zoom = clamp( zoom, minZoom, maxZoom );
	sds->WD_state.zoom = zoom;
	event->accept();
} ;

void DataGraphic_class::paint(	QPainter* painter,
								const QStyleOptionGraphicsItem* option,
								QWidget* widget )
{

	auto CursorLine = [ this, painter ]( Qt::GlobalColor color, uint16_t cursor )
	{
    	painter->setPen		( QPen(color, 1, Qt::SolidLine, Qt::RoundCap) );
    	painter->drawLine	( cursor, 0, cursor, height );
	};

	auto showCursor = [ this, CursorLine ](  )
	{
	    uint16_t cursor 	= sds->WD_state.cursor.cur;
	    if ( cursor > 0  )
	    {
	    	CursorLine( Qt::black, cursor );
	    }
	    if ( sds->WD_state.mode == wavedisplay_t::RANGE_WD )
	    {
	    	CursorLine( Qt::green, sds->WD_state.cursor.min );
	    	CursorLine( Qt::green, sds->WD_state.cursor.max );
	    }
	};

	auto showZoom = [ this, painter ]( float zoomFactor )
	{
		stringstream	strs	{};
		strs << setprecision(4) << dec << "zoom: " << 100/zoomFactor << "%";
		QString	txt				= Qstring( strs.str() );
		painter->drawText		( 10, 20, txt );
	};

	Comment( DEBUG, "DataGraphic_class::paint" );

	painter->drawLine( drawregion.topLeft(), drawregion.topRight() );
	painter->drawLine( drawregion.bottomLeft(), drawregion.bottomRight() );
    if( param.cursor )
    	showCursor			();
    QGraphicsPolygonItem 	polygon_item { polygon };
    polygon_item.paint		( painter, option, widget );
    float zoom				= sds->WD_state.zoom;
    if( zoom < 1.0f )
    	showZoom( zoom );

}

Data_t DataGraphic_class::readDataBegin()
{
	polygon.clear();
    polygon << QPoint( 0, 0 );
    return 0;
}
void DataGraphic_class::readDataEnd( Data_t x )
{
	polygon << QPoint( x, 0 );// horizontal line

    // shift polygon upward
	polygon.translate( QPoint( 0, height-shiftY ) );

}

void DataGraphic_class::ReadData( Data_t* Y, int len )
{
	Data_t X = readDataBegin();
    for ( int n = 0; n < len; n++ )
    {
        polygon << QPoint( X, - Y[n] * param.scale );
        X = n;
    }
    readDataEnd( X );
}

