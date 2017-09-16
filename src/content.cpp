#include "content.h"

#include <QDebug>
#include <QGridLayout>
#include <QMouseEvent>
namespace pp {

static const int resize_offset = 5;

static const int resize_none =      0x00000;
static const int resize_left =      0x00001;
static const int resize_right =     0x00010;
static const int resize_top =       0x00100;
static const int resize_bottom =    0x01000;

struct content::Private {
    bool leftPressed;
    QPoint lastPos;
    int moveDirection;
    QSize lastSize;
    QPoint lastGobalPos;
};

content::content(QWidget *child) noexcept {
    impl = new Private;
    [=]() {
        impl->leftPressed = false;
        impl->moveDirection = 0;
    }();
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(child, 0, 0);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    // setAttribute(Qt::WA_PaintOnScreen);
    // setWindowOpacity(0.8);
    this->show();
}

content::~content() noexcept { delete impl; }

void content::mousePressEvent(QMouseEvent *event) {
    //鼠标左键按下
    if (event->button() == Qt::LeftButton) {
        //记录按下状态
        impl->leftPressed = true;
        impl->lastPos = event->pos();
        impl->lastSize = this->size();
        impl->lastGobalPos = event->globalPos();
        // left
        if (event->pos().x() < resize_offset)
            impl->moveDirection |= resize_left;
        // right
        if (event->pos().x() > this->size().width() - resize_offset)
            impl->moveDirection |= resize_right;
        // top
        if (event->pos().y() < resize_offset)
            impl->moveDirection |= resize_top;
        // bottom
        if (event->pos().y() > this->size().height() - resize_offset)
            impl->moveDirection |= resize_bottom;
        event->ignore();
    }
}

void content::mouseReleaseEvent(QMouseEvent *event) {
    if (impl->leftPressed)
        impl->leftPressed = false;
    impl->moveDirection = resize_none;
    event->ignore();
}

void content::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (windowState() != Qt::WindowFullScreen) {
            setWindowState(Qt::WindowFullScreen);
        } else {
            setWindowState(Qt::WindowNoState);
        }
    }
}

void content::mouseMoveEvent(QMouseEvent *event) {
    if (impl->leftPressed) {
        qDebug() << event->pos();
        switch (impl->moveDirection) {
        case resize_left:
            qDebug() << " resize_left ";
            if (event->globalPos().x() <
                impl->lastGobalPos.x() + resize_offset) {
                const auto rx = impl->lastSize.width() +
                                impl->lastGobalPos.x() - event->globalPos().x();
                const auto ry = impl->lastSize.height();
                this->resize(rx, ry);
                const auto mx = event->globalPos().x() - impl->lastPos.x();
                const auto my = impl->lastGobalPos.y() - impl->lastPos.y();
                this->move(mx, my);
            }
            break;
        case resize_right:
            qDebug() << " resize_right ";
            this->resize(event->pos().x(), impl->lastSize.height());
            break;
        case resize_bottom:
            qDebug() << " resize_bottom ";
            this->resize(impl->lastSize.width(), event->pos().y());
            break;
        case resize_top:
            qDebug() << " resize_top ";
            if (event->globalPos().y() <
                impl->lastGobalPos.y() + resize_offset) {
                const auto rx = impl->lastSize.width();
                const auto ry = impl->lastSize.height() +
                                impl->lastGobalPos.y() - event->globalPos().y();
                this->resize(rx, ry);
                const auto mx = impl->lastGobalPos.x() - impl->lastPos.x();
                const auto my = event->globalPos().y() - impl->lastPos.y();
                this->move(mx, my);
            }
            break;
        case resize_left | resize_bottom:
            qDebug() << " resize_left|resize_bottom ";
            if (event->globalPos().x() <
                impl->lastGobalPos.x() + resize_offset) {
                const auto rx = impl->lastSize.width() +
                                impl->lastGobalPos.x() - event->globalPos().x();
                this->resize(rx, event->pos().y());
                const auto mx = event->globalPos().x() - impl->lastPos.x();
                const auto my = impl->lastGobalPos.y() - impl->lastPos.y();
                this->move(mx, my);
            } else
                this->resize(impl->lastSize.width(), event->pos().y());
            break;
        case resize_left | resize_top:
            qDebug() << " resize_left|resize_top ";
            if (event->globalPos().x() <
                    impl->lastGobalPos.x() + resize_offset &&
                event->globalPos().y() <
                    impl->lastGobalPos.y() + resize_offset) {
                const auto rx = impl->lastSize.width() +
                                impl->lastGobalPos.x() - event->globalPos().x();
                const auto ry = impl->lastSize.height() +
                                impl->lastGobalPos.y() - event->globalPos().y();
                this->resize(rx, ry);
                const auto mx = event->globalPos().x() - impl->lastPos.x();
                const auto my = event->globalPos().y() - impl->lastPos.y();
                this->move(mx, my);
            } else if (event->globalPos().x() <
                       impl->lastGobalPos.x() + resize_offset) {
                const auto rx = impl->lastSize.width() +
                                impl->lastGobalPos.x() - event->globalPos().x();
                const auto ry = impl->lastSize.height();
                this->resize(rx, ry);
                const auto mx = event->globalPos().x() - impl->lastPos.x();
                const auto my = impl->lastGobalPos.y() - impl->lastPos.y();
                this->move(mx, my);
            } else if (event->globalPos().y() <
                       impl->lastGobalPos.y() + resize_offset) {
                const auto rx = impl->lastSize.width();
                const auto ry = impl->lastSize.height() +
                                impl->lastGobalPos.y() - event->globalPos().y();
                this->resize(rx, ry);
                const auto mx = impl->lastGobalPos.x() - impl->lastPos.x();
                const auto my = event->globalPos().y() - impl->lastPos.y();
                this->move(mx, my);
            }
            break;
        case resize_right | resize_bottom:
            qDebug() << " resize_right|resize_bottom ";
            this->resize(event->pos().x(), event->pos().y());
            break;
        case resize_right | resize_top:
            qDebug() << " resize_right|resize_top ";
            if (event->globalPos().y() <
                impl->lastGobalPos.y() + resize_offset) {
                const auto rx = event->pos().x();
                const auto ry = impl->lastSize.height() +
                                impl->lastGobalPos.y() - event->globalPos().y();
                this->resize(rx, ry);
                const auto mx = impl->lastGobalPos.x() - impl->lastPos.x();
                const auto my = event->globalPos().y() - impl->lastPos.y();
                this->move(mx, my);
            } else
                this->resize(event->pos().x(), impl->lastSize.height());
            break;
        default:
            qDebug() << "move";
            this->move(event->globalPos() - impl->lastPos);
            break;
        }
        event->ignore();
    }
}
}