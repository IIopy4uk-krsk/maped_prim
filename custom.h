#ifndef CUSTOM_H
#define CUSTOM_H
#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

///
/// \brief Графическая сцена с возможностью взаимодействия с пользователем.
///
class CustomScene : public QGraphicsScene
{
public:
    /// конструктор сцены
    /// \param [in] parent владелец объекта, необходимо для корректного удаления
    explicit CustomScene(QObject *parent);
protected:
    /// событие сцены "клик кнопки мыши"
    /// \param [in] event указывает какая кнопка, с какими модификаторами была нажата в каких координатах и т.д.
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /// событие сцены "перемещение указателя мыши"
    /// \param [in] event указывает параметры, модификаторы и координаты мыши во время перемещения
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    /// событие сцены "нажатие клавиши клавиатуры во время фокуса"
    /// \param [in] event указывает параметры и модификаторы нажатия
    void keyPressEvent(QKeyEvent *event);
    /// событие сцены "перемещение колёсика мыши"
    /// \param [in] event указывает параметры, модификаторы и смещение колёсика мыши
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

};
#endif
