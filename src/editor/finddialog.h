#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

/**
 * @brief Find and replace dialog
 */
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);

signals:
    void find(const QString &substring, bool matchCase);
    void replace(const QString &before, const QString &after, bool all, bool matchCase);

private:
    QLineEdit *_findText;
    QLineEdit *_replaceText;
    QCheckBox *_case;
    QPushButton *_find;
    QPushButton *_replace;
    QPushButton *_replaceAll;
};

#endif // FINDDIALOG_H
