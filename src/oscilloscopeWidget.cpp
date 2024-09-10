// C++ Project
// https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene_QGraphicsItems.php

#include <qpainter.h>
#include <oszilloscopewidget.h>


QRectF OszilloscopeWidget::boundingRect() const
{
    return QRectF( drawregion );
}

OszilloscopeWidget::OszilloscopeWidget( ifd_t* ifd, QRectF rectangle)
{
    drawregion  = { 0,0,rectangle.width()-10, rectangle.height()-10 };

    height 		= drawregion.height();
    shiftY 		= height / 2;
	data_scale	= height / max_data_amp / 2;

    this->ifd   = ifd;
    this->update();
};

void OszilloscopeWidget::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    QGraphicsPolygonItem polygon_item;
    painter->drawRect( drawregion );
    polygon_item.setPolygon( polygon);
    polygon_item.paint( painter, option, widget);
}

void OszilloscopeWidget::read_polygon_data(  )
{
    // init polygon
	polygon.clear();
    polygon << QPoint(0,0);

    Data_t x = 0;
    for ( auto y : ifd->wavedata )
    {
        polygon << QPoint(x, - y * data_scale );
        x++;
    }

    polygon << QPoint(x,0);

    // shift polygon upward
    polygon.translate(QPoint( 0, height-shiftY ));
    this->update(); // redraw scene
}
