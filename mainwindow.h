#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include "mapfile.h"
#include <QDebug>
#include <QFutureWatcher>

namespace Ui {
class MainWindow;
}

///
/// \brief Основное окно программы.
/// \details Класс окна, в котором происходит взаимодействие пользователя с программой.
/// В нём прописаны реакции на ввод данных, нажатия кнопок и т.д.
/// \warning Реакция на клик по ячейке карты реализован в классе CustomScene.
///
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int m_userMode READ userMode)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    /// даём доступ на чтение к члену m_userMode
    /// \return значение m_userMode
    int userMode()
    {
        return m_userMode;
    }

    /// устанавливаем точку начальную или конечную для поиска пути
    /// \param [in] index координаты точки
    void setPoint(uint index);
    /// ищем путь до временной точки, над которой стоит курсор (клика ещё нет)
    /// \param [in] index координаты временной точки
    void setTempPoint(uint index);

private slots:
    /// чистим за собой. Если работает поток поиска пути, отменяем (если получится) и ждём его завершения.
    void cleanup();
    /// отображаем путь на уже существующей карте; параметр не используется.
    /// этот слот с параметром нужен для отлова сигнала resultReadyAt от m_watcher. Сигнал finished без параметра не гарантирует готовность результата
    void showPath(int = 0);
    /// отображаем карту
    void showMap();
    /// кликаем на кнопку генерации карты
    void on_generateBtn_clicked();

private:
    /// элементы графического интерфейса окна
    Ui::MainWindow *ui;

    /// режим клика по ячейке - какую точку пути ставить, начальную или конечную
    int m_userMode;
    /// координаты начальной точки
    int m_startPoint;
    /// координаты конечной точки
    int m_destPoint;
    /// карта
    MapFile m_map;
    /// следилка за потоком поиска пути
    QFutureWatcher< QVector<uint> > m_watcher;

    /// имя файла настроек для сохранения-загрузки
    const QString settingName = "maped.ini";
};

#endif // MAINWINDOW_H
