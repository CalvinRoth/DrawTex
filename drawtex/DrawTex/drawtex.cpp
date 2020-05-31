#include "drawtex.h"
#include "ui_drawtex.h"
#include "scribble.h"
#include <QDebug>
#include <QMessageBox>

#include <QtWidgets>


DrawTex::DrawTex(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawTex)
{
    ui->setupUi(this);
    scribbleArea = new Scribble;
    setCentralWidget(scribbleArea);
    createActions();
    createMenues();
    setWindowTitle(tr("Scribble"));
    resize(500,500);


}

DrawTex::~DrawTex()
{
    delete ui;
}

void DrawTex::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    } else {
        event->ignore();
    }
}

void DrawTex::open(){
    if(maybeSave()){
       QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                       QDir::currentPath());
       if(!fileName.isEmpty()){
           scribbleArea->openImage(fileName);
       }
    }
}

void DrawTex::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void DrawTex::penColor(){
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if(newColor.isValid()){
        scribbleArea->setPenColor(newColor);
    }
}

void DrawTex::penWidth(){
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen Width: "),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if(ok){
        scribbleArea->setPenWidth(newWidth);
    }
}

void DrawTex::about(){
    QMessageBox::about(this, tr("About scribble"), tr("<p> lorem ipsum </p>"));
}

void DrawTex::createActions(){
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(open()));
    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));
    exportTexAct = new QAction(tr("Export to Latex"), this);
    exportTexAct->setShortcut(tr("Ctrl+E"));
    connect(exportTexAct, SIGNAL(triggered()), this, SLOT(exportTex()));
    exitAct = new QAction(tr("&Exit"),this);
    exitAct-> setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction(tr("&Pen Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));
    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));
    aboutQtAct = new QAction(tr("&About Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    lineAct = new QAction(tr("Line"), this);
    connect(lineAct, SIGNAL(triggered()), scribbleArea, SLOT(lineMode()));
    ellipseAct = new QAction(tr("Ellipse"), this);
    connect(ellipseAct, SIGNAL(triggered()), scribbleArea, SLOT(ellipsMode()));
    rectangleAct = new QAction(tr("Rectangle"), this);
    connect(rectangleAct, SIGNAL(triggered()), scribbleArea, SLOT(rectMode()));

}


void DrawTex::createMenues(){
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addAction(exportTexAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    optionMenu = new QMenu(tr("&Options"),this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"),this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    shapeMenu = new QMenu(tr("Shape"), this);
    shapeMenu->addAction(lineAct);
    shapeMenu->addAction(ellipseAct);
    shapeMenu->addAction(rectangleAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
    menuBar()->addMenu(shapeMenu);
}

bool DrawTex::maybeSave(){
    if(scribbleArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified\n Do you want to save you changes"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save){
            return saveFile("png");
        } else if(ret==QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}


bool DrawTex::saveFile(const QByteArray &fileformat){
    QString initalPath = QDir::currentPath()+ "/untitled" + fileformat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initalPath,
                                                    tr("%1 Files (*.%2);; All Files(*))")
                                                    .arg(QString::fromLatin1(fileformat.toUpper()))
                                                    .arg(QString::fromLatin1(fileformat)));
    if(fileName.isEmpty()){
        return false;
    }
    return scribbleArea->saveImage(fileName, fileformat.constData());
}

void DrawTex::exportTex(){
    QString texOut = scribbleArea->getTex() + "\\end{tikzpicture}";
    QMessageBox::about(this, "Latex Output", texOut);
}
