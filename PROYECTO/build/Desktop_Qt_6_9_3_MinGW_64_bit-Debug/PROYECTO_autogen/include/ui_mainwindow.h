/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLCDNumber *lcdNumber;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QRadioButton *radioButton;
    QLabel *label_2;
    QPushButton *bot_vel_subir;
    QPushButton *bot_vel_bajar;
    QLabel *label_3;
    QLabel *indicatorLed;
    QTextEdit *te_sentido;
    QTextEdit *estado_motor;
    QLabel *label_4;
    QLabel *label_5;
    QTextEdit *te_temp;
    QPushButton *bot_inv_giro;
    QPlainTextEdit *visorLog;
    QPushButton *on_btn_borrar_logs;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1124, 495);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lcdNumber = new QLCDNumber(centralwidget);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setGeometry(QRect(10, 40, 251, 141));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QRadialGradient gradient(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient.setSpread(QGradient::Spread::PadSpread);
        gradient.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient.setColorAt(0, QColor(0, 229, 255, 255));
        gradient.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush1(gradient);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        QRadialGradient gradient1(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient1.setSpread(QGradient::Spread::PadSpread);
        gradient1.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient1.setColorAt(0, QColor(0, 229, 255, 255));
        gradient1.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush3(gradient1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush3);
        QRadialGradient gradient2(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient2.setSpread(QGradient::Spread::PadSpread);
        gradient2.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient2.setColorAt(0, QColor(0, 229, 255, 255));
        gradient2.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush4(gradient2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush4);
        QBrush brush5(QColor(255, 255, 255, 128));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        QRadialGradient gradient3(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient3.setSpread(QGradient::Spread::PadSpread);
        gradient3.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient3.setColorAt(0, QColor(0, 229, 255, 255));
        gradient3.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush6(gradient3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        QRadialGradient gradient4(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient4.setSpread(QGradient::Spread::PadSpread);
        gradient4.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient4.setColorAt(0, QColor(0, 229, 255, 255));
        gradient4.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush7(gradient4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush7);
        QRadialGradient gradient5(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient5.setSpread(QGradient::Spread::PadSpread);
        gradient5.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient5.setColorAt(0, QColor(0, 229, 255, 255));
        gradient5.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush8(gradient5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush8);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        QRadialGradient gradient6(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient6.setSpread(QGradient::Spread::PadSpread);
        gradient6.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient6.setColorAt(0, QColor(0, 229, 255, 255));
        gradient6.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush9(gradient6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush9);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        QRadialGradient gradient7(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient7.setSpread(QGradient::Spread::PadSpread);
        gradient7.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient7.setColorAt(0, QColor(0, 229, 255, 255));
        gradient7.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush10(gradient7);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush10);
        QRadialGradient gradient8(0.5, 0.5, 0.7, 0.5, 0.5);
        gradient8.setSpread(QGradient::Spread::PadSpread);
        gradient8.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient8.setColorAt(0, QColor(0, 229, 255, 255));
        gradient8.setColorAt(1, QColor(0, 0, 0, 255));
        QBrush brush11(gradient8);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush11);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        lcdNumber->setPalette(palette);
        lcdNumber->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(cx: 0.5, cy: 0.5, radius: 0.7,\n"
"                                      fx: 0.5, fy: 0.5,\n"
"                                      stop: 0 #00E5FF, stop: 1 black);\n"
"    color: white; /* El texto blanco es fundamental aqu\303\255 */"));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(90, 0, 81, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(20);
        label->setFont(font);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(670, 40, 111, 51));
        QPalette palette1;
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush12(QColor(46, 204, 113, 255));
        brush12.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush12);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush12);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush12);
        QBrush brush13(QColor(31, 155, 93, 255));
        brush13.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush12);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush12);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush12);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush12);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush12);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush12);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        pushButton->setPalette(palette1);
        pushButton->setStyleSheet(QString::fromUtf8("#pushButton {\n"
"    background-color: #2ECC71; /* Verde esmeralda */\n"
"    color: white;\n"
"    border: 1px solid #27AE60;\n"
"    border-radius: 5px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"#pushButton:hover {\n"
"    background-color: #58D68D; /* Un verde m\303\241s brillante */\n"
"}\n"
"\n"
"#pushButton:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #27AE60, stop: 1 #229954);\n"
"    border-color: #1E8449;\n"
"}"));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(670, 110, 111, 51));
        QPalette palette2;
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush14(QColor(231, 76, 60, 255));
        brush14.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush14);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush14);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush14);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush14);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush14);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush14);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush14);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush14);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush14);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        pushButton_2->setPalette(palette2);
        pushButton_2->setStyleSheet(QString::fromUtf8("#pushButton_2 {\n"
"    background-color: #E74C3C; /* Rojo carmes\303\255 */\n"
"    color: white;\n"
"    border: 1px solid #C0392B;\n"
"    border-radius: 5px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"#pushButton_2:hover {\n"
"    background-color: #EC7063; /* Un rojo m\303\241s suave */\n"
"}\n"
"\n"
"#pushButton_2:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #C0392B, stop: 1 #A93226);\n"
"    border-color: #922B21;\n"
"}"));
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(10, 390, 131, 21));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(370, 130, 191, 31));
        QPalette palette3;
        QLinearGradient gradient9(0, 0, 1, 1);
        gradient9.setSpread(QGradient::Spread::PadSpread);
        gradient9.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient9.setColorAt(0, QColor(42, 245, 152, 255));
        gradient9.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush15(gradient9);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush15);
        QBrush brush16(QColor(92, 107, 127, 255));
        brush16.setStyle(Qt::BrushStyle::SolidPattern);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush16);
        QLinearGradient gradient10(0, 0, 1, 1);
        gradient10.setSpread(QGradient::Spread::PadSpread);
        gradient10.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient10.setColorAt(0, QColor(42, 245, 152, 255));
        gradient10.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush17(gradient10);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush17);
        QLinearGradient gradient11(0, 0, 1, 1);
        gradient11.setSpread(QGradient::Spread::PadSpread);
        gradient11.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient11.setColorAt(0, QColor(42, 245, 152, 255));
        gradient11.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush18(gradient11);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush18);
        QLinearGradient gradient12(0, 0, 1, 1);
        gradient12.setSpread(QGradient::Spread::PadSpread);
        gradient12.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient12.setColorAt(0, QColor(42, 245, 152, 255));
        gradient12.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush19(gradient12);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush19);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush16);
        QLinearGradient gradient13(0, 0, 1, 1);
        gradient13.setSpread(QGradient::Spread::PadSpread);
        gradient13.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient13.setColorAt(0, QColor(42, 245, 152, 255));
        gradient13.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush20(gradient13);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush20);
        QLinearGradient gradient14(0, 0, 1, 1);
        gradient14.setSpread(QGradient::Spread::PadSpread);
        gradient14.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient14.setColorAt(0, QColor(42, 245, 152, 255));
        gradient14.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush21(gradient14);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush21);
        QLinearGradient gradient15(0, 0, 1, 1);
        gradient15.setSpread(QGradient::Spread::PadSpread);
        gradient15.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient15.setColorAt(0, QColor(42, 245, 152, 255));
        gradient15.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush22(gradient15);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush22);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush16);
        QLinearGradient gradient16(0, 0, 1, 1);
        gradient16.setSpread(QGradient::Spread::PadSpread);
        gradient16.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient16.setColorAt(0, QColor(42, 245, 152, 255));
        gradient16.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush23(gradient16);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush23);
        QLinearGradient gradient17(0, 0, 1, 1);
        gradient17.setSpread(QGradient::Spread::PadSpread);
        gradient17.setCoordinateMode(QGradient::CoordinateMode::ObjectBoundingMode);
        gradient17.setColorAt(0, QColor(42, 245, 152, 255));
        gradient17.setColorAt(1, QColor(0, 158, 253, 255));
        QBrush brush24(gradient17);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush24);
        label_2->setPalette(palette3);
        label_2->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,\n"
