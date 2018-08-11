#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QShortcut>
#include <QTime>

#include <player.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    bool eventF ilter(QObject *obj, QEvent *event);

private slots:
    void btnCameraOpenClicked();

    void btnPlayPauseClicked();
    void btnPlayPause_set_Play();
    void btnPlayPause_set_Pause();

    void updatePlayerUI(QImage img);

    QString getFormattedTime(int timeInSeconds);

    void frameSliderPressed();
    void frameSliderReleased();
    void frameSliderMoved(int position);

    void mouseDoubleClick(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    void resizeEvent(QResizeEvent *event);

    Player* myPlayer;
    QString lastDir;
};

#endif // MAINWINDOW_H
