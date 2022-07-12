#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QStandardItemModel>
#include "dispatch_core.h"
#include "add_query_window.h"
#include <QPainter>
#include <QPen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_updateRequestButton_clicked();

    void on_createRequestButton_clicked();

private:
    Ui::MainWindow *ui;
    int nowClock;
    QList<Query> queryList;
    QStandardItemModel *queryListModel;
    bool eventFilter(QObject *watched,QEvent *event);
    int stationNumber;
    int nowPosition;
private slots:
    void handleNewQuery(Query);
    void on_updateClockButton_clicked();
};
#endif // MAINWINDOW_H