"                                      stop: 0 #2AF598, stop: 1 #009EFD);\n"
"    /* Inicia en verde (#2AF598) en la esquina superior izquierda y termina en azul (#009EFD) en la inferior derecha */"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        bot_vel_subir = new QPushButton(centralwidget);
        bot_vel_subir->setObjectName("bot_vel_subir");
        bot_vel_subir->setGeometry(QRect(370, 40, 121, 51));
        bot_vel_subir->setStyleSheet(QString::fromUtf8("#bot_vel_subir {\n"
"    /* Estado normal: Naranja claro */\n"
"    background-color: rgb(255, 190, 100); /* Naranja claro */\n"
"    \n"
"    color: black; /* Texto negro para legibilidad */\n"
"    \n"
"    border: 1px solid #CC9030; /* Borde naranja un poco m\303\241s oscuro */\n"
"    border-radius: 5px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"#bot_vel_subir:hover {\n"
"    /* Estado hover: Un naranja ligeramente m\303\241s oscuro */\n"
"    background-color: rgb(255, 175, 70); \n"
"}\n"
"\n"
"#bot_vel_subir:pressed {\n"
"    /* Estado presionado: Gradiente de naranja oscuro */\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                     stop: 0 #FF8C00, stop: 1 #CC6D00);\n"
"    border-color: #A65A00; /* Borde a\303\272n m\303\241s oscuro */\n"
"}"));
        bot_vel_bajar = new QPushButton(centralwidget);
        bot_vel_bajar->setObjectName("bot_vel_bajar");
        bot_vel_bajar->setGeometry(QRect(500, 40, 111, 51));
        bot_vel_bajar->setStyleSheet(QString::fromUtf8("#bot_vel_bajar {\n"
"    /* Estado normal: Naranja claro */\n"
"    background-color: rgb(255, 190, 100); /* Naranja claro */\n"
"    \n"
"    color: black; /* Texto negro para legibilidad */\n"
"    \n"
"    border: 1px solid #CC9030; /* Borde naranja un poco m\303\241s oscuro */\n"
"    border-radius: 5px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"#bot_vel_bajar:hover {\n"
"    /* Estado hover: Un naranja ligeramente m\303\241s oscuro */\n"
"    background-color: rgb(255, 175, 70); \n"
"}\n"
"\n"
"#bot_vel_bajar:pressed {\n"
"    /* Estado presionado: Gradiente de naranja oscuro */\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                     stop: 0 #FF8C00, stop: 1 #CC6D00);\n"
"    border-color: #A65A00; /* Borde a\303\272n m\303\241s oscuro */\n"
"}"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(410, 0, 201, 31));
        label_3->setFont(font);
        indicatorLed = new QLabel(centralwidget);
        indicatorLed->setObjectName("indicatorLed");
        indicatorLed->setGeometry(QRect(10, 330, 40, 40));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(indicatorLed->sizePolicy().hasHeightForWidth());
        indicatorLed->setSizePolicy(sizePolicy);
        indicatorLed->setMinimumSize(QSize(20, 20));
        indicatorLed->setMaximumSize(QSize(40, 40));
        indicatorLed->setStyleSheet(QString::fromUtf8("    background-color: #333333;\n"
"    border-radius: 10px;\n"
"    min-width: 20px;\n"
"    min-height: 20px;"));
        te_sentido = new QTextEdit(centralwidget);
        te_sentido->setObjectName("te_sentido");
        te_sentido->setGeometry(QRect(370, 170, 191, 51));
        estado_motor = new QTextEdit(centralwidget);
        estado_motor->setObjectName("estado_motor");
        estado_motor->setGeometry(QRect(10, 260, 151, 51));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 230, 91, 16));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Times New Roman")});
        font1.setPointSize(14);
        label_4->setFont(font1);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(840, 0, 241, 31));
        label_5->setFont(font);
        te_temp = new QTextEdit(centralwidget);
        te_temp->setObjectName("te_temp");
        te_temp->setGeometry(QRect(880, 40, 121, 81));
        QFont font2;
        font2.setPointSize(24);
        te_temp->setFont(font2);
        bot_inv_giro = new QPushButton(centralwidget);
        bot_inv_giro->setObjectName("bot_inv_giro");
        bot_inv_giro->setGeometry(QRect(670, 190, 111, 71));
        QPalette palette4;
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush25(QColor(52, 122, 183, 255));
        brush25.setStyle(Qt::BrushStyle::SolidPattern);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush25);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush25);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush25);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush25);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush25);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush25);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush25);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush25);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush25);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, brush13);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush5);
