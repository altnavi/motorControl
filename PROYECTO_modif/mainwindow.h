#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QJsonObject.h>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool check);

    void on_pushButton_2_clicked(bool check);

    void ledOn();
    void ledOff();

    void conectarServidor();
    void enviarMensaje();
    void enviarEstadoBoton(bool checked);
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void on_bot_vel_subir_clicked();

    void on_bot_vel_bajar_clicked();

    void on_bot_inv_giro_clicked(bool checked);

    void registrarLog(const QString &tipo, const QString &mensaje, const QJsonObject &datos = QJsonObject());

    void on_on_btn_borrar_logs_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    bool alarmaActiva;
};
#endif // MAINWINDOW_H
