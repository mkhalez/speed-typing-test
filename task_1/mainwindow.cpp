#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setFocus();
    start_text =
        loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/english_1.txt");
    text = start_text;
    current_symbol = 0;
    ui->label->setText(text);
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignCenter);

    second = 0;
    minute = 0;
    mili_second = 0;
    frequency = 0;
    number_of_words = 0;
    index_without_error_in_space = 0;
    right_symbol = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerSlot);
    connect(ui->openButton, &QPushButton::clicked, this,
            &MainWindow::onSelectFileClicked);
    connect(ui->languageBox, &QComboBox::currentIndexChanged, this,
            &MainWindow::onLanguageChanged);

    isFileDialogOpen = false;

    initEnglishLayout();
    currentLayout = &englishLayout;
    updateKeyboardLayout();


    /*ui->pushButton1->setFocusPolicy(Qt::StrongFocus);
    english_map[32] = ui->pushButton34;
    english_map[39] = ui->pushButton23;
    english_map[44] = ui->pushButton31;
    english_map[46] = ui->pushButton32;
    english_map[47] = ui->pushButton33;
    english_map[59] = ui->pushButton22;
    english_map[65] = ui->pushButton13;
    english_map[66] = ui->pushButton28;
    english_map[67] = ui->pushButton26;
    english_map[68] = ui->pushButton15;
    english_map[69] = ui->pushButton3;
    english_map[70] = ui->pushButton16;
    english_map[71] = ui->pushButton17;
    english_map[72] = ui->pushButton18;
    english_map[73] = ui->pushButton8;
    english_map[74] = ui->pushButton19;
    english_map[75] = ui->pushButton20;
    english_map[76] = ui->pushButton21;
    english_map[77] = ui->pushButton30;
    english_map[78] = ui->pushButton29;
    english_map[79] = ui->pushButton9;
    english_map[80] = ui->pushButton10;
    english_map[81] = ui->pushButton1;
    english_map[82] = ui->pushButton4;
    english_map[83] = ui->pushButton14;
    english_map[84] = ui->pushButton5;
    english_map[85] = ui->pushButton7;
    english_map[86] = ui->pushButton27;
    english_map[87] = ui->pushButton2;
    english_map[88] = ui->pushButton25;
    english_map[89] = ui->pushButton6;
    english_map[90] = ui->pushButton24;
    english_map[91] = ui->pushButton11;
    english_map[93] = ui->pushButton12;

    english.push_back(" ");	 // 32
    english.push_back("'");	 // 39
    english.push_back(",");	 // 44
    english.push_back(".");	 // 46
    english.push_back("/");	 // 47
    english.push_back(";");	 // 59
    english.push_back("a");	 // 65
    english.push_back("b");	 // 66
    english.push_back("c");	 // 67
    english.push_back("d");	 // 68
    english.push_back("e");	 // 69
    english.push_back("f");	 // 70
    english.push_back("g");	 // 71
    english.push_back("h");	 // 72
    english.push_back("i");	 // 73
    english.push_back("j");	 // 74
    english.push_back("k");	 // 75
    english.push_back("l");	 // 76
    english.push_back("m");	 // 77
    english.push_back("n");	 // 78
    english.push_back("o");	 // 79
    english.push_back("p");	 // 80
    english.push_back("q");	 // 81
    english.push_back("r");	 // 82
    english.push_back("s");	 // 83
    english.push_back("t");	 // 84
    english.push_back("u");	 // 85
    english.push_back("v");	 // 86
    english.push_back("w");	 // 87
    english.push_back("x");	 // 88
    english.push_back("y");	 // 89
    english.push_back("z");	 // 90
    english.push_back("[");	 // 91
    english.push_back("]");	 // 93

    auto it = english_map.constBegin();
    for (int i = 0; it != english_map.constEnd(); ++it, ++i) {
        if (i < english.size()) {
            it.value()->setText(english[i]);
        }
    }*/
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (isFileDialogOpen) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    int key = event->nativeScanCode();
    qDebug() << key;
    if (currentLayout->keyMap.contains(key)) {
        if (current_symbol == 0) {
            timer->start(1);
            colored_text = "";
            ui->label->setText(text);
        }
        QPushButton* btn = currentLayout->keyMap[key];
        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: gray;"
            "   color: white;"
            "   border: 2px solid #00a8ff;"
            "   border-radius: 15px;"
            "}");
        pressed_buttons.insert(btn);
        QString pressed_text = event->text();
        QChar pressed_char = pressed_text.at(0);
        if (text[current_symbol] == pressed_char) {
            status = 1;
            right_symbol++;
        } else if (text[current_symbol] != pressed_char &&
                   text[current_symbol] == ' ') {
            text.insert(current_symbol, pressed_text);
            status = -1;
        } else {
            status = 0;
        }
        if (status != -1) {
            index_without_error_in_space++;
        }

        QString part = start_text.left(index_without_error_in_space + 1);
        number_of_words = NumberOfWords(part);
        double sec_in_min = second / 60.0 + minute + mili_second / 60000;
        if (sec_in_min > 0) {
            frequency = number_of_words / sec_in_min;
        } else {
            frequency = 0;
        }
        ui->frequencylabel->setText(QString::number(frequency));

        ui->rightLabel->setText(QString::number(
            (double)(right_symbol * 100 / start_text.length())));

        updateLabelColoring(current_symbol, status);
        current_symbol++;
    }
    if (current_symbol == text.length()) {
        timer->stop();
        ui->rightLabel->setText(QString::number(
            (double)(right_symbol * 100 / start_text.length())));
        current_symbol = 0;

        text = start_text;
        mili_second = 0;
        second = 0;
        minute = 0;
        frequency = 0;
        number_of_words = 0;
        index_without_error_in_space = 0;
        right_symbol = 0;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::updateLabelColoring(int index, int status) {
    QString result;
    switch (status) {
        case 1:
            colored_text += QString("<span style='color:green;'>") +
                            text.at(index) + QString("</span>");
            break;
        case -1:
            colored_text += QString(
                                "<span style='"
                                "color: red;"
                                "text-decoration: underline;"
                                "'>") +
                            text.at(index) + QString("</span>");
            break;
        case 0:
            colored_text += QString("<span style='color:red;'>") +
                            text.at(index) + QString("</span>");
            break;
    }

    result = colored_text;
    result += text.mid(index + 1);

    ui->label->setText(result);
}

int MainWindow::NumberOfWords(QString str) {
    int wordCount = 0;
    bool inWord = false;


    for (const QChar& ch : str) {
        if (ch.isLetterOrNumber() || ch == '\'') {
            if (!inWord) {
                wordCount++;
                inWord = true;
            }
        } else {
            inWord = false;
        }
    }
    return wordCount;
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    int key = event->nativeScanCode();

    if (currentLayout->keyMap.contains(key)) {
        QPushButton* btn = currentLayout->keyMap[key];
        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: black;"
            "   color: white;"
            "   border: 2px solid #00a8ff;"
            "   border-radius: 15px;"
            "   padding: 8px;"
            "   font-weight: bold;"
            "}");
        pressed_buttons.remove(btn);
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::TimerSlot() {
    mili_second++;
    if (mili_second > 1000) {
        mili_second = 0;
        second++;
    }
    if (second > 60) {
        second = 0;
        minute++;
    }

    ui->minuteLabel->setText(QString("%1").arg(minute, 3, 10, QChar('0')) +
                             " : ");

    ui->secondLabel->setText(QString("%1").arg(second, 2, 10, QChar('0')));

    ui->milisecondLabel->setText(
        " : " + QString("%1").arg(mili_second, 3, 10, QChar('0')));
}

void MainWindow::onSelectFileClicked() {
    if (isFileDialogOpen)
        return;
    isFileDialogOpen = true;

    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Text File"), "", tr("Text Files (*.txt)"));

    isFileDialogOpen = false;

    if (!fileName.isEmpty()) {
        start_text = loadTextFromFile(fileName);
        text = start_text;
        current_symbol = 0;
        right_symbol = 0;
        ui->label->setText(text);

        timer->stop();
        mili_second = 0;
        second = 0;
        minute = 0;
        frequency = 0;
        number_of_words = 0;
        index_without_error_in_space = 0;

        ui->frequencylabel->setText("0");
        ui->rightLabel->setText("0");
        ui->minuteLabel->setText(QString("%1").arg(minute, 3, 10, QChar('0')) +
                                 " : ");

        ui->secondLabel->setText(QString("%1").arg(second, 2, 10, QChar('0')));

        ui->milisecondLabel->setText(
            " : " + QString("%1").arg(mili_second, 3, 10, QChar('0')));
        this->setFocus();
    }
}

void MainWindow::onLanguageChanged() {
    int index = ui->languageBox->currentIndex();
    switch (index) {
        case 0:
            initEnglishLayout();
            currentLayout = &englishLayout;
            start_text = loadTextFromFile(
                "/home/mkh-alez/lab/lab_5/task_1/english_1.txt");
            break;
        case 1:
            initRussianLayout();
            currentLayout = &russianLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/russian.txt");
            break;
        case 2:
            initGermanLayout();
            currentLayout = &germanLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/german.txt");
            break;
        case 3:
            initFrenchLayout();
            currentLayout = &frenchLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/french.txt");
            break;
        case 4:
            initArabicLayout();
            currentLayout = &arabicLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/arabic.txt");
            break;
        case 5:
            initChineseLayout();
            currentLayout = &chineseLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/chinese.txt");
            break;
        case 6:
            initBelarusianLayout();
            currentLayout = &belarusianLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/belarus.txt");
            break;
        case 7:
            initHebrewLayout();
            currentLayout = &hebrewLayout;
            start_text =
                loadTextFromFile("/home/mkh-alez/lab/lab_5/task_1/hebrew.txt");
            break;
    }
    updateKeyboardLayout();
    text = start_text;
    current_symbol = 0;
    right_symbol = 0;
    ui->label->setText(text);

    timer->stop();
    mili_second = 0;
    second = 0;
    minute = 0;
    frequency = 0;
    number_of_words = 0;
    index_without_error_in_space = 0;

    ui->frequencylabel->setText("0");
    ui->rightLabel->setText("0");
    ui->minuteLabel->setText(QString("%1").arg(minute, 3, 10, QChar('0')) +
                             " : ");

    ui->secondLabel->setText(QString("%1").arg(second, 2, 10, QChar('0')));

    ui->milisecondLabel->setText(
        " : " + QString("%1").arg(mili_second, 3, 10, QChar('0')));
    this->setFocus();
}

QString MainWindow::loadTextFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return "Quick foxes jump over lazy dogs.";
    }
    QTextStream in(&file);
    return in.readLine();
}

