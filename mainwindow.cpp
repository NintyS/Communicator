#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  chatSocket = new QTcpSocket();

  QObject::connect(chatSocket, &QTcpSocket::connected, [&] {
    chatSocket->write(
        "{"
        " \"Nick\": \"" +
        ui->plainTextEdit->toPlainText().toUtf8() +
        "\""
        "}");
    chatSocket->flush();
    ui->stackedWidget->setCurrentIndex(1);
  });

  QObject::connect(chatSocket, &QTcpSocket::disconnected,
                   [&] { ui->stackedWidget->setCurrentIndex(0); });

  QObject::connect(chatSocket, &QTcpSocket::readyRead, [&] {
    QByteArray messageByte = chatSocket->readAll();
    chatSocket->flush();
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
        ui->listWidget_3->clear();

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

  // QAudioInput *audioInput = new QAudioInput();
  // audioInput->

  // ui->textEdit->installEventFilter(eventFilter);

  // int error;
  // encoder = opus_encoder_create(48000, 2, OPUS_APPLICATION_VOIP, &error);
  // decoder = opus_decoder_create(48000, 2, &error);

  // QObject::connect(voiceSocket, &QUdpSocket::readyRead, [&] {
  //     while (voiceSocket->hasPendingDatagrams()) {
  //         QByteArray datagram;
  //         datagram.resize(voiceSocket->pendingDatagramSize());
  //         QHostAddress sender;
  //         quint16 senderPort;

  //         voiceSocket->readDatagram(datagram.data(), datagram.size(),
  //                                   &sender, &senderPort);

  //         int frame_size = 960;
  //         opus_int16 decoded_data[48000*2];
  //         int decoded_samples = opus_decode(decoder, (const unsigned
  //         char*)datagram.data(), datagram.size(), decoded_data, frame_size,
  //         0); if (decoded_samples > 0) {
  //             QByteArray data((const char*)decoded_data, decoded_samples *
  //             sizeof(opus_int16)); audioOutput->start()->write(data);
  //     }
  // });

  // QAudioSource *audioSource = new QAudioSource();
  // QIODevice *inputDevice = audioSource->start();
  //   connect(inputDevice, &QIODevice::readyRead, [&] {
  //       QByteArray data = inputDevice->readAll();
  //       // Encode data...
  //       int frame_size = 960;
  //       unsigned char encoded_data[4000];
  //       int encoded_bytes = opus_encode(encoder, (const
  //       opus_int16*)data.data(), frame_size, encoded_data, 4000); if
  //       (encoded_bytes > 0) {
  //         voiceSocket->writeDatagram((const char*)encoded_data,
  //         encoded_bytes, QHostAddress::LocalHost, 8081);
  //       }
  //   });
}

MainWindow::~MainWindow() { /* opus_encoder_destroy(encoder);
                               opus_decoder_destroy(decoder); */
  delete chatSocket;        /* delete voiceSocket; delete audioInput; delete
                               audioOutput; delete format; */
  delete ui;
}

void MainWindow::on_pushButton_5_clicked() {
  ui->plainTextEdit_2->toPlainText();  // IP

  chatSocket->connectToHost(ui->plainTextEdit_2->toPlainText(), 8080);
  // voiceSocket->connectToHost(ui->plainTextEdit_2->toPlainText(), 8081);
  chatSocket->flush();
}

void MainWindow::on_pushButton_2_clicked() {
  // Disconect TCP connection
  chatSocket->flush();
  chatSocket->disconnectFromHost();

  // voiceSocket->flush();
  // voiceSocket->disconnectFromHost();

  ui->listWidget_2->clear();
  ui->listWidget_3->clear();
}

void MainWindow::on_pushButton_6_clicked() {
  chatSocket->write(
      "{"
      " \"Message\": \"" +
      ui->textEdit->toPlainText().toUtf8() +
      "\""
      "}");
  chatSocket->flush();
  ui->textEdit->setText("");
}

void MainWindow::on_pushButton_3_clicked() {
  if (ui->pushButton_3->text() == "Mute") {
    ui->pushButton_3->setText("Unmute");
    // audioInput->setMuted(true);
  }

  if (ui->pushButton_3->text() == "Unmute") {
    ui->pushButton_3->setText("Mute");
    // audioInput->setMuted(false);
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (((event->key() == Qt::Key_Return) &&
       ui->stackedWidget->currentIndex() == 1) &&
      event->key() != Qt::Key_Shift) {
    chatSocket->write(
        "{"
        " \"Message\": \"" +
        ui->textEdit->toPlainText().toUtf8() +
        "\""
        "}");
    chatSocket->flush();
    ui->textEdit->setText("");
  }
}

// bool MainWindow::eventFilter(QObject *obj, QEvent *event)
// {
//     if (obj == ui->textEdit )
//     {
//         if (event->type() == QEvent::Enter)
//         {
//             chatSocket->write(
//                 "{"
//                 " \"Message\": \"" +
//                 ui->textEdit->toPlainText().toUtf8() +
//                 "\""
//                 "}");
//             chatSocket->flush();
//             ui->textEdit->setText("");
//         }

//     }
// }
