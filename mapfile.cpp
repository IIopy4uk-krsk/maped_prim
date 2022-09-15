#include "mapfile.h"
#include <QDebug>
#include <QTime>
#include <QGraphicsRectItem>

MapFile::MapFile() : m_width(0), m_height(0), m_field(nullptr)
{
}

MapFile::MapFile(int w, int h) : m_width(w), m_height(h), m_field(new uchar[m_width * m_height])
{
    memset(m_field, 0, m_width * m_height * sizeof(m_field[0]));

    for (uint i = 0; i < m_width; i++)
    {
        m_field[coordToIndex(i, 0)]            = CELL_WALL;
        m_field[coordToIndex(i, m_height - 1)] = CELL_WALL;
    }

    for (uint i = 0; i < m_height; i++)
    {
        m_field[coordToIndex(0, i)]            = CELL_WALL;
        m_field[coordToIndex(m_width - 1, i)]  = CELL_WALL;
    }
}

MapFile::MapFile(const MapFile& src)
{
    m_width = src.m_width;
    m_height = src.m_height;

    m_field = new uchar[m_width * m_height];

    memcpy(m_field, src.getFieldConst(), m_width * m_height * sizeof(m_field[0]));
}

MapFile::~MapFile()
{
    if (m_field != nullptr)
        delete []m_field;
}

MapFile &MapFile::operator=(const MapFile &src)
{
    if (this == &src)
        return *this;

    m_width = src.m_width;
    m_height = src.m_height;

    m_field = new uchar[m_width * m_height];

    memcpy(m_field, src.getFieldConst(), m_width * m_height * sizeof(m_field[0]));

    return *this;
}

void MapFile::loadFromSetting(const QSettings &set)
{
    m_width = set.value("width", DEFAULT_MAP_SIZE).toUInt();
    m_height = set.value("height", DEFAULT_MAP_SIZE).toUInt();

    if (m_field != nullptr)
        delete []m_field;
    m_field = new uchar[m_width * m_height];

    QByteArray arr = set.value("field").toByteArray();
    memcpy(m_field, arr.constData(), m_width * m_height * sizeof(m_field[0]));
}

void MapFile::saveToSetting(QSettings &set) const
{
    set.setValue("width", m_width);
    set.setValue("height", m_height);

    QByteArray arr(reinterpret_cast<char *>(m_field), m_width * m_height * sizeof(m_field[0]));

    set.setValue("field", arr);
}

void MapFile::generate(int w, int h)
{
    m_width = w;
    m_height = h;

    if (m_field != nullptr)
        delete []m_field;

    m_field = new uchar[m_width * m_height];
    memset(m_field, 0xFF, m_width * m_height * sizeof(m_field[0]));

    qsrand(QTime::currentTime().msec());

    int startx = 1 + qrand() % (m_width - 2);
    int starty = 1 + qrand() % (m_height - 2);

    m_field[coordToIndex(startx, starty)] = CELL_EMPTY;

    // возможные стены
    if (m_field[coordToIndex(startx + 1, starty)] == 0xFF)
        m_field[coordToIndex(startx + 1, starty)] = 0xFE;
    if (m_field[coordToIndex(startx - 1, starty)] == 0xFF)
        m_field[coordToIndex(startx - 1, starty)] = 0xFE;
    if (m_field[coordToIndex(startx, starty + 1)] == 0xFF)
        m_field[coordToIndex(startx, starty + 1)] = 0xFE;
    if (m_field[coordToIndex(startx, starty - 1)] == 0xFF)
        m_field[coordToIndex(startx, starty - 1)] = 0xFE;

    uint col = 0;
    while (col < (m_width + m_height) * 10)
    {
        int x = 0, y = 0;

        do {
            x = 1 + qrand() % (m_width - 2);
            y = 1 + qrand() % (m_height - 2);

        } while (m_field[coordToIndex(x, y)] != 0xFE);

        int p = 0;
        if (m_field[coordToIndex(x + 1, y)] == 0xFF) p++;
        if (m_field[coordToIndex(x - 1, y)] == 0xFF) p++;
        if (m_field[coordToIndex(x, y - 1)] == 0xFF) p++;
        if (m_field[coordToIndex(x, y + 1)] == 0xFF) p++;

        if (p < 2) { col++; continue; }

        col = 0;


        m_field[coordToIndex(x, y)] = CELL_EMPTY;

        if (m_field[coordToIndex(x + 1, y)] == 0xFF)
            m_field[coordToIndex(x + 1, y)] = 0xFE;
        if (m_field[coordToIndex(x - 1, y)] == 0xFF)
            m_field[coordToIndex(x - 1, y)] = 0xFE;
        if (m_field[coordToIndex(x, y + 1)] == 0xFF)
            m_field[coordToIndex(x, y + 1)] = 0xFE;
        if (m_field[coordToIndex(x, y - 1)] == 0xFF)
            m_field[coordToIndex(x, y - 1)] = 0xFE;
    }

    for (uint j = 0; j < m_height; j++)
    {
        for (uint i = 0; i < m_width; i++)
        {
            if ((m_field[coordToIndex(i, j)] == 0xFE) ||
                (m_field[coordToIndex(i, j)] == 0xFF))
                m_field[coordToIndex(i, j)] = CELL_WALL;
        }

        m_field[coordToIndex(j, 0)] = CELL_WALL;
        m_field[coordToIndex(0, j)] = CELL_WALL;
        m_field[coordToIndex(m_width - 1, j)] = CELL_WALL;
        m_field[coordToIndex(j, m_height - 1)] = CELL_WALL;
    }

    for (uint j = 1; j < m_width - 2; j++)
        for (uint i = 1; i < m_height - 2; i++)
            if ((m_field[coordToIndex(j + 0, i + 0)] == CELL_WALL) &&
                (m_field[coordToIndex(j + 1, i + 0)] == CELL_WALL) &&
                (m_field[coordToIndex(j + 0, i + 1)] == CELL_WALL) &&
                (m_field[coordToIndex(j + 1, i + 1)] == CELL_WALL))
            {
                int z = qrand() % 4;
                switch (z)
                {
                case 0: m_field[coordToIndex(j + 0, i + 0)] = CELL_EMPTY; break;
                case 1: m_field[coordToIndex(j + 1, i + 0)] = CELL_EMPTY; break;
                case 2: m_field[coordToIndex(j + 0, i + 1)] = CELL_EMPTY; break;
                case 3: m_field[coordToIndex(j + 1, i + 1)] = CELL_EMPTY; break;
                }
            }
}

