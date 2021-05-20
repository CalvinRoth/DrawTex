#ifndef SCRIBBLE_H
#define SCRIBBLE_H
#include <QImage>
#include <QColor>
#include <QPoint>
#include <QWidget>
#include <QVector>
#include <state.h>
class Scribble : public QWidget
{
    Q_OBJECT
public:
        Scribble(QWidget *parent = 0);
        bool openImage(const QString &fileName);
        bool saveImage(const QString &fileName, const char* fileFormat);
        void setPenColor(const QColor &newColor);
        void setPenWidth(int newWidth);
        bool isModified() const {return modified;}
        QColor penColor() const {return myPenColor;}
        int penWidth() const {return myPenWidth;}
        int getDrawState();
        void setDrawState(int newState);
        QString pointToQString(const QPoint &point);
        QString colorToRGBs();
        QString getTex();
        QList<state> stateList;
public slots:
        void clearImage();
        void print();
        void lineMode();
        void ellipsMode();
        void rectMode();
        void undo();

protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
private:
        void resizeImage(QImage *image, const QSize &newSize);
        void drawLineTo(const QPoint &endPoint);
        void drawEllipse(const QPoint &endPoint);
        void drawRect(const QPoint &endpoint);
        bool modified;
        bool scribbing;
        QColor myPenColor;
        int myPenWidth;
        QImage image;
        QPoint lastPoint;
        int drawState = 1;
        //QPainter painter;
        QString texOut = "\\begin{tikzpicture}[xscale=0.02, yscale=-0.02]\n";
};
#endif // SCRIBBLE_H
