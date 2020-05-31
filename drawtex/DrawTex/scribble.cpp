#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include <math.h>
#endif
#endif

#include "scribble.h"

Scribble::Scribble(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbing = false;
    myPenColor = Qt::blue;
    myPenWidth = 1;
}

bool Scribble::openImage(const QString &fileName){
    QImage loadedImage;
    if(!loadedImage.load(fileName)){
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool Scribble::saveImage(const QString &fileName, const char* fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    }
    return false;
}

void Scribble::setPenColor(const QColor &newColor){
    myPenColor = newColor;
}

void Scribble::setPenWidth(int newWidth){
    myPenWidth = newWidth;
}

void Scribble::setDrawState(int newState){
    drawState = newState;
}

int Scribble::getDrawState(){
    return drawState;
}

void Scribble::clearImage(){
    image.fill(qRgb(255,255,255));
    modified = true;
    texOut = "\\begin{tikzpicture}[scale=0.02]\n";
    update();
}

void Scribble::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        scribbing = true;
    }
}


void Scribble::mouseMoveEvent(QMouseEvent *event){
//    if((event->buttons() & Qt::LeftButton) && scribbing)
//            drawLineTo((event->pos()));
}

void Scribble::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && scribbing){
        switch(drawState) {
            case 0 :
                drawLineTo(event->pos());
                break;
            case 1 :
                drawEllipse(event->pos());
                break;
            case 2 :
                drawRect(event->pos());
                break;
            default :
                qDebug() << "Oh no!";
                exit(0);
        }
        //drawLineTo(event->pos());
        scribbing = false;
    }
}

void Scribble::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void Scribble::resizeEvent(QResizeEvent *event){
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

QString Scribble::pointToQString(const QPoint &point){
    QString s = "(" + QString::number(point.x());
    s = s  + "," + QString::number(point.y()) + ")";
    return s;
}

QString Scribble::colorToRGBs(){
    QString rgb[3] = {QString::number(myPenColor.red()), QString::number(myPenColor.green()), QString::number(myPenColor.blue())};
    QString s = "red, " + rgb[0] + "; green, " + rgb[1] + "; blue," + rgb[2];
    return s;
}

void Scribble::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth,
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    QString rgb[3] = {QString::number(myPenColor.red()), QString::number(myPenColor.green()), QString::number(myPenColor.blue())};

    texOut = texOut + "\\draw[line width = ";
    texOut = texOut + QString::number(myPenWidth) + ", color= {rgb,255:";
    texOut= texOut + colorToRGBs() + "}] ";
    texOut = texOut + pointToQString(lastPoint) + " -- " + pointToQString(endPoint);
    texOut = texOut + "; \n";
    lastPoint = endPoint;
}

void Scribble::drawEllipse(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth,
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter.drawEllipse(lastPoint, endPoint);
    painter.drawEllipse(lastPoint, abs(endPoint.x()-lastPoint.x()), abs(endPoint.y()-lastPoint.y()));
    \
    //TODO OutLine Width
    texOut += "\\draw[color = {rgb, 255:" + colorToRGBs() + "}, ]" + pointToQString(lastPoint) + "ellipse (" +
            QString::number(abs(endPoint.x()-lastPoint.x())) + " and " + QString::number(abs(endPoint.y()-lastPoint.y())) + "); \n";
    modified = true;
    update();
    lastPoint = endPoint;
}

void Scribble::drawRect(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth,
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(lastPoint.x(), lastPoint.y(), -lastPoint.x() +endPoint.x(), -lastPoint.y()+endPoint.y());
    texOut += "\\draw[color = {rgb, 255:" + colorToRGBs() + "}]" + pointToQString(lastPoint) + " rectangle " + pointToQString(endPoint) + "; \n";
    modified = true;
    update();
    lastPoint = endPoint;
}

QString Scribble::getTex(){
    return texOut;
}


void Scribble::resizeImage(QImage *image, const QSize &newSize){
    if(image->size() == newSize){
        return;
    }
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}

void Scribble::print(){
 #if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted){
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0,0,image);
    }
#endif
}


void Scribble::lineMode(){
    drawState = 0;
}
void Scribble::ellipsMode(){
    drawState = 1;
}

void Scribble::rectMode(){
    drawState = 2;
}
