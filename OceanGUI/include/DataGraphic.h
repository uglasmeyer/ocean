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
 * DataGraphic.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSZILLOSCOPEWIDGET_H
#define OSZILLOSCOPEWIDGET_H

// Qt
#include <QPolygon>
#include <QGraphicsWidget>
#include <QGraphicsView>

// Ocean
#include <data/Interface.h>

/**************************************************
 * Pushbutton_class
 * add rightclick event to qpushbutton
 *************************************************/
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QGraphicsSceneEvent>

/**************************************************
 * Pushbutton_class
 * provides direction event for Pushbuttons
 *************************************************/
class Pushbutton_class :
	public QPushButton
{
	Q_OBJECT
public:
	Pushbutton_class( QPushButton* pb ) :
		QPushButton( pb )
	{
        pb->installEventFilter( this );

	};
	virtual ~Pushbutton_class() = default;

signals:
	void clickdirection( int );

protected:
	inline bool eventFilter(QObject* watched, QEvent* event) override
    {
        if ( event->type() == QEvent::MouseButtonPress )
        {
            auto mouseEvent = (QMouseEvent*)event;
            switch ( mouseEvent->button() )
			{
				case Qt::RightButton	: { emit( clickdirection( -1 ) ); break; }
				case Qt::LeftButton		: { emit( clickdirection(  1 ) ); break; }
				default 				: { break; }
			} // switch mouseEvent->button()
        }
        return false;
    };

};

// https://doc.qt.io/qt-6/eventsandfilters.html
// https://doc.qt.io/qt-6/qgraphicsview.html
// https://forum.qt.io/topic/86500/understanding-paintevent


/**************************************************
 * DataGraphic_class
 *************************************************/
typedef struct datagraphics_struct
{
	Data_t	scale	= 0;
	bool 	cursor	= false;
	bool 	centerY	= false;
} 	datagraphics_param_t;

class DataGraphic_class
	: 			Logfacility_class
	, public	QGraphicsItem

{
	const float	zoomFactor	= pow( 2.0, 1.0/12.0 );	// 1.059463094
	const float minZoom 	= pow( 2.0, -5.0 );		// 0.03125
	const float maxZoom 	= pow( 2.0, 0.0 );

    QRectF          		drawregion		;
    QGraphicsView*			graphicsview	;
    QPolygonF       		polygon			;
    uint 					shiftY 			;
    uint					height			;
    datagraphics_param_t	param			;
    interface_t*          	sds				;

public:

    		DataGraphic_class	( interface_t*, QGraphicsView*, datagraphics_param_t _param );
    virtual	~DataGraphic_class	() = default;

    void    ReadData			( Data_t* Data, int len );
    void	Set_parameter		( datagraphics_param_t _param );
    void	Set_sds				( interface_t* _sds );

private:
    Data_t 	readDataBegin();
    void 	readDataEnd( Data_t x );
    QRectF  boundingRect() const override; // implement virtual method boundingRect, need by qgraphicsitem.h
    void    paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;     // overriding paint() - react on update

protected:
    void 	wheelEvent( QGraphicsSceneWheelEvent* event ) override;

};




#endif // OSZILLOSCOPEWIDGET_H
