#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void enviarEstadoBoton(bool checked);   // ⬅️ nuevo
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void on_bot_vel_subir_clicked();

    void on_bot_vel_bajar_clicked();

    void on_bot_inv_giro_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket; // <-- 2. Declarar el objeto socket
};
#endif // MAINWINDOW_H
