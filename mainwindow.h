#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "anagramderivation.h"
namespace Ui {
class FindingWord;
}
QT_BEGIN_NAMESPACE
class QAction;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QFileDialog;
QT_END_NAMESPACE
class Anagrams : public QWidget
{
    Q_OBJECT

public:
    explicit Anagrams(QWidget *parent = nullptr);
    ~Anagrams();


 signals:
    void findAnagrams(QString word);

private slots:

    /*!
     * \brief on_searchButton_clicked - slot which is doing main functionality
     */
    void onSearchButtonClicked();

    void onLoadButtonClicked();
    void selectedFile(const QString& path);
    void onFindButtonClicked();
    void on_AcceptButton_clicked();
    void onCanceledButtonClicked();
    void setResults(const QString& result);

private:
    AnagramDerivation* dictionary;
    QThread *worker;
    QGroupBox *gridGroupBox;
    QTextEdit *bigEditor;
    QLabel *labelDictionary;
    QLabel *labelWord;
    QLabel *labelSearching;
    QLabel *labelStatus;
    QLineEdit *lineEditDictionary;
    QLineEdit *lineEditWord;
    QPushButton *buttonLoad;
    QPushButton *buttonSearch;
    QPushButton *buttonFind;
    QFileDialog* filedialog;
    void creatGui();
};

#endif // MAINWINDOW_H
