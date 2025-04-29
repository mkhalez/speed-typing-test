#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <QTimer>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private:
    Ui::MainWindow* ui;
    QMap<int, QPushButton*> english_map;
    QSet<QPushButton*> pressed_buttons;
    QVector<QString> english;
    int status;
    QString text;
    QString colored_text = "";
    QString start_text;
    int current_symbol;
    void updateLabelColoring(int index, int status);
    QTimer* timer;
    int second;
    int minute;
    int mili_second;
    double frequency;
    int number_of_words;
    int index_without_error_in_space;
    int right_symbol;

    int NumberOfWords(QString text);

    QString loadTextFromFile(const QString& filePath);
    bool isFileDialogOpen = false;


    struct LanguageLayout {
        QMap<int, QPushButton*> keyMap;
        QVector<QString> characters;
    };

    LanguageLayout englishLayout;
    LanguageLayout russianLayout;
    LanguageLayout germanLayout;
    LanguageLayout frenchLayout;
    LanguageLayout arabicLayout;
    LanguageLayout chineseLayout;
    LanguageLayout belarusianLayout;
    LanguageLayout hebrewLayout;
    LanguageLayout* currentLayout;

    void initEnglishLayout();
    void initRussianLayout();
    void initGermanLayout();
    void initFrenchLayout();
    void initArabicLayout();
    void initChineseLayout();
    void initBelarusianLayout();
    void initHebrewLayout();
    void updateKeyboardLayout();

   protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
   private slots:
    void TimerSlot();
    void onSelectFileClicked();
    void onLanguageChanged();
};
#endif	// MAINWINDOW_H
