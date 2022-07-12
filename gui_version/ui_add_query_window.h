/********************************************************************************
** Form generated from reading UI file 'add_query_window.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_QUERY_WINDOW_H
#define UI_ADD_QUERY_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_add_query_window
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QComboBox *serviceTypeComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonSubmit;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonClear;

    void setupUi(QWidget *add_query_window)
    {
        if (add_query_window->objectName().isEmpty())
            add_query_window->setObjectName(QString::fromUtf8("add_query_window"));
        add_query_window->resize(313, 238);
        verticalLayout = new QVBoxLayout(add_query_window);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(add_query_window);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        serviceTypeComboBox = new QComboBox(add_query_window);
        serviceTypeComboBox->addItem(QString());
        serviceTypeComboBox->addItem(QString());
        serviceTypeComboBox->addItem(QString());
        serviceTypeComboBox->setObjectName(QString::fromUtf8("serviceTypeComboBox"));

        horizontalLayout->addWidget(serviceTypeComboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(add_query_window);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit = new QLineEdit(add_query_window);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonSubmit = new QPushButton(add_query_window);
        buttonSubmit->setObjectName(QString::fromUtf8("buttonSubmit"));

        horizontalLayout_3->addWidget(buttonSubmit);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        buttonClear = new QPushButton(add_query_window);
        buttonClear->setObjectName(QString::fromUtf8("buttonClear"));

        horizontalLayout_3->addWidget(buttonClear);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(add_query_window);

        QMetaObject::connectSlotsByName(add_query_window);
    } // setupUi

    void retranslateUi(QWidget *add_query_window)
    {
        add_query_window->setWindowTitle(QCoreApplication::translate("add_query_window", "\346\226\260\345\242\236\350\257\267\346\261\202\347\252\227\345\217\243", nullptr));
        label->setText(QCoreApplication::translate("add_query_window", "\350\257\267\346\261\202\347\261\273\345\236\213", nullptr));
        serviceTypeComboBox->setItemText(0, QCoreApplication::translate("add_query_window", "\351\241\272\346\227\266\351\222\210\350\257\267\346\261\202", nullptr));
        serviceTypeComboBox->setItemText(1, QCoreApplication::translate("add_query_window", "\351\200\206\346\227\266\351\222\210\350\257\267\346\261\202", nullptr));
        serviceTypeComboBox->setItemText(2, QCoreApplication::translate("add_query_window", "\350\275\246\345\206\205\350\257\267\346\261\202", nullptr));

        label_2->setText(QCoreApplication::translate("add_query_window", "\350\257\267\346\261\202\347\253\231\347\202\271", nullptr));
        buttonSubmit->setText(QCoreApplication::translate("add_query_window", "\346\217\220\344\272\244\350\257\267\346\261\202", nullptr));
        buttonClear->setText(QCoreApplication::translate("add_query_window", "\346\270\205\347\251\272\345\206\205\345\256\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_query_window: public Ui_add_query_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_QUERY_WINDOW_H
