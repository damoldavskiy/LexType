#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

#include "styler.h"

typedef QPair<QString, QString> Property;
typedef QVector<Property> Properties;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0);

    void clearForm();
    void appendFontButtons(const Properties &pairs);
    void appendColorButtons(const Properties &pairs);
    void appendCheckBoxes(const Properties &pairs);
    void appendSnippetsList();
    void appendResetSettings();

    template <typename T>
    void appendButton(const QString &title, T handler)
    {
        QPushButton *button = new QPushButton(title);
        button->setMaximumWidth(200);
        connect(button, &QPushButton::clicked, this, handler);
        _form->addRow(button);
    }

    template <typename T, typename U, typename V>
    void appendTextEdit(const Property &prop, U converter, V checker)
    {
        QLineEdit *edit = new QLineEdit;
        edit->setText(QString::number(Styler::get<T>(prop.first)));
        edit->setMaximumWidth(200);
        connect(edit, &QLineEdit::textChanged, this, [edit, converter, checker, prop] () {
            QString text = edit->text();
            if (checker(text)) {
                edit->setStyleSheet("QLineEdit { background: rgb(50, 50, 50); }");
                Styler::set(prop.first, converter(text));
            }
            else
                edit->setStyleSheet("QLineEdit { background: rgb(150, 30, 30); }");
        });
        _form->addRow(prop.second, edit);
    }

    template <typename T>
    void appendIntEdit(const Property &prop, T checker)
    {
        appendTextEdit<int>(prop, [] (QString text) {
            return text.toInt();
        }, [checker] (QString text) {
            bool ok;
            int n = text.toInt(&ok);
            return ok && checker(n);
        });
    }

    template <typename T>
    void appendDoubleEdit(const Property &prop, T checker)
    {
        appendTextEdit<qreal>(prop, [] (QString text) {
            return text.toDouble();
        }, [checker] (QString text) {
            bool ok;
            qreal n = text.toDouble(&ok);
            return ok && checker(n);
        });
    }

private:
    QListWidget *_list;
    QFormLayout *_form;
    QHBoxLayout *_layout;
};

#endif // SETTINGSDIALOG_H
