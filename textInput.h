#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QApplication>
#include <QTextEdit>
#include <QKeyEvent>
#include <QClipboard>
#include <QMimeData>
#include <QBuffer>

class TextInput : public QTextEdit {
    Q_OBJECT

public:
    TextInput(QWidget *parent = nullptr) : QTextEdit(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->matches(QKeySequence::Paste)) {
            const QClipboard *clipboard = QApplication::clipboard();
            const QMimeData *mimeData = clipboard->mimeData();

            if (mimeData->hasImage()) {
                QImage image = qvariant_cast<QImage>(mimeData->imageData());
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");  // Możesz wybrać inny format zapisu obrazu
                QString imageBase64 = QString::fromLatin1(byteArray.toBase64().data());

                // Wstawienie obrazu jako HTML do QTextEdit
                QString html = QString("<img src='data:image/png;base64,") + imageBase64 + QString("'>");
                QTextCursor cursor = textCursor();
                cursor.insertHtml(html);
                setTextCursor(cursor);
            }
        } else if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && !event->modifiers().testFlag(Qt::ShiftModifier) && hasFocus()) {
            qDebug() << "Pressed";
            emit enterPressed();
            event->accept();
        } else {
            QTextEdit::keyPressEvent(event);
        }
    }


signals:
    void enterPressed();
};


#endif // TEXTINPUT_H
