#ifndef ADD_QUERY_WINDOW_H
#define ADD_QUERY_WINDOW_H

#include <QWidget>
#include "dispatch_core.h"

namespace Ui {
class add_query_window;
}

class add_query_window : public QWidget
{
    Q_OBJECT

public:
    explicit add_query_window(QWidget *parent = nullptr);
    ~add_query_window();

private:
    Ui::add_query_window *ui;

signals:
    void addNewQuery(Query);
private slots:
    void on_buttonSubmit_clicked();
    void on_buttonClear_clicked();
};

#endif // ADD_QUERY_WINDOW_H
