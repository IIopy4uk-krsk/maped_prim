#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtConcurrent>
#include "custom.h"
#include <QGraphicsRectItem>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userMode(0), m_startPoint(0), m_destPoint(0), m_map(), m_watcher(this)
{
    ui->setupUi(this);

    if (QFile::exists(settingName))
    {
        QSettings set(settingName, QSettings::IniFormat);

        QRect rc = set.value("window").toRect();
        setGeometry(rc);
        m_map.loadFromSetting(set);

        ui->widthEdit->setValue(m_map.getWidth());
        ui->heightEdit->setValue(m_map.getHeight());

        QTimer::singleShot(1, this, SLOT(showMap()));
    }

    // сигнал finished не подходит, потому как есть возможность вызова слота в тот момент, когда результат еще не готов
    connect(&m_watcher, SIGNAL(resultReadyAt(int)), SLOT(showPath(int)));
    connect(this, SIGNAL(destroyed()), this, SLOT(cleanup()));
}

MainWindow::~MainWindow()
{
    cleanup();
    QSettings set(settingName, QSettings::IniFormat);
    set.setValue("window", geometry());

    m_map.saveToSetting(set);

    delete ui;
}

void MainWindow::cleanup()
{
    if (!m_watcher.isFinished())
    {
        m_watcher.cancel();
        m_watcher.waitForFinished();
    }
}

int a1(int)
{
    return 1;
}

void MainWindow::setTempPoint(uint index)
{
    if (!m_watcher.isFinished())
    {
        if (!m_watcher.isCanceled())
        {
            m_watcher.cancel();
            m_watcher.waitForFinished();
        }
        return;
    }
    else
    {
    }


    if (m_map.getFieldConst()[index] != CELL_EMPTY)
        return;

    // костылинг: отслеживаем поиск пути тем же watcherом
    // особенность: для поиска пути по двум постоянным точкам используется QtConcurrent::run, который нельзя отменить.
    // для поиска временного пути до ячейки под курсором мыши, приходится использовать workaround в виде  QtConcurrent::mappedReduced
    QVector<uint> list = {1};
    auto a2 = [=]( QVector<uint> &res, const int &){res = m_map.findPath(m_startPoint, index);};
    m_watcher.setFuture(QtConcurrent::mappedReduced< QVector<uint> >(list, a1, a2));
//    m_watcher.waitForFinished();
}

void MainWindow::setPoint(uint index)
{
    if (!m_watcher.isFinished())
    {
        if (!m_watcher.isCanceled())
            m_watcher.cancel();
        m_watcher.waitForFinished();
    }

    if (m_map.getFieldConst()[index] != CELL_EMPTY)
    {
        QMessageBox::warning(this, "", "Ячейка должна быть без стены!");
        return;
    }

    if (m_userMode == 0)
    {
        m_startPoint = index;
        m_destPoint = -1;
    }
    if (m_userMode == 1)
    {
        m_destPoint = index;

        if (m_destPoint != -1)
            m_watcher.setFuture(QtConcurrent::run(m_map, &MapFile::findPath, m_startPoint, m_destPoint));
    }

    m_userMode = 1 - m_userMode;

}

void MainWindow::showMap()
{
    if (ui->mainField->scene() != nullptr)
        ui->mainField->scene()->deleteLater();

    CustomScene *scene = new CustomScene(this);

    m_map.draw(scene);

    ui->mainField->setScene(scene);
    ui->mainField->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showPath(int )
{
    QMutex mutex;

    mutex.lock();

    if (m_watcher.isRunning())
    {
        m_watcher.cancel();
        return;
    }
//    qDebug() << m_watcher.isStarted() << m_watcher.isPaused() << m_watcher.isRunning() << m_watcher.isCanceled() << m_watcher.isFinished();


    QVector<uint> path = m_watcher.result();

    QString str = QString("Старт: (%1, %2)\n").arg(1 + m_startPoint % m_map.getWidth()).arg(1 + m_startPoint / m_map.getWidth());

    if (m_destPoint != -1)
        str += QString("Финиш: (%3, %4)\n").arg(1 + m_destPoint % m_map.getWidth()).arg(1 + m_destPoint / m_map.getWidth());
    else
        str += QString("\n");

    str += QString("Длина пути: %5").arg(path.size());

    if (m_userMode == 1)
        str += "\n\nУкажите вторую точку.";
    else
        str += "\n\nУкажите первую точку.";
    ui->infoText->setText(str);

    QGraphicsRectItem *gg[2] = {nullptr, nullptr};

     foreach(QGraphicsItem *it, ui->mainField->scene()->items(Qt::AscendingOrder))
     {
         QGraphicsRectItem *rect = dynamic_cast<QGraphicsRectItem *>(it);

         if (rect == nullptr) continue;

         if ((rect->brush().color() != Qt::white) && (rect->brush().color() != Qt::black))
             rect->setBrush(Qt::white);

         if (path.indexOf(it->data(1).toInt()) != -1)
             rect->setBrush(Qt::cyan);
         if (it->data(1).toInt() == m_startPoint)
             gg[0] = rect;
         if (it->data(1).toInt() == m_destPoint)
             gg[1] = rect;
     }

     if (gg[0] != nullptr)
         gg[0]->setBrush(Qt::darkGreen);
     if (gg[1] != nullptr)
         gg[1]->setBrush(Qt::darkRed);

     mutex.unlock();

     path.detach();
}


void MainWindow::on_generateBtn_clicked()
{
    if (!m_watcher.isFinished())
    {
        if (!m_watcher.isCanceled())
            m_watcher.cancel();
        m_watcher.waitForFinished();
    }

//    qDebug() << m_watcher.isStarted() << m_watcher.isPaused() << m_watcher.isRunning() << m_watcher.isCanceled() << m_watcher.isFinished();

    m_map.generate(ui->widthEdit->value(), ui->heightEdit->value());
    showMap();

   m_startPoint = 0;
   m_destPoint = 0;

   while (m_map.getFieldConst()[m_startPoint] == CELL_WALL)
   {
       m_startPoint = qrand () % (m_map.getWidth() * m_map.getHeight());
   }

   while (m_map.getFieldConst()[m_destPoint] == CELL_WALL)
   {
       m_destPoint = qrand () % (m_map.getWidth() * m_map.getHeight());
   }

   m_watcher.setFuture(QtConcurrent::run(m_map, &MapFile::findPath, m_startPoint, m_destPoint));
}