void MapFile::draw(QGraphicsScene *scene) const
{
    if (m_field == nullptr)
        return;

    for (uint j = 0; j < m_height; j++)
    for (uint i = 0; i < m_width; i++)
    {
        QGraphicsRectItem *item = new QGraphicsRectItem(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        item->setData(1, coordToIndex(i, j));

        switch(m_field[coordToIndex(i,j)])
        {
        case CELL_EMPTY:
            item->setBrush(Qt::white);
            break;
        case CELL_WALL:
            item->setBrush(Qt::black);
            break;
        default:
            item->setBrush(Qt::gray);
            break;
        }

        scene->addItem(item);
}

}

QVector<uint> MapFile::findPath(uint src, uint dest) const
{
    if (m_field == nullptr)
        return QVector<uint>();

    QVector<uint> result;

    if (src == dest) return QVector<uint>();
    if ((m_field[src] == CELL_WALL) || (m_field[dest] == CELL_WALL))
        return QVector<uint>();

    QVector<uint> nodes;
    QMap<uint,uint> steps;

    nodes.push_back(src);

    auto checkLambda = [&](uint cell)
    {
        if ((m_field[cell] == CELL_EMPTY) && (steps.find(cell) == steps.end()) && (nodes.indexOf(cell) == -1))
        {
            nodes.push_back(cell);
        }
    };

    while (!nodes.isEmpty())
    {
        uint cur = nodes.takeFirst();

        if (cur == src)
            steps.insert(cur, 0);
        else
        {
            uint a = qMin(steps.value(cur - 1, INT_MAX) , steps.value(cur + 1, INT_MAX));
            uint b = qMin(steps.value(cur - m_width, INT_MAX) , steps.value(cur + m_width, INT_MAX));
            steps.insert(cur, qMin(a, b) + 1);
        }

        if (cur == dest)
            break;

        checkLambda(cur - m_width);
        checkLambda(cur + m_width);
        checkLambda(cur - 1);
        checkLambda(cur + 1);
    }

    if (steps.find(dest) == steps.end())
        return QVector<uint>();

    // идём с конца до начала, зная количество ходов до цели
    result.push_front(dest);
    uint last_cell = dest;
    for (uint i = steps[dest] - 1; i > 0; i--)
    {
        QMapIterator<uint, uint> it(steps);
        while (it.hasNext())
        {
            it.next();

            if ((last_cell - it.key() == m_width) || (it.key() - last_cell == m_width) || (last_cell - it.key() == 1) || (it.key() - last_cell == 1))
            if ((it.value() == i) )
            {
                last_cell = it.key();
                result.push_front(it.key());
                break;
            }

        }
    }
    result.push_front(src);

    return result;
}

int MapFile::coordToIndex(uint x, uint y) const
{
    return x + m_width * y;
}
