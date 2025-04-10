#ifndef SCANNER_TEXT_H
#define SCANNER_TEXT_H


#include <QApplication>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QBuffer>
#include <QLabel>
#include <QComboBox>

class Painter_for_new_widget : public QWidget{
    Q_OBJECT
public:
    Painter_for_new_widget(QWidget *parent = nullptr);


    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent*);
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent*);

    bool check_paint = false;
    int local_first_click_X, local_first_click_Y, local_out_first_click_X, local_out_first_click_Y;
    int global_first_click_X, global_first_click_Y, global_out_first_click_X, global_out_first_click_Y;
    QString result = "";
    QSize screen;
    QTextEdit* txt_dublikate;
    QString lang;
};


class Scanner_text : public QWidget
{
    Q_OBJECT

public:
    Scanner_text(QWidget *parent = nullptr);
    int x = 20;
    Painter_for_new_widget* window_supple;
    QTextEdit* txt;

private:

    void set_txt_on_window(const QTextEdit&);
    void main_function();
    void closeEvent(QCloseEvent *);

    QString lang;
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
         //qDebug() << this << "\t" << lang;
         if(str == ""){return;}
         lang = str;
     }
};
#endif // SCANNER_TEXT_H
