#include "settingsdialog.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFontDialog>
#include <QColorDialog>
#include <QAction>

#include "styler.h"

QString fontToString(const QFont &font)
{
    return font.family() + " " + QString::number(font.pointSize());
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setStyleSheet(Styler::get<QString>("widget-style"));

    setMinimumWidth(700);
    setMinimumHeight(500);

    _list = new QListWidget;
    _form = new QFormLayout;
    _layout = new QHBoxLayout;

    _list->setMaximumWidth(200);

    _form->setMargin(20);

    _layout->addWidget(_list);
    _layout->addLayout(_form);
    setLayout(_layout);

    QListWidgetItem *generalItem = new QListWidgetItem("General");
    QListWidgetItem *editorItem = new QListWidgetItem("Editor");
    QListWidgetItem *snippetsItem = new QListWidgetItem("Snippets");
    QListWidgetItem *mathItem = new QListWidgetItem("Math mode");

    _list->addItem(generalItem);
    _list->addItem(editorItem);
    _list->addItem(snippetsItem);
    _list->addItem(mathItem);

    connect(_list, &QListWidget::currentItemChanged, this, [this, generalItem, editorItem, snippetsItem, mathItem] (QListWidgetItem *item) {
        clearForm();
        if (item == generalItem) {
            appendCheckBoxes(Properties {
                { "window-flag-askexit", "Ask on exit" }
            });
        } else if (item == editorItem) {
            appendFontButtons(Properties {
                { "editor-font", "Editor font" }
            });

            appendColorButtons(Properties {
                { "editor-back", "Editor background" },
                { "editor-regular", "Editor regular" },
                { "editor-mathematics", "Editor mathematics" },
                { "editor-command", "Editor command" },
                { "editor-special", "Editor special" },
                { "editor-line", "Editor line" },
                { "editor-selection", "Editor selection" },
                { "editor-caret", "Editor caret" },
                { "numbers-back", "Numbers background" },
                { "numbers-fore", "Numbers foreground" },
                { "numbers-current", "Numbers current" },
            });

            appendCheckBoxes(Properties {
                { "editor-flag-line", "Highlight current line" },
                { "editor-flag-numbers", "Show line numbers" },
                { "editor-flag-autocompile", "Compile on type" },
            });
        } else if (item == snippetsItem) {
            appendCheckBoxes(Properties {
                { "editor-flag-snippets", "Enable snippets" }
            });
        } else if (item == mathItem) {
            appendCheckBoxes(Properties {
                { "editor-flag-keyboard", "Inverse keyboard layout in math mode" },
            });
        }
    });
}

void SettingsDialog::clearForm()
{
    while (!_form->isEmpty())
        _form->removeRow(0);
}

void SettingsDialog::appendFontButtons(const Properties &pairs)
{
    for (const auto &pair : pairs) {
        QPushButton *button = new QPushButton(fontToString(Styler::get<QFont>(pair.first)));
        button->setMaximumWidth(200);
        connect(button, &QPushButton::pressed, this, [button, pair] () {
            Styler::set(pair.first, QFontDialog::getFont(nullptr, Styler::get<QFont>(pair.first)));
            button->setText(fontToString(Styler::get<QFont>(pair.first)));
        });
        _form->addRow(pair.second, button);
    }
}

void SettingsDialog::appendColorButtons(const Properties &pairs)
{
    for (const auto &pair : pairs) {
        QPushButton *button = new QPushButton(Styler::get<QColor>(pair.first).name());
        button->setMaximumWidth(200);
        connect(button, &QPushButton::pressed, this, [button, pair] () {
            Styler::set(pair.first, QColorDialog::getColor(Styler::get<QColor>(pair.first)));
            button->setText(Styler::get<QColor>(pair.first).name());
        });
        _form->addRow(pair.second, button);
    }
}

void SettingsDialog::appendCheckBoxes(const Properties &pairs)
{
    for (const auto &pair : pairs) {
        QCheckBox *box = new QCheckBox(pair.second);
        box->setChecked(Styler::get<bool>(pair.first));
        connect(box, &QCheckBox::toggled, this, [pair] (bool checked) {
            Styler::set(pair.first, checked);
        });
        _form->addRow(box);
    }
}

