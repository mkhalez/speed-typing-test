#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    MyString str = "123";
    MyString test = str;
    qDebug() << test;
}

MainWindow::~MainWindow() {
    delete ui;
}
