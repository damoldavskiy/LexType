#include "settingsdialog.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFontDialog>
#include <QColorDialog>
#include <QAction>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QMessageBox>

#include "styler.h"
#include "snippetmanager.h"

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
    setMaximumWidth(700);
    setMinimumHeight(500);
    setMaximumHeight(500);

    _list = new QListWidget;
    _form = new QFormLayout;
    _layout = new QHBoxLayout;

    _list->setMinimumWidth(200);
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
                { "editor-flag-snippets-regular", "Enable regular snippets in editor" },
                { "editor-flag-snippets-math", "Enable math snippets in editor" },
                { "painter-flag-snippets", "Enable snippets in painter (math only)" }
            });
            appendSnippetsList();
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
            QColor color = QColorDialog::getColor(Styler::get<QColor>(pair.first));
            if (color.isValid()) {
                Styler::set(pair.first, color);
                button->setText(Styler::get<QColor>(pair.first).name());
            }
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

void SettingsDialog::appendSnippetsList()
{
    QGroupBox *box = new QGroupBox("Snippets list");

    QHBoxLayout *layout = new QHBoxLayout;

    QListWidget *list = new QListWidget;
    list->setMinimumWidth(150);
    list->setMaximumWidth(150);

    SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
    QVector<Snippet> snippets = manager.snippets();

    for (const Snippet &snippet : snippets)
        list->addItem(snippet.pattern().replace("\n", " "));

    QCheckBox *math = new QCheckBox;
    QLineEdit *position = new QLineEdit;
    QTextEdit *pattern = new QTextEdit;
    QTextEdit *value = new QTextEdit;
    QPushButton *accept = new QPushButton("Accept");

    QFormLayout *info = new QFormLayout;
    info->addRow("Math", math);
    info->addRow("Caret index", position);
    info->addRow("Pattern", pattern);
    info->addRow("Value", value);
    info->addWidget(accept);

    // TODO Better to save indices of all snippets
    connect(list, &QListWidget::currentItemChanged, this, [list, info, layout, math, position, pattern, value] (QListWidgetItem *) {
        SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
        QVector<Snippet> snippets = manager.snippets();
        int index = list->currentRow();
        Snippet &snippet = snippets[index];

        if (info->parent() == nullptr)
            layout->addLayout(info);

        math->setChecked(!snippet.regular());
        position->setText(QString::number(snippet.position()));
        pattern->setText(snippet.pattern());
        value->setText(snippet.value());
    });

    connect(accept, &QPushButton::clicked, this, [list, math, position, pattern, value] () {
        SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
        QVector<Snippet> &snippets = manager.snippets();
        int index = list->currentRow();
        Snippet &snippet = snippets[index];

        bool ok;
        int pos = position->text().toInt(&ok);
        if (pos < 0 || pos > value->toPlainText().size())
            ok = false;

        if (!ok) {
            QMessageBox(QMessageBox::Warning, "Warning", "Incorrect final caret index").exec();
            return;
        }

        snippet.setRegular(!math->isChecked());
        snippet.setPosition(pos);
        snippet.setPattern(pattern->toPlainText());
        snippet.setValue(value->toPlainText());

        Styler::set<QVariant>("snippets", QVariant::fromValue(manager));
    });

    layout->addWidget(list, 0, Qt::AlignLeft);

    box->setLayout(layout);

    _form->addRow(box);
}
