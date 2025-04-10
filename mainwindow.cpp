#include "scanner_text.h"

Scanner_text::Scanner_text(QWidget *parent)
    : QWidget(parent)
{

    const int size_x = 600, size_y = 400;
    setWindowTitle("Сканер текста"); // Наименование

    setMaximumHeight(size_y);// Размеры высота, ширина окна
    setMaximumWidth(size_x);
    setMinimumHeight(size_y);
    setMinimumWidth(size_x);

    txt = new QTextEdit(this);

    txt->setMinimumHeight(size_y / 2); // Размеры высота, ширина выходящего текста
    txt->setMinimumWidth(size_x);
    txt->setReadOnly(true); // Только копирование
    QFont font("Arial", 14);


    Button_out_txt = new QPushButton("Выделить область и вывести в текст выше"); // Созданы две кнопки
    Button_clear_txt = new QPushButton("Очистить текст");

    Button_clear_txt->setFont(font); // Установим шрифты
    Button_out_txt ->setFont(font);

    Button_out_txt->setShortcut(QKeySequence("Ctrl+s"));

    st_list = {{"rus"}, {"eng"}, {"rus+eng"}};
    info_lang = new QLabel("Выберите язык для распознования:");
    info_lang->setFont(font);
    boxes = new QComboBox();
    boxes->setFont(font);
    boxes->addItems(st_list);

    connect(boxes, &QComboBox::currentTextChanged, this, &Scanner_text::choice_lang);

    box_v = new QVBoxLayout(); // Для метки и выбора языка
    box_h = new QHBoxLayout(); // Для двух кнопок

    box_v->addStretch(1);
    box_v->addSpacing(100);
    box_v->addWidget(info_lang, 0, Qt::AlignBottom);
    box_v->addWidget(boxes, 0, Qt::AlignBottom);
    box_v->addLayout(box_h);

    box_h->addWidget(Button_out_txt, 0, Qt::AlignBottom);
    box_h->addWidget(Button_clear_txt, 0, Qt::AlignBottom);
    setLayout(box_v);

    connect(Button_clear_txt, SIGNAL(clicked()), this, SLOT(clear_field()));
    connect(Button_out_txt, SIGNAL(clicked()), this, SLOT(button_func()));
}

Painter_for_new_widget::Painter_for_new_widget(QWidget *parent)
    : QWidget(parent)
{

}


void Scanner_text::main_function(){

    window_supple = new Painter_for_new_widget();

    window_supple->txt_dublikate = txt;

    screen = QGuiApplication::primaryScreen()->size();
    window_supple->setMinimumHeight(screen.height());
    window_supple->setMinimumWidth(screen.width());
    window_supple->setWindowOpacity(0.4);

    window_supple->setWindowFlag(Qt::FramelessWindowHint);
    window_supple->setAttribute(Qt::WA_MouseTracking);

    window_supple->lang = this->lang;

    showMinimized();
    window_supple->show();
    QObject::connect(window_supple, &QWidget::destroyed, [this]() {
        activateWindow();
    });

}

void Painter_for_new_widget::closeEvent(QCloseEvent *){
    this->~Painter_for_new_widget();
}

void Scanner_text::closeEvent(QCloseEvent *){
    window_supple->close();
    this->close();
}
void Painter_for_new_widget::mousePressEvent(QMouseEvent * clicker){
    if (clicker->button() == Qt::LeftButton) {
        QPoint localPos = clicker->pos();
        QPoint globalPos = clicker->globalPos();

        global_first_click_X = globalPos.x();
        global_first_click_Y = globalPos.y();

        local_first_click_X= localPos.x();
        local_first_click_Y = localPos.y();

    }
}
void Painter_for_new_widget::mouseMoveEvent(QMouseEvent * movement_mouse){
    if (movement_mouse->buttons()){
        QPoint localPos = movement_mouse->pos();

        local_out_first_click_X= localPos.x();
        local_out_first_click_Y= localPos.y();
        check_paint = true;
        repaint();
    }
}
const QRect helps_object(const int& x, const int& y, const int& height,const int& width){
    return QRect(x+1, y+1, height-2, width-2);
}

void Painter_for_new_widget::mouseReleaseEvent(QMouseEvent* real_click){
    if(real_click->button() == Qt::LeftButton){
        QPoint globalPos = real_click->globalPos();
        QPoint localPos = real_click->pos();

        global_out_first_click_X = globalPos.x();
        global_out_first_click_Y = globalPos.y();

        local_out_first_click_X= localPos.x();
        local_out_first_click_Y= localPos.y();

        QScreen* test = QGuiApplication::primaryScreen();
        QRect rect_test;
        if(global_first_click_X < global_out_first_click_X && global_first_click_Y < global_out_first_click_Y){
            rect_test = helps_object(global_first_click_X, global_first_click_Y, global_out_first_click_X - global_first_click_X, global_out_first_click_Y-global_first_click_Y);
        }
        else if(global_first_click_X > global_out_first_click_X && global_first_click_Y < global_out_first_click_Y){
            rect_test = helps_object(global_out_first_click_X, global_first_click_Y, global_first_click_X-global_out_first_click_X, global_out_first_click_Y - global_first_click_Y);
        }
        else if(global_first_click_X < global_out_first_click_X && global_first_click_Y > global_out_first_click_Y){
            rect_test = helps_object(global_first_click_X, global_out_first_click_Y, global_out_first_click_X-global_first_click_X, global_first_click_Y-global_out_first_click_Y);
        }else{
            rect_test = helps_object(global_out_first_click_X, global_out_first_click_Y, global_first_click_X - global_out_first_click_X, global_first_click_Y-global_out_first_click_Y);
        }


        QPixmap screenshot = test->grabWindow(0, rect_test.x(), rect_test.y(), rect_test.width(), rect_test.height());
        tesseract::TessBaseAPI api;
        QImage qimage = screenshot.toImage();

        int chance_up_power_tesseract = 12;
        QImage q_scaled_i = qimage.scaled(qimage.width() * chance_up_power_tesseract, qimage.height() * chance_up_power_tesseract, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);


        q_scaled_i.save(&buffer, "BMP");

        const unsigned char* imageData = reinterpret_cast<const unsigned char*>(buffer.data().constData());
        size_t imageSize = buffer.size();

        Pix* image = pixReadMem(imageData, imageSize);

        if(lang == "") lang = "rus";    // По умолчанию будет только русский    !   !   !


        std::string sat = this->lang.toStdString();
        api.Init(nullptr, sat.c_str());

        api.SetImage(image);

        result = api.GetUTF8Text();
        txt_dublikate->setText(result);
        this->close();

        buffer.close();
        buffer.setData(nullptr, 0);
        api.End();
        pixDestroy(&image);
    }
}
void Painter_for_new_widget::paintEvent(QPaintEvent *){
    if(check_paint){
    QPainter paint(this);
    paint.setOpacity(0.2);
    paint.drawRect(QRect(local_first_click_X,local_first_click_Y,local_out_first_click_X-local_first_click_X- 1,local_out_first_click_Y- local_first_click_Y - 1));
    paint.fillRect(local_first_click_X+1,local_first_click_Y + 1, local_out_first_click_X-local_first_click_X- 2,local_out_first_click_Y-local_first_click_Y - 2, Qt::red);

    check_paint = false;
    }
}

