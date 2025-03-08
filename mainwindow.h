#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QFont>
#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QImage>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QPixmap>
#include <QBuffer>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>
// #include <math>

class Painter_for_new_widget : public QWidget{
    Q_OBJECT
public:
    QTextEdit* txt_dublikate;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent*);
    void closeEvent(QCloseEvent *);
    Painter_for_new_widget(QWidget *parent = nullptr);
    bool check_paint = false;
    QString result = "";
    int local_first_click_X, local_first_click_Y, local_out_first_click_X, local_out_first_click_Y;
    int global_first_click_X, global_first_click_Y, global_out_first_click_X, global_out_first_click_Y;
    QSize screen;
    void paintEvent(QPaintEvent*);
    QString lang;

private slots:
};


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int x = 20;
    Painter_for_new_widget* window_supple;

    QTextEdit* txt;
private:
    QString lang;
    void set_txt_on_window(const QTextEdit&);
    void main_function();
    void closeEvent(QCloseEvent *);
    QSize screen;
    QStringList st_list;
    QHBoxLayout* box_h;
    QVBoxLayout* box_v;
    QPushButton* Button_out_txt;
    QPushButton* Button_clear_txt;
    QPushButton* test_rect;
    QComboBox* boxes;
    QLabel* info_lang;
 private slots:

     void clear_field(){txt->clear();}
     void button_func(){main_function();}
     void choice_lang(const QString& str){
         qDebug() << this << "\t" << lang;
         if(str == ""){return;}
         lang = str;
     }
};
#endif // MAINWINDOW_H
