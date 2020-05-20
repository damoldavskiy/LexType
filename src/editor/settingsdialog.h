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
    /**
     * @brief Constructs settings dialog
     * @param Parent widget
     */
    SettingsDialog(QWidget *parent = 0);

    /**
     * @brief Cleares form from control elements
     */
    void clearForm();

    /**
     * @brief Appends font selection buttons
     * @param Properties
     */
    void appendFontButtons(const Properties &pairs);

    /**
     * @brief Appends color selection buttons
     * @param Properties
     */
    void appendColorButtons(const Properties &pairs);

    /**
     * @brief Appends check boxes
     * @param Properties
     */
    void appendCheckBoxes(const Properties &pairs);

    /**
     * @brief Appends snippet list control
     */
    void appendSnippetsList();

private:
    QListWidget *_list;
    QFormLayout *_form;
    QHBoxLayout *_layout;
};

#endif // SETTINGSDIALOG_H
