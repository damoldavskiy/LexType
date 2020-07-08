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
        connect(button, &QPushButton::clicked, this, handler);
        _form->addRow(button);
    }

    template <typename T>
    void appendNumberEditInt(const Property &prop, T checker)
    {
        QLineEdit *edit = new QLineEdit;
        edit->setText(QString::number(Styler::get<int>(prop.first)));
        edit->setMaximumWidth(200);
        connect(edit, &QLineEdit::textChanged, this, [edit, checker, prop] () {
            QString text = edit->text();
            bool ok;
            int n = text.toInt(&ok);
            if (ok && checker(n)) {
                edit->setStyleSheet("QLineEdit { background: rgb(50, 50, 50); }");
                Styler::set(prop.first, n);
            }
            else
                edit->setStyleSheet("QLineEdit { background: rgb(150, 30, 30); }");
        });
        _form->addRow(prop.second, edit);
    }

private:
    QListWidget *_list;
    QFormLayout *_form;
    QHBoxLayout *_layout;
};

#endif // SETTINGSDIALOG_H
