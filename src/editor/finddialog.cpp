#include "finddialog.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>

#include "styler.h"

FindDialog::FindDialog(const QString &text, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Find & Replace");
    setStyleSheet(Styler::get<QString>("widget-style"));
    setMaximumHeight(0);

    _findText = new QLineEdit(text);
    _replaceText = new QLineEdit;
    _case = new QCheckBox("Match case");
    _find = new QPushButton("Find");
    _replace = new QPushButton("Replace");
    _replaceAll = new QPushButton("Replace all");

    connect(_find, &QPushButton::clicked, this, [this] () {
        emit find(_findText->text(), _case->isChecked());
    });

    connect(_replace, &QPushButton::clicked, this, [this] () {
        emit replace(_findText->text(), _replaceText->text(), false, _case->isChecked());
    });

    connect(_replaceAll, &QPushButton::clicked, this, [this] () {
        emit replace(_findText->text(), _replaceText->text(), true, _case->isChecked());
    });

    QFormLayout *fields = new QFormLayout;
    fields->setSpacing(4);
    fields->addRow("Find:", _findText);
    fields->addRow("Replacer:", _replaceText);

    QGridLayout *options = new QGridLayout;
    options->addWidget(_case);

    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addWidget(_find);
    buttons->addWidget(_replace);
    buttons->addWidget(_replaceAll);

    QGridLayout *layout = new QGridLayout;
    layout->addItem(fields, 0, 0);
    layout->addItem(options, 1, 0);
    layout->addItem(buttons, 0, 1, 2);
    setLayout(layout);
}
