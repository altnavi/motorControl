#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStyle>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    // ESTABLECE EL ESTADO INICIAL DEL INDICADOR
    ui->indicatorLed->setStyleSheet("background-color: #333333; border-radius: 10px; min-width: 20px; min-height: 20px;");

    // Conectar señales del socket
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MainWindow::onError);

    // Ejemplo: conectarse automáticamente
    conectarServidor();
    // Conectamos los botones
    //connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ledOn);
    //connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::ledOff);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conectarServidor()
{
    QString ip = "10.189.78.48";
    quint16 port = 5000;

    qDebug() << "Conectando a" << ip << ":" << port << "...";
    socket->connectToHost(ip, port);
}

void MainWindow::enviarMensaje()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray mensaje = "Hola desde Qt!\r\n";
        socket->write(mensaje);
        qDebug() << "Enviado:" << mensaje;
    } else {
        qDebug() << "No conectado al servidor.";
    }
}

// =======================
// Slots de eventos
// =======================

void MainWindow::enviarEstadoBoton(bool checked)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("LED_ON");
    data.append(checked ? "1\r\n" : "0\r\n");
    socket->write(data);

    qDebug() << "➡️ Enviado estado del botón:" << (checked ? "1" : "0");
}

void MainWindow::onConnected()
{
    qDebug() << "✅ Conectado al servidor!";
    enviarMensaje(); // enviar al conectarse
}

void MainWindow::onDisconnected()
{
    qDebug() << "⚠️ Desconectado del servidor.";
}

void MainWindow::onReadyRead()
{
    // 1. Leer todos los datos que llegaron al buffer del socket
    QByteArray datos = socket->readAll();

    // Opcional: Mostrar los datos crudos en la consola de debug de Qt
    qDebug() << "Datos crudos recibidos:" << datos;

    // 2. Parsear el texto como un documento JSON
    QJsonDocument doc = QJsonDocument::fromJson(datos);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Error: No se recibió un JSON válido.";
        return;
    }

    // 3. Convertir el documento a un objeto JSON para poder acceder a sus claves
    QJsonObject objetoJson = doc.object();

    // 4. Extraer los valores usando las claves que definiste en el microcontrolador
    if (objetoJson.contains("rpm") && objetoJson["rpm"].isDouble()) {
        int rpm = objetoJson["rpm"].toInt();

        // 5. Actualizar la interfaz gráfica
        ui->lcdNumber->display(rpm);
    }

    if (objetoJson.contains("temp") && objetoJson["temp"].isDouble()) {
        double temperatura = objetoJson["temp"].toDouble();

        // Convertir a QString con 1 decimal y añadir °C
        QString temp_s = QString::number(temperatura, 'f', 1) + " °C";

        ui->te_temp->setText(temp_s);
    }


    if (objetoJson.contains("estado") && objetoJson["estado"].isBool()) {
        bool estado = objetoJson["estado"].toBool();
        qDebug() << "Estado:" << estado;
    }

    if (objetoJson.contains("sentido") && objetoJson["sentido"].isDouble()) {
        int estado = objetoJson["sentido"].toDouble();
        qDebug() << "sentido:" << estado;
        if(estado == 0)
        ui->te_sentido->setText("HORARIO");
        else if(estado == 1)
        ui->te_sentido->setText("ANTIHORARIO");
        else if(estado == 2)
        ui->te_sentido->setText("DETENIDO");
    }

    if (objetoJson.contains("alarma") && objetoJson["alarma"].isBool()) {
        bool estado = objetoJson["alarma"].toBool();
        qDebug() << "alarma:" << estado;
        if(estado)
        {
            ledOn();
            ui->estado_motor->setText("FALLA DETECTADA");
        }
        else
        {
            ledOff();
            ui->estado_motor->setText("NORMAL");
        }
    }
}

void MainWindow::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << "❌ Error de socket:" << socket->errorString();
}

void MainWindow::on_pushButton_clicked(bool check)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("motor_ON");
    //data.append("RPM=120\r\n");
    socket->write(data);

    qDebug() << "➡️ Enviado estado del botón:" << (check ? "1" : "0");
}


void MainWindow::on_pushButton_2_clicked(bool check)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("motor_OFF");
    socket->write(data);

    qDebug() << "➡️ Enviado estado del botón:" << (check ? "1" : "0");
}

void MainWindow::ledOn()
{
    ui->indicatorLed->setStyleSheet("background-color: #ff0000; border-radius: 10px; min-width: 20px; min-height: 20px;");
}


void MainWindow::ledOff()
{
    ui->indicatorLed->setStyleSheet("background-color: #333333; border-radius: 10px; min-width: 20px; min-height: 20px;");
}

void MainWindow::on_bot_vel_subir_clicked()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("aumentar");
    socket->write(data);

    //qDebug() << "➡️ Enviado estado del botón:" << (check ? "1" : "0");
}


void MainWindow::on_bot_vel_bajar_clicked()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("disminuir");
    socket->write(data);

}


void MainWindow::on_bot_inv_giro_clicked(bool checked)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("invertir");
    socket->write(data);
}

