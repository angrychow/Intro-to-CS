#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->queryListModel = new QStandardItemModel();
    this->ui->tableView->setModel(queryListModel);
    queryListModel->setHorizontalHeaderItem(0,new QStandardItem("服务站点"));
    queryListModel->setHorizontalHeaderItem(1,new QStandardItem("服务类型"));
    queryListModel->setHorizontalHeaderItem(2,new QStandardItem("生成时间"));
    nowClock=0;
    QFile configFile("./dict.txt");
    qDebug() << QCoreApplication::applicationDirPath();
    qDebug() << configFile.exists();
    if(!configFile.open(QIODevice::ReadOnly)) {
        qDebug()<<"Failed to load config file.";
        qDebug()<<configFile.errorString();
    } else {
        QTextStream configStream(&configFile);
        QString ModeTypeTemp;configStream >> ModeTypeTemp;
        QString StationTemp;configStream >> StationTemp;
        QString DistanceTemp;configStream >> DistanceTemp;
        qDebug()<<DistanceTemp;
        int strategy;
        if(ModeTypeTemp == "SSTF") {
            strategy = MODE_SSTF;
        } else if(ModeTypeTemp == "SCAN") {
            strategy = MODE_SCAN;
        } else {
            strategy = MODE_FCFS;
        }
        int distance = DistanceTemp.toInt();
        int totalStation = StationTemp.toInt();
        configFile.close();
        g_initDispatchCore(distance,strategy,totalStation);
    }
    nowPosition=0;
    stationNumber = g_getTotalStation();
    this->ui->widgetBus->installEventFilter(this);
    this->ui->widgetSign->installEventFilter(this);
    this->ui->widgetBus->update();
    this->ui->widgetSign->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_updateRequestButton_clicked()
{
    int* cnt = (int*)malloc(sizeof(int));
    Query* temp = g_getNowQueryList(cnt);

    queryListModel->clear();
    queryListModel->setHorizontalHeaderItem(0,new QStandardItem("服务站点"));
    queryListModel->setHorizontalHeaderItem(1,new QStandardItem("服务类型"));
    queryListModel->setHorizontalHeaderItem(2,new QStandardItem("生成时间"));
    for(int i=0;i<*cnt;i++) {

        queryListModel->setItem(i,0,new QStandardItem(QString::number(temp[i].station)));
//
        if(temp[i].type == CLOCKWISE) {
            queryListModel->setItem(i,1,new QStandardItem("顺时针接送请求"));
        } else if(temp[i].type == COUNTERCLOCKWISE) {
            queryListModel->setItem(i,1,new QStandardItem("逆时针接送请求"));
        } else {
            queryListModel->setItem(i,1,new QStandardItem("下车请求"));
        }
        queryListModel->setItem(i,2,new QStandardItem(QString::number(temp[i].genTime)));
    }
}


void MainWindow::on_createRequestButton_clicked()
{
    add_query_window* queryWindow = new add_query_window();
    connect(queryWindow,SIGNAL(addNewQuery(Query)),this,SLOT(handleNewQuery(Query)));
    queryWindow->show();
}

void MainWindow::handleNewQuery(Query ret) {
    ret.genTime = nowClock;
    g_insertNewQuery(ret);
    qDebug()<<ret.genTime;
    qDebug()<<ret.station;
    qDebug()<<ret.type;
    on_updateRequestButton_clicked();
}

void MainWindow::on_updateClockButton_clicked()
{
//    ui->currentTime->setText(QString::number(++nowClock));
//    ui->lcdNumber->setDigitCount(++nowClock);
    ui->lcdNumber->display(++nowClock);
    nowPosition = g_nextClock();
    on_updateRequestButton_clicked();
    this->ui->widgetSign->update();
    int nowCarState = g_getNowState();
    if(nowCarState == STATE_WAITING || nowCarState== STATE_SERVING) {
        ui->currentStateLabel->setText("停车");
    } else {
        if(g_getNowDirection()==CLOCKWISE) {
            ui->currentStateLabel->setText("顺时针");
        } else {
            ui->currentStateLabel->setText("逆时针");
        }
    }
    ui->currentPositionLabel->setText(QString::number(nowPosition));
    this->ui->widgetBus->repaint();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this->ui->widgetBus && event->type() == QEvent::Paint) {
        int widgetWidth = ui->widgetBus->width();
        int widgetHeight = ui->widgetBus->height();
        QPainter paint(ui->widgetBus);
        QPen widgetPen;
        widgetPen.setColor(QColor(0,0,0));
        widgetPen.setWidth(2);
        paint.setPen(widgetPen);

        paint.drawLine(20,widgetHeight/2,widgetWidth-20,widgetHeight/2);
        int intervene = (widgetWidth-40)/(stationNumber - 1);
        int nowDotPos = 20;
        for(int i=1;i<=stationNumber;i++) {
            widgetPen.setWidth(5);
            paint.setPen(widgetPen);
            paint.drawPoint(nowDotPos,widgetHeight/2);
            paint.drawText(nowDotPos-10,widgetHeight*3/4,QString("车站")+QString::number(i));
            nowDotPos+=intervene;
        }
        widgetPen.setColor(QColor(255,0,0));
        paint.setPen(widgetPen);
        paint.drawPoint(20+(int)((float)intervene*(float)nowPosition/(float)(g_getDistance()/g_getTotalStation())),widgetHeight/2);
        int centerX = 20+(int)((float)intervene*(float)nowPosition/(float)(g_getDistance()/g_getTotalStation()));
        QRect busRect(centerX-10,widgetHeight/4,30,30);
        if(g_getNowDirection()==CLOCKWISE) {
            paint.drawImage(busRect,QImage(":/static/bus_right.png"));
        } else {
            paint.drawImage(busRect,QImage(":/static/bus_left.png"));
        }
        paint.drawText(10+(int)((float)intervene*(float)nowPosition/(float)(g_getDistance()/g_getTotalStation())),widgetHeight*7/8,QString("车辆位置"));
        qDebug()<<g_getDistance();
    }
    if(watched==this->ui->widgetSign&&event->type()==QEvent::Paint) {
        QRect rect = this->ui->widgetSign->rect();
        QPainter paint(this->ui->widgetSign);
        if(g_getNowState()==STATE_RUNNING) {
            if(g_getNowDirection()==CLOCKWISE) {
                paint.drawImage(rect,QImage(":/static/right_sign.png"));
            } else {
                paint.drawImage(rect,QImage(":/static/left_sign.png"));
            }
        } else {
            paint.drawImage(rect,QImage(":/static/stop.png"));
        }

    }
    return QWidget::eventFilter(watched,event);
}
