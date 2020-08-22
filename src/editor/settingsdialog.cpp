#include "settingsdialog.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QFontDialog>
#include <QAction>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>

#include "snippetmanager.h"
#include "colorbutton.h"

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
    setMinimumHeight(600);
    setMaximumHeight(600);

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
    QListWidgetItem *painterItem = new QListWidgetItem("Painter");
    QListWidgetItem *viewerItem = new QListWidgetItem("Viewer");
    QListWidgetItem *snippetsItem = new QListWidgetItem("Snippets");
    QListWidgetItem *mathItem = new QListWidgetItem("Math mode");

    _list->addItem(generalItem);
    _list->addItem(editorItem);
    _list->addItem(painterItem);
    _list->addItem(viewerItem);
    _list->addItem(snippetsItem);
    _list->addItem(mathItem);

    connect(_list, &QListWidget::currentItemChanged, this, [this, generalItem, editorItem, painterItem, viewerItem, snippetsItem, mathItem] (QListWidgetItem *item) {
        clearForm();
        if (item == generalItem) {
            appendCheckBoxes(Properties {
                { "window-flag-askexit", "Ask on exit" }
            });

            appendButton("Reset to defaults", Styler::reset);
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
                { "editor-comment", "Editor comment" },
                { "editor-line", "Editor line" },
                { "editor-selection-back", "Editor selection background" },
                { "editor-selection-fore", "Editor selection text" },
                { "editor-error", "Editor error" },
                { "editor-caret", "Editor caret" },
                { "numbers-back", "Numbers background" },
                { "numbers-fore", "Numbers foreground" },
                { "numbers-current", "Numbers current" }
            });

            appendDoubleEdit({ "editor-caret-width", "Caret width (pixels)" }, [] (qreal n) { return n > 0 && n <= 5; });
            appendIntEdit({ "editor-tab-width", "Tab width (spaces)" }, [] (int n) { return n > 0; });
            appendIntEdit({ "editor-tick-time", "Cursor tick interval (ms)" }, [] (int n) { return n > 0; });
            appendIntEdit({ "editor-autocompile-interval", "Auto-compile interval (ms)" }, [] (int n) { return n >= 0; });

            appendCheckBoxes(Properties {
                { "editor-flag-line", "Highlight current line" },
                { "editor-flag-numbers", "Show line numbers" },
                { "editor-flag-autocompile", "Compile on type" },
                { "editor-flag-brackets", "Smart brackets" }
            });
        } else if (item == painterItem) {
            appendColorButtons(Properties {
                { "painter-back", "Painter background" },
                { "painter-fore", "Painter foreground" },
                { "painter-highlight", "Painter highlight" }
            });

            appendIntEdit({ "painter-attract-radius", "Attract radius (px)" }, [] (int n) { return n > 0; });
        } else if (item == viewerItem) {
            appendColorButtons(Properties {
                { "viewer-back", "Viewer background" },
                { "viewer-paper", "Document paper" },
                { "viewer-text", "Document text" }
            });

            appendDoubleEdit({ "viewer-page-shift", "Shift between pages (inches)" }, [] (qreal n) { return n >= 0; });
            appendDoubleEdit({ "viewer-zoom-factor", "Zoom factor" }, [] (qreal n) { return n > 1 && n <= 2; });

            appendIntEdit({ "viewer-dpi-default", "Default DPI" }, [] (int n) { return n > 0 && n <= 500; });
            appendIntEdit({ "viewer-dpi-min", "Minumum DPI" }, [] (int n) { return n > 0 && n <= 500; });
            appendIntEdit({ "viewer-dpi-max", "Maximum DPI" }, [] (int n) { return n > 0 && n <= 500; });

            appendDoubleEdit({ "viewer-resolution", "Render resolution" }, [] (qreal n) { return n > 0 && n <= 4; });
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
                { "math-flag-fractions", "New fraction style" },
                { "math-flag-matrices", "New matrix style" }
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
        ColorButton *button = new ColorButton;
        button->setMaximumWidth(200);
        button->setColor(Styler::get<QColor>(pair.first));
        connect(button, &ColorButton::selected, this, [pair] (QColor color) {
            Styler::set(pair.first, color);
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
    list->setStyleSheet(Styler::get<QString>("scroll-style"));
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
    QPushButton *remove = new QPushButton("Remove");
    QPushButton *insert = new QPushButton("Insert");

    QFormLayout *info = new QFormLayout;
    info->addRow("Math", math);
    info->addRow("Caret index", position);
    info->addRow("Pattern", pattern);
    info->addRow("Value", value);
    info->addWidget(accept);
    info->addWidget(remove);
    info->addWidget(insert);

    math->setEnabled(false);
    position->setEnabled(false);
    pattern->setEnabled(false);
    value->setEnabled(false);
    accept->setEnabled(false);
    remove->setEnabled(false);
    insert->setEnabled(false);

    // TODO Better to save indices of all snippets
    connect(list, &QListWidget::currentItemChanged, this, [list, math, position, pattern, value, accept, remove, insert] (QListWidgetItem *) {
        SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
        QVector<Snippet> snippets = manager.snippets();
        int index = list->currentRow();
        if (index == -1)
            return;
        Snippet &snippet = snippets[index];

        math->setEnabled(true);
        position->setEnabled(true);
        pattern->setEnabled(true);
        value->setEnabled(true);
        accept->setEnabled(true);
        remove->setEnabled(true);
        insert->setEnabled(true);

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

        if (pattern->toPlainText().size() == 0 || value->toPlainText().size() == 0)
            ok = false;

        if (!ok) {
            QMessageBox(QMessageBox::Warning, "Warning", "Incorrect snippet data").exec();
            return;
        }

        snippet.setRegular(!math->isChecked());
        snippet.setPosition(pos);
        snippet.setPattern(pattern->toPlainText());
        snippet.setValue(value->toPlainText());

        Styler::set<QVariant>("snippets", QVariant::fromValue(manager));
    });

    connect(remove, &QPushButton::clicked, this, [list, math, position, pattern, value, accept, remove, insert] () {
        SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
        QVector<Snippet> &snippets = manager.snippets();
        int index = list->currentRow();

        snippets.remove(index);
        Styler::set<QVariant>("snippets", QVariant::fromValue(manager));

        math->setEnabled(false);
        position->setEnabled(false);
        pattern->setEnabled(false);
        value->setEnabled(false);
        accept->setEnabled(false);
        remove->setEnabled(false);
        insert->setEnabled(false);

        list->clear();
        for (const Snippet &snippet : snippets)
            list->addItem(snippet.pattern().replace("\n", " "));
    });

    connect(insert, &QPushButton::clicked, this, [list] () {
        SnippetManager manager = Styler::get<QVariant>("snippets").value<SnippetManager>();
        QVector<Snippet> &snippets = manager.snippets();
        int index = list->currentRow();

        snippets.insert(index, Snippet(true, "Pattern", "Value"));
        Styler::set<QVariant>("snippets", QVariant::fromValue(manager));

        list->clear();
        for (const Snippet &snippet : snippets)
            list->addItem(snippet.pattern().replace("\n", " "));

        list->setCurrentRow(index);
    });

    layout->addWidget(list, 0, Qt::AlignLeft);
    layout->addLayout(info);

    box->setLayout(layout);

    _form->addRow(box);
}
