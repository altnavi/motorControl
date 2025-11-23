#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStyle>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

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

    conectarServidor();

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile archivo(path + "/log_motor.txt");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        ui->visorLog->setPlainText(in.readAll());
        archivo.close();

        // Mover el scroll al final
        ui->visorLog->moveCursor(QTextCursor::End);
    }

    alarmaActiva = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conectarServidor()
{
    QString ip = "192.168.1.38";
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
    registrarLog("CONEXION", "Conexión exitosa al servidor.");
    enviarMensaje(); // enviar al conectarse
}

void MainWindow::onDisconnected()
{
    qDebug() << "⚠️ Desconectado del servidor.";
    registrarLog("DESCONEXION", "Se ha perdido la conexión con el servidor.");
}

void MainWindow::onReadyRead()
{
    QByteArray datos = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(datos);
    if (doc.isNull() || !doc.isObject()) {
        //qDebug() << "Error: No se recibió un JSON válido.";
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
        bool hayFallaActualmente = objetoJson["alarma"].toBool();

        if(hayFallaActualmente)
        {
            ledOn();
            ui->estado_motor->setText("FALLA DETECTADA");

            if (!alarmaActiva) {
                registrarLog("ALARMA_MOTOR", "Falla detectada por el microcontrolador.");
                alarmaActiva = true;
                qDebug() << "⚠️ Alarma registrada (Flag activado)";
            }
        }
        else
        {
            ledOff();
            ui->estado_motor->setText("NORMAL");

            if (alarmaActiva) {
                registrarLog("INFO_MOTOR", "La falla se ha solucionado. Sistema normal.");
                alarmaActiva = false;
                qDebug() << "✅ Alarma limpiada (Flag reseteado)";
            }
        }
    }
}

void MainWindow::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << "❌ Error de socket:" << socket->errorString();
    registrarLog("ERROR_SOCKET", socket->errorString());
}

void MainWindow::on_pushButton_clicked(bool check)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "No conectado, no se puede enviar.";
        return;
    }

    QByteArray data;
    data.append("motor_ON");
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

void MainWindow::registrarLog(const QString &tipo, const QString &mensaje, const QJsonObject &datos)
{
    // --- PARTE 1: Preparar el texto ---
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString registro = QString("%1 - [%2]: %3").arg(timestamp, tipo, mensaje);

    if (!datos.isEmpty()) {
        QJsonDocument doc(datos);
        registro += " | JSON: " + doc.toJson(QJsonDocument::Compact);
    }

    // --- PARTE 2: Mostrar en la Interfaz (NUEVO) ---
    // appendPlainText agrega una nueva línea al final automáticamente
    ui->visorLog->appendPlainText(registro);

    // --- PARTE 3: Guardar en Archivo (TU CÓDIGO ANTERIOR) ---
    // Nota: Usamos QStandardPaths para asegurar que funcione en cualquier PC
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");

    QFile archivo(dir.filePath("log_motor.txt")); // O usa tu ruta relativa si ya te funciona

    if (archivo.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&archivo);
        out << registro << "\n";
        archivo.close();
    } else {
        qDebug() << "Error escribiendo en archivo:" << archivo.errorString();
    }
}

void MainWindow::on_on_btn_borrar_logs_clicked()
{
    // 1. Preguntar confirmación para seguridad
    QMessageBox::StandardButton respuesta;
    respuesta = QMessageBox::question(this, "Borrar Logs",
                                      "¿Estás seguro de que quieres borrar TODO el historial de eventos?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::No) {
        return; // Si dice que no, no hacemos nada
    }

    // 2. Limpiar la pantalla (el visor en la GUI)
    ui->visorLog->clear();

    // 3. Limpiar el archivo de texto (Borrando su contenido)
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    QString filePath = dir.filePath("log_motor.txt");

    QFile archivo(filePath);

    // Al abrir con WriteOnly | Truncate, el archivo se vacía automáticamente (tamaño 0)
    if (archivo.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        archivo.close();
        qDebug() << "Archivo de log vaciado correctamente.";

        // (Opcional) Agregar un mensaje indicando que se limpió
        // registrarLog("SISTEMA", "El usuario ha borrado el historial de logs.", QJsonObject());
    } else {
        QMessageBox::warning(this, "Error", "No se pudo borrar el archivo de logs:\n" + archivo.errorString());
    }
}

