#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Instantiate myPlayer class
    myPlayer = new Player();

    // GUI settings
    ui->setupUi(this);
    ui->videoCanvas->setPixmap(QPixmap());
    this->showMaximized();
    
    // Connect SIGNAL and SLOTS
    connect(ui->btnCameraOpen, SIGNAL(clicked()), this, SLOT(btnCameraOpenClicked()));
    connect(ui->btnPlayPause, SIGNAL(clicked()), this, SLOT(btnPlayPauseClicked()));

    connect(myPlayer, SIGNAL(processedImage(QImage)), this, SLOT(updatePlayerUI(QImage)));
    connect(myPlayer, SIGNAL(started()), this, SLOT(btnPlayPause_set_Pause()));
    connect(myPlayer, SIGNAL(finished()), this, SLOT(btnPlayPause_set_Play()));

    connect(ui->frameSlider, SIGNAL(sliderPressed()), this, SLOT(frameSliderPressed()));
    connect(ui->frameSlider, SIGNAL(sliderReleased()), this, SLOT(frameSliderReleased()));
    connect(ui->frameSlider, SIGNAL(sliderMoved(int)), this, SLOT(frameSliderMoved(int)));

    connect(ui->videoCanvas, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(mouseDoubleClick(QMouseEvent *)));
    connect(ui->videoCanvas, SIGNAL(mousePressed(QMouseEvent *)), this, SLOT(mousePressed(QMouseEvent *)));
    connect(ui->videoCanvas, SIGNAL(mouseMoved(QMouseEvent *)), this, SLOT(mouseMoved(QMouseEvent *)));

    // Keyboard shortcuts
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close())); // Quit app macOS
    new QShortcut(QKeySequence(Qt::ALT + Qt::Key_F4), this, SLOT(close())); // Quit app Windows

}

MainWindow::~MainWindow()
{
    delete myPlayer;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->statusBar()->showMessage(tr("Test"), 3000);

    // Update video canvas
    if (myPlayer->isOpened() && !myPlayer->isRunning()) {
        ui->videoCanvas->setAlignment(Qt::AlignCenter);
        ui->videoCanvas->setPixmap(QPixmap::fromImage(myPlayer->image()).scaled(
                                       ui->videoCanvas->size(),
                                       Qt::KeepAspectRatio,
                                       Qt::FastTransformation));
    }
}

void MainWindow::btnCameraOpenClicked()
{
    if (lastDir.isEmpty()) {
        lastDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0, QDir::homePath());
    }

    myPlayer->Stop();

    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open video"));
    fileDialog.setMimeTypeFilters(myPlayer->mimeTypeFilters);
    fileDialog.setDirectory(lastDir);

    if (fileDialog.exec() == QDialog::Accepted) {
        if (myPlayer->loadVideo(fileDialog.selectedFiles().first())) {
            ui->btnPlayPause->setEnabled(true);
            ui->frameSlider->setEnabled(true);

            ui->frameSlider->setMaximum(int(myPlayer->getNumberOfFrames()));
            ui->labelVideoTime->setText(getFormattedTime(int(myPlayer->getNumberOfFrames() / myPlayer->getFrameRate())));

            this->updatePlayerUI(myPlayer->getFrame());

            lastDir = fileDialog.directory().path();
        } else {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
    }
}

void MainWindow::btnPlayPauseClicked()
{
    if (!myPlayer->isRunning()) {
        myPlayer->Play();
    } else {
        myPlayer->Stop();
    }
}

void MainWindow::btnPlayPause_set_Play()
{
    ui->btnPlayPause->setIcon(QIcon(":/pics/MediaPlay.png"));
}

void MainWindow::btnPlayPause_set_Pause()
{
    ui->btnPlayPause->setIcon(QIcon(":/pics/MediaPause.png"));
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->videoCanvas->setAlignment(Qt::AlignCenter);
        ui->videoCanvas->setPixmap(QPixmap::fromImage(img).scaled(ui->videoCanvas->size(), Qt::KeepAspectRatio, Qt::FastTransformation));

        ui->frameSlider->setValue(int(myPlayer->getCurrentFrame()));
        ui->labelVideoTime->setText(getFormattedTime(int(myPlayer->getCurrentFrame() / myPlayer->getFrameRate())));
    }
}

QString MainWindow::getFormattedTime(int timeInSeconds)
{
    int seconds = int(timeInSeconds) % 60;
    int minutes = int((timeInSeconds / 60) % 60);
    int hours   = int((timeInSeconds / (60*60) % 24));

    QTime t(hours, minutes, seconds);
    if (hours == 0) {
        return t.toString("mm:ss");
    } else {
        return t.toString("h:mm:ss");
    }
}

void MainWindow::frameSliderPressed()
{
    myPlayer->Stop();
    frameSliderMoved(ui->frameSlider->value());
}

void MainWindow::frameSliderReleased()
{
    myPlayer->Play();
}

void MainWindow::frameSliderMoved(int position)
{
    if (position >= int(myPlayer->getNumberOfFrames())) {
        position = int(myPlayer->getNumberOfFrames() - 1);
    }

    myPlayer->setCurrentFrame(position);
    ui->labelVideoTime->setText(getFormattedTime(position / int(myPlayer->getFrameRate())));

    ui->videoCanvas->setAlignment(Qt::AlignCenter);
    ui->videoCanvas->setPixmap(QPixmap::fromImage(myPlayer->getFrame()).scaled(
                                   ui->videoCanvas->size(),
                                   Qt::KeepAspectRatio,
                                   Qt::FastTransformation));
}

void MainWindow::mouseDoubleClick(QMouseEvent *event)
{
    statusBar()->showMessage(tr("Double Click"), 3000);
}

void MainWindow::mousePressed(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        statusBar()->showMessage(tr("Left button pressed"), 3000);
    } else if (event->button() == Qt::RightButton) {
        statusBar()->showMessage(tr("Right button pressed"), 3000);
    }
}

void MainWindow::mouseMoved(QMouseEvent *event)
{
    statusBar()->showMessage(tr("x=%1, y=%2").arg(event->pos().x()).arg(event->pos().y()), 3000);
}