void MainWindow::initEnglishLayout() {
    englishLayout.keyMap[24] = ui->pushButton1;
    englishLayout.keyMap[25] = ui->pushButton2;
    englishLayout.keyMap[26] = ui->pushButton3;
    englishLayout.keyMap[27] = ui->pushButton4;
    englishLayout.keyMap[28] = ui->pushButton5;
    englishLayout.keyMap[29] = ui->pushButton6;
    englishLayout.keyMap[30] = ui->pushButton7;
    englishLayout.keyMap[31] = ui->pushButton8;
    englishLayout.keyMap[32] = ui->pushButton9;
    englishLayout.keyMap[33] = ui->pushButton10;
    englishLayout.keyMap[34] = ui->pushButton11;
    englishLayout.keyMap[35] = ui->pushButton12;
    englishLayout.keyMap[38] = ui->pushButton13;
    englishLayout.keyMap[39] = ui->pushButton14;
    englishLayout.keyMap[40] = ui->pushButton15;
    englishLayout.keyMap[41] = ui->pushButton16;
    englishLayout.keyMap[42] = ui->pushButton17;
    englishLayout.keyMap[43] = ui->pushButton18;
    englishLayout.keyMap[44] = ui->pushButton19;
    englishLayout.keyMap[45] = ui->pushButton20;
    englishLayout.keyMap[46] = ui->pushButton21;
    englishLayout.keyMap[47] = ui->pushButton22;
    englishLayout.keyMap[48] = ui->pushButton23;
    englishLayout.keyMap[52] = ui->pushButton24;
    englishLayout.keyMap[53] = ui->pushButton25;
    englishLayout.keyMap[54] = ui->pushButton26;
    englishLayout.keyMap[55] = ui->pushButton27;
    englishLayout.keyMap[56] = ui->pushButton28;
    englishLayout.keyMap[57] = ui->pushButton29;
    englishLayout.keyMap[58] = ui->pushButton30;
    englishLayout.keyMap[59] = ui->pushButton31;
    englishLayout.keyMap[60] = ui->pushButton32;
    englishLayout.keyMap[61] = ui->pushButton33;
    englishLayout.keyMap[65] = ui->pushButton34;

    englishLayout.characters = {"q", "w", "e", "r", "t", "y", "u", "i", "o",
                                "p", "[", "]", "a", "s", "d", "f", "g", "h",
                                "j", "k", "l", ";", "'", "z", "x", "c", "v",
                                "b", "n", "m", ",", ".", "/", " "};
}

