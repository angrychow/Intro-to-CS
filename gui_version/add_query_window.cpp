#include "add_query_window.h"
#include "ui_add_query_window.h"

add_query_window::add_query_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_query_window)
{
    ui->setupUi(this);
}

add_query_window::~add_query_window()
{
    delete ui;
}

void add_query_window::on_buttonSubmit_clicked()
{
    qDebug()<<ui->serviceTypeComboBox->currentText()<<'\n';
    Query ret ={0,0,0};
    if(ui->serviceTypeComboBox->currentText() == "逆时针请求") {
        ret.type = COUNTERCLOCKWISE;
    } else if(ui->serviceTypeComboBox->currentText() == "顺时针请求") {
        ret.type = CLOCKWISE;
    } else {
        ret.type = TARGET;
    }
    ret.station = ui->lineEdit->text().toInt();
    emit addNewQuery(ret);
}


void add_query_window::on_buttonClear_clicked()
{
    ui->lineEdit->clear();
}

