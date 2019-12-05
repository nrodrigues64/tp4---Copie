#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <QFileDialog>
#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QSlider>
#include <QComboBox>
#include <QPixmap>
#include <QTimer>
#include <QResizeEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QPixmap>
#include <QInputDialog>
class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPushButton *open_Dir = new QPushButton("&Open Directory Image",this);
    QPushButton *load_Calib = new QPushButton("&Load Calibration",this);
    QPushButton *save_Calib = new QPushButton("&Save Current Calibration",this);
    QPushButton *open_Left_Img = new QPushButton("&Open Left Image",this), *open_Right_Img  = new QPushButton("&Open Right Image");
    QTextEdit *txt = new QTextEdit();
    QLabel *l = new QLabel,*l1 = new QLabel,*t = new QLabel, *t1 = new QLabel;
    std::vector<cv::Mat> all_Img_lft,all_Img_rgt;
    std::vector<cv::Mat> all_Img_Gray_lft, all_Img_Gray_rgt;
    std::vector<cv::String> fn_left, fn_right;
    std::vector< std::vector< cv::Point3f > > object_points;
    std::vector< std::vector< cv::Point2f > > image_points1,image_points2;
    std::vector< cv::Point2f > corners1,corners2;
    std::vector< cv::Mat > rvecs, tvecs;
    cv::Mat K1,K2;
    cv::Mat D1,D2;
    cv::Mat R,F,E;
    cv::Vec3d T;
    cv::Mat img_lft, img_right;
    QPixmap p,p1;
    cv:: Mat map1x, map1y, map2x, map2y;

    cv::Mat R1, R2, P1, P2, Q;
    QGridLayout *layout = new QGridLayout;
    int board_width = 9;
    int board_height = 6;
    double square_size = 24.33;


public:
    MainWindow(QWidget *parent = 0);
    void displayCurrentCalib(QString);

    ~MainWindow();

private slots:
    void openDir();
    void openLeftImg();
    void openRightImg();
    void saveCalib();
    void loadCalib();
};

#endif // MAINWINDOW_H