void MainWindow::initRussianLayout() {
    russianLayout.keyMap = englishLayout.keyMap;

    russianLayout.characters = {"й", "ц", "у", "к", "е", "н", "г", "ш", "щ",
                                "з", "х", "ъ", "ф", "ы", "в", "а", "п", "р",
                                "о", "л", "д", "ж", "э", "я", "ч", "с", "м",
                                "и", "т", "ь", "б", "ю", ".", " "};
}

void MainWindow::initGermanLayout() {
    germanLayout.keyMap = englishLayout.keyMap;

    germanLayout.characters = {"q", "w", "e", "r", "t", "z", "u", "i", "o",
                               "p", "ü", "+", "a", "s", "d", "f", "g", "h",
                               "j", "k", "l", "ö", "ä", "#", "y", "x", "c",
                               "v", "b", "n", "m", ",", ".", "-", " "};
}

void MainWindow::initFrenchLayout() {
    frenchLayout.keyMap = englishLayout.keyMap;

    frenchLayout.characters = {"a", "z", "e", "r", "t", "y", "u", "i", "o",
                               "p", "^", "$", "q", "s", "d", "f", "g", "h",
                               "j", "k", "l", "m", "ù", "*", "w", "x", "c",
                               "v", "b", "n", ",", ";", ":", "!", " "};
}

