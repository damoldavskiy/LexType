#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QListWidget>

typedef QVector<QPair<QString, QString>> Properties;

/**
 * @brief Settings dialog for updating general application preferences
 */
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

private:
    QListWidget *_list;
    QFormLayout *_form;
    QHBoxLayout *_layout;
};

#endif // SETTINGSDIALOG_H
