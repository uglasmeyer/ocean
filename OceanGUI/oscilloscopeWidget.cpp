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
