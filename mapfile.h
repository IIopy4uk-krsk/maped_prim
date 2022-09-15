#ifndef MAPFILE_H
#define MAPFILE_H
#pragma once

#include <QSettings>
#include <QGraphicsScene>

#define DEFAULT_MAP_SIZE    (21)
#define CELL_EMPTY          (0)
#define CELL_WALL           (1)
#define CELL_SIZE           (8)

///
/// \brief Карта со стенами и проходами
///
class MapFile
{
private:
    /// ширина лабиринта
    uint m_width;
    /// высота лабиринта
    uint m_height;
    /// поле лабиринта, представляет собой массив чисел в котором 0 - пустая клетка, 1 - стена
    uchar *m_field;

public:

    /// конструктор по умолчанию
    /// \param [in] w,h ширина и высота, возможно оставить по умолчанию
    MapFile();
    /// конструктор c заданием размеров лабиринта
    /// \param [in] w,h ширина и высота, возможно оставить по умолчанию
    MapFile(int w, int h);

    /// конструктор копирования
    /// \param [in] src объект откуда копируются данные
    MapFile(const MapFile& src);
    /// оператор присваивания
    /// \param [in] src объект откуда копируются данные
    MapFile &operator=(const MapFile &src);

    /// чистим массивы
    ~MapFile();

    /// предоставление поля для чтения
    /// \return массив байт, представляющее поле лабиринта
    uchar *getFieldConst() const
    {
        return m_field;
    }

    /// предоставление поля для чтения и записи
    /// \return массив байт, представляющее поле лабиринта
    uchar *getField()
    {
        return m_field;
    }

    /// предоставление ширины лабиринта для чтения
    /// \return ширина лабиринта
    uint getWidth() const
    {
        return m_width;
    }

    /// предоставление высоты лабиринта для чтения
    /// \return высота лабиринта
    uint getHeight() const
    {
        return m_height;
    }

    /// загрузка лабиринта из настроек
    /// \param [in] set объект настроек, откуда загружаются данные лабиринта
    void loadFromSetting(const QSettings &set);
    /// сохранение лабиринта в настройки
    /// \param [out] set объект настроек, куда загружаются данные лабиринта
    void saveToSetting(QSettings &set) const;

    /// простенький генератор лабиринта шириной w и высотой h
    /// \param [in] w ширина лабиринта
    /// \param [in] h высота лабиринта
    void generate(int w, int h);

    /// реализация поиска кратчайшего пути через поиск в ширину
    /// \param [in] src начальная точка
    /// \param [in] dest конечная точка
    /// \return перечень ячеек на пути из src в dest
    QVector<uint> findPath(uint src, uint dest) const;

    /// пересчёт от координат ячейки в индекс массива; обратная операция не требуется (пока)
    /// \param [in] x,y координаты ячейки
    /// \return индекс массива
    int coordToIndex(uint x, uint y) const;

    /// отрисовка карты на графической сцене; происходит путём создания квадратов в сцене
    /// \param [out] scene сцена, в которой рисуется карта
    void draw (QGraphicsScene *scene) const;
};

#endif // MAPFILE_H
