#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
// #include <opus/opus.h>
#include <QAudioSource>

#include "textInput.h"

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
    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *chatSocket;
    // QUdpSocket *voiceSocket;

    bool connect = true;

    // OpusEncoder *encoder;
    // OpusDecoder *decoder;

    // QAudioInput *audioInput;
    // QAudioOutput *audioOutput;
    // QAudioFormat *format;
};
#endif // MAINWINDOW_H
