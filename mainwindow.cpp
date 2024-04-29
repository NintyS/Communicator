#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  client = new QTcpSocket();

  QObject::connect(client, &QTcpSocket::connected, [&] {
    client->write(
        "{"
        " \"Nick\": \"" +
        ui->plainTextEdit->toPlainText().toUtf8() +
        "\""
        "}");
    client->flush();
    ui->stackedWidget->setCurrentIndex(1);
  });

  QObject::connect(client, &QTcpSocket::disconnected,
                   [&] { ui->stackedWidget->setCurrentIndex(0); });

  QObject::connect(client, &QTcpSocket::readyRead, [&] {
      QByteArray messageByte = client->readAll();
      client->flush();
      QString message = messageByte;
      qDebug() << "Message: " + message;

      QJsonParseError JsonError;
      QJsonDocument Document = QJsonDocument::fromJson(messageByte, &JsonError);
      if (JsonError.error != QJsonParseError::NoError) {
        qDebug() << "Error In Json Data : " << JsonError.errorString();
        return;
      }

      if (Document.isObject()) {
        QJsonObject Obj = Document.object();

        QJsonArray users = Obj.value("Users").toArray();
        QString message = Obj.value("Message").toString();

        qDebug() << Obj;

        if (!users.isEmpty()) {
          qDebug() << "Przydzielanie aktywnych userów!";

          for (auto user : users) {
            QJsonObject ObjUsers = user.toObject();
            ui->listWidget_3->addItem(ObjUsers.value("Nick").toString());
          }
        }

        if (message != "") {
          qDebug() << "Doszła wiadomość!";
          ui->listWidget_2->addItem(Obj.value("Nick").toString() + ": " +
                                    Obj.value("Message").toString() + " - " +
                                    Obj.value("Date").toString());
        }
    }
  });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_5_clicked() {
  ui->plainTextEdit_2->toPlainText();  // IP

  client->connectToHost(ui->plainTextEdit_2->toPlainText(), 8080);
  client->flush();
}

void MainWindow::on_pushButton_2_clicked() {
  // Disconect TCP connection
  client->flush();
  client->disconnectFromHost();

  ui->listWidget_2->clear();
  ui->listWidget_3->clear();
}

void MainWindow::on_pushButton_6_clicked() {
  client->write(
      "{"
      " \"Message\": \"" +
      ui->textEdit->toPlainText().toUtf8() +
      "\""
      "}");
  client->flush();
  ui->textEdit->setText("");
}
