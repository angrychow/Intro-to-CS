/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *currentPositionLabel;
    QLabel *label_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widgetSign;
    QLabel *currentStateLabel;
    QLabel *label_3;
    QLCDNumber *lcdNumber;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *updateRequestButton;
    QPushButton *updateClockButton;
    QPushButton *createRequestButton;
    QWidget *widgetBus;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(540, 667);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(19);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        currentPositionLabel = new QLabel(frame);
        currentPositionLabel->setObjectName(QString::fromUtf8("currentPositionLabel"));
        QFont font1;
        font1.setPointSize(15);
        currentPositionLabel->setFont(font1);
        currentPositionLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(currentPositionLabel);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(90, 100));
        widget->setSizeIncrement(QSize(0, 0));
        widget->setBaseSize(QSize(50, 50));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        widgetSign = new QWidget(widget);
        widgetSign->setObjectName(QString::fromUtf8("widgetSign"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetSign->sizePolicy().hasHeightForWidth());
        widgetSign->setSizePolicy(sizePolicy1);
        widgetSign->setSizeIncrement(QSize(0, 0));

        horizontalLayout_3->addWidget(widgetSign);

        currentStateLabel = new QLabel(widget);
        currentStateLabel->setObjectName(QString::fromUtf8("currentStateLabel"));
        currentStateLabel->setFont(font1);
        currentStateLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(currentStateLabel);


        verticalLayout->addWidget(widget);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        lcdNumber = new QLCDNumber(frame);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setDigitCount(7);
        lcdNumber->setProperty("value", QVariant(0.000000000000000));

        verticalLayout->addWidget(lcdNumber);


        horizontalLayout->addWidget(frame);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->horizontalHeader()->setCascadingSectionResizes(true);
        tableView->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(tableView);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        updateRequestButton = new QPushButton(centralwidget);
        updateRequestButton->setObjectName(QString::fromUtf8("updateRequestButton"));

        horizontalLayout_2->addWidget(updateRequestButton);

        updateClockButton = new QPushButton(centralwidget);
        updateClockButton->setObjectName(QString::fromUtf8("updateClockButton"));

        horizontalLayout_2->addWidget(updateClockButton);

        createRequestButton = new QPushButton(centralwidget);
        createRequestButton->setObjectName(QString::fromUtf8("createRequestButton"));

        horizontalLayout_2->addWidget(createRequestButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        widgetBus = new QWidget(centralwidget);
        widgetBus->setObjectName(QString::fromUtf8("widgetBus"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(widgetBus->sizePolicy().hasHeightForWidth());
        widgetBus->setSizePolicy(sizePolicy2);
        widgetBus->setMinimumSize(QSize(0, 120));
        widgetBus->setSizeIncrement(QSize(0, 0));
        widgetBus->setBaseSize(QSize(50, 100));

        verticalLayout_2->addWidget(widgetBus);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 540, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\260\203\345\272\246\347\252\227\345\217\243", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\350\275\246\350\276\206\344\275\215\347\275\256", nullptr));
        currentPositionLabel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\350\275\246\350\276\206\347\212\266\346\200\201", nullptr));
        currentStateLabel->setText(QCoreApplication::translate("MainWindow", "\345\201\234\350\275\246", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\227\266\351\227\264", nullptr));
        updateRequestButton->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\226\260\350\257\267\346\261\202", nullptr));
        updateClockButton->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\347\247\222", nullptr));
        createRequestButton->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\242\236\350\257\267\346\261\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
