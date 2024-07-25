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
    OszilloscopeWidget::polygon.clear();
    OszilloscopeWidget::polygon << QPoint(0,0);

    Data_t x = 0;
    for ( auto y : ifd->wavedata )
    {
        OszilloscopeWidget::polygon << QPoint(x, - y/200);
        x++;
    }

    OszilloscopeWidget::polygon << QPoint(x,0);

    // shift polygon upward
    int height = drawregion.height() ;
    int shiftY = (drawregion.height() )/2;
    OszilloscopeWidget::polygon.translate(QPoint( 0, height-shiftY ));
    this->update(); // redraw scene
}
