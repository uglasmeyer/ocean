
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
    QPolygon        polygon;
    uint 			shiftY 		= 0;
    uint			height		= 0;
    float			data_scale	= 0;

    QRectF  boundingRect() const; // implement virtual method boundingRect, need by qgraphicsitem.h

    void    paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);     // overriding paint() - react on update

};




#endif // OSZILLOSCOPEWIDGET_H