#endif
        bot_inv_giro->setPalette(palette4);
        bot_inv_giro->setStyleSheet(QString::fromUtf8("#bot_inv_giro {\n"
"    background-color: #347AB7; /* Azul formal */\n"
"    color: white;\n"
"    border: 1px solid #2E6DA4; /* Azul oscuro (borde) */\n"
"    border-radius: 5px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"#bot_inv_giro:hover {\n"
"    background-color: #4A90D9; /* Azul m\303\241s claro */\n"
"}\n"
"\n"
"#bot_inv_giro:pressed {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #2E6DA4, stop: 1 #285E8E); /* Gradiente oscuro */\n"
"    border-color: #204D74; /* Borde m\303\241s oscuro */\n"
"}"));
        visorLog = new QPlainTextEdit(centralwidget);
        visorLog->setObjectName("visorLog");
        visorLog->setGeometry(QRect(370, 330, 411, 121));
        visorLog->setStyleSheet(QString::fromUtf8("QPlainTextEdit {\n"
"    border: 2px solid #555555;   /* Borde gris s\303\263lido de 2 pixeles */\n"
"    border-radius: 8px;          /* Esquinas redondeadas */\n"
"    background-color: #1e1e1e;   /* Fondo casi negro */\n"
"    color: #00ff00;              /* Texto verde tipo terminal (o usa #ffffff para blanco) */\n"
"    padding: 10px;               /* Espacio entre el texto y el borde */\n"
"    font-family: \"Consolas\", \"Courier New\", monospace; /* Fuente tipo c\303\263digo */\n"
"    selection-background-color: #444444; /* Color al seleccionar texto */\n"
"}"));
        on_btn_borrar_logs = new QPushButton(centralwidget);
        on_btn_borrar_logs->setObjectName("on_btn_borrar_logs");
        on_btn_borrar_logs->setGeometry(QRect(810, 350, 101, 51));
        on_btn_borrar_logs->setStyleSheet(QString::fromUtf8("QPushButton#on_btn_borrar_logs {\n"
"    background-color: #2c3e50;\n"
"    color: #ecf0f1;\n"
"    border: 2px solid #e74c3c; /* Borde rojo */\n"
"    border-radius: 4px;\n"
"    padding: 6px 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton#on_btn_borrar_logs:hover {\n"
"    background-color: #e74c3c; /* Al pasar el mouse se llena de rojo */\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton#on_btn_borrar_logs:pressed {\n"
"    background-color: #c0392b;\n"
"    border-color: #c0392b;\n"
"}"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1124, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "R.P.M", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "INICIAR", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "DETENER", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Sensado activo", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "SENTIDO DE GIRO", nullptr));
        bot_vel_subir->setText(QCoreApplication::translate("MainWindow", "AUMENTAR ", nullptr));
        bot_vel_bajar->setText(QCoreApplication::translate("MainWindow", "DISMINUIR", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "VELOCIDAD", nullptr));
        indicatorLed->setText(QString());
        estado_motor->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "ESTADO", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "TEMPERATURA", nullptr));
        te_temp->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:24pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        bot_inv_giro->setText(QCoreApplication::translate("MainWindow", "INVERTIR", nullptr));
        on_btn_borrar_logs->setText(QCoreApplication::translate("MainWindow", "RESET LOG", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
