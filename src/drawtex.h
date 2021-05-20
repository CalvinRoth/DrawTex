#ifndef DRAWTEX_H
#define DRAWTEX_H

#include <QMainWindow>
#include <QList>
#include <QMainWindow>
class Scribble;

namespace Ui {
class DrawTex;
}

class DrawTex : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrawTex(QWidget *parent = 0);
    ~DrawTex();
    //DrawTex();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
    void exportTex();



private:
    Ui::DrawTex *ui;
    void createActions();
    void createMenues();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    Scribble *scribbleArea;
    QMenu* saveAsMenu;
    QMenu* fileMenu;
    QMenu* optionMenu;
    QMenu* helpMenu;
    QMenu* shapeMenu;
    QAction* openAct;
    QAction* exportTexAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *lineAct;
    QAction *ellipseAct;
    QAction *rectangleAct;
    QAction *undoAct;

};

#endif // DRAWTEX_H
