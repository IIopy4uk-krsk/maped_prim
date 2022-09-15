#include "custom.h"

#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include "mainwindow.h"

CustomScene::CustomScene(QObject *parent) : QGraphicsScene(parent)
{
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if(event->isAccepted())
        return;

    if(event->button() == Qt::LeftButton)
    {
        QGraphicsRectItem *item = nullptr;
        foreach(QGraphicsItem *it, items(event->scenePos()))
        {
            item = dynamic_cast<QGraphicsRectItem *>(it);
            break;
        }

        if (item != nullptr)
        {
            MainWindow *wnd = dynamic_cast<MainWindow *>(parent());
            wnd->setPoint(item->data(1).toUInt());
        }
    }
}
void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (event->isAccepted())
        return;

    QGraphicsRectItem *item = nullptr;
    foreach(QGraphicsItem *it, items(event->scenePos()))
    {
        item = dynamic_cast<QGraphicsRectItem *>(it);
        break;
    }

    if (item != nullptr)
    {
        static uint lastCell = 0;
        MainWindow *wnd = dynamic_cast<MainWindow *>(parent());
        if (item->data(1).toUInt() != lastCell)
        {
            lastCell = item->data(1).toUInt();
            if (wnd->userMode() == 1)
                wnd->setTempPoint(item->data(1).toUInt());

        }
    }
}

void CustomScene::keyPressEvent(QKeyEvent * event)
{
        QGraphicsScene::keyPressEvent(event);
}


void CustomScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    if (views().isEmpty()) return;
    qreal delta = (wheelEvent->delta() > 0) ? 1.1 : 0.9;
    views().first()->scale(delta, delta);
}