void MainWindow::initArabicLayout() {
    arabicLayout.keyMap = englishLayout.keyMap;

    arabicLayout.characters = {"ض",	 "ص", "ث", "ق", "ف", "غ", "ع", "ه", "خ",
                               "ح",	 "ج", "د", "ش", "س", "ي", "ب", "ل", "ا",
                               "ت",	 "ن", "م", "ك", "ط", "ئ", "ء", "ؤ", "ر",
                               "لا", "ى", "ة", "و", "ز", "ظ", " "};
}

void MainWindow::initChineseLayout() {
    chineseLayout.keyMap = englishLayout.keyMap;

    chineseLayout.characters = {
        "的", "一", "是", "在", "不", "了", "有", "和", "人", "这", "【", "】",
        "中", "大", "为", "上", "个", "国", "我", "以", "要", "他", "、", "时",
        "要", "用", "们", "生", "到", "作", "，", "。", "？", " "};
}

void MainWindow::initBelarusianLayout() {
    belarusianLayout.keyMap = englishLayout.keyMap;

    belarusianLayout.characters = {"й", "ц", "у", "к", "е", "н", "г", "ш", "ў",
                                   "з", "х", "'", "ф", "ы", "в", "а", "п", "р",
                                   "о", "л", "д", "ж", "э", "я", "ч", "с", "м",
                                   "і", "т", "ь", "б", "ю", ".", " "};
}

void MainWindow::initHebrewLayout() {
    hebrewLayout.keyMap = englishLayout.keyMap;

    hebrewLayout.characters = {"/", "'", "ק", "ר", "א", "ט", "ו", "ן", "ם",
                               "פ", "]", "[", "ש", "ד", "ג", "כ", "ע", "י",
                               "ח", "ל", "ך", "ף", ",", "ז", "ס", "ב", "ה",
                               "נ", "מ", "צ", "ת", "ץ", ".", " "};
}

void MainWindow::updateKeyboardLayout() {
    if (!currentLayout)
        return;

    auto it = currentLayout->keyMap.constBegin();

    for (int i = 0; it != currentLayout->keyMap.constEnd(); ++it, ++i) {
        if (i < currentLayout->characters.size()) {
            QPushButton* button = it.value();
            button->setText(currentLayout->characters[i]);
        }
    }
}
