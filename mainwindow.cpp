#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Calibration");
    QWidget *win;
    win = new QWidget;
    this->setCentralWidget(win);
    connect(open_Dir,SIGNAL(clicked()),this,SLOT(openDir()));
    layout->addWidget(open_Dir,0,0);
    connect(load_Calib,SIGNAL(clicked()),this,SLOT(loadCalib()));
    layout->addWidget(load_Calib,1,0);
    connect(save_Calib,SIGNAL(clicked()),this,SLOT(saveCalib()));
    layout->addWidget(save_Calib,2,0);
    connect(open_Left_Img,SIGNAL(clicked()),this,SLOT(openLeftImg()));
    layout->addWidget(open_Left_Img,3,0);
    connect(open_Right_Img,SIGNAL(clicked()),this,SLOT(openRightImg()));
    layout->addWidget(open_Right_Img,3,1);
    win->setLayout(layout);
    win->show();

}

MainWindow::~MainWindow()
{

}



void MainWindow::openDir()
{
    if(!all_Img_lft.empty())
    {
        all_Img_lft.clear();
    }
    if(!all_Img_Gray_lft.empty())
    {
        all_Img_Gray_lft.clear();
    }
    if(!all_Img_rgt.empty())
    {
        all_Img_rgt.clear();
    }
    if(!all_Img_Gray_rgt.empty())
    {
        all_Img_Gray_rgt.clear();
    }
    if(!fn_left.empty())
    {
        fn_left.clear();
    }
    if(!fn_right.empty())
    {
        fn_right.clear();
    }
    if(!object_points.empty())
    {
        object_points.clear();
    }
    if(!image_points1.empty())
    {
        image_points1.clear();
    }
    if(!image_points2.empty())
    {
        image_points2.clear();
    }
    if(!corners1.empty())
    {
        corners1.clear();
    }
    if(!corners2.empty())
    {
        corners2.clear();
    }

    if(!rvecs.empty())
    {
        rvecs.clear();
    }
    if(!tvecs.empty())
    {
        tvecs.clear();
    }
    board_width = QInputDialog::getInt(this,"Saisir Valeur","Largeur de votre plateau :",9);
    board_height = QInputDialog::getInt(this,"Saisir Valeur","Hauteur de votre plateau :",6);
    square_size = QInputDialog::getDouble(this,"Saisir Valeur", "Taille des carreaux de votre plateau (en mm)",24.33,0,2147483647,2);
    std::cout << board_width << std::endl;
    QString f_left = QFileDialog::getExistingDirectory(this, tr("Open Left Directory"),
                                                  "/home",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
    QString f_right = QFileDialog::getExistingDirectory(this, tr("Open Right Directory"),
                                                  "/home",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
    if(!f_left.isEmpty() || !f_right.isEmpty()){
        std::string st_left = f_left.toUtf8().constData();
        cv::glob(st_left,fn_left,true); // recurse
        std::string st_right = f_right.toUtf8().constData();
        cv::glob(st_right,fn_right,true);
        for (size_t k=0; k<fn_left.size()-1; ++k)
        {
            cv::Mat im = cv::imread(fn_left[k]);
            if (im.empty()) continue; //only proceed if sucsessful
            // you probably want to do some preprocessing
            all_Img_lft.push_back(im);
            cv::Mat im_right = cv::imread(fn_right[k]);
            if(im_right.empty()) continue;
            all_Img_rgt.push_back(im_right);

        }
        std::cout << all_Img_lft.size() << all_Img_rgt.size() << std::endl;


        cv::Size board_size = cv::Size(board_width, board_height);
        for(unsigned int i = 0; i < all_Img_lft.size(); i++)
        {

            cv::Mat gray,gray2;
            cv::cvtColor(all_Img_lft[i], gray, CV_BGR2GRAY);
            all_Img_Gray_lft.push_back(gray);
            cv::cvtColor(all_Img_rgt[i], gray2, CV_BGR2GRAY);
            all_Img_Gray_rgt.push_back(gray2);

        }
        for (unsigned int k = 0; k < all_Img_lft.size(); k++) {
            bool found1 = false,found2 = false;
            found1 = cv::findChessboardCorners(all_Img_lft[k], board_size, corners1,
                                              CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
            found2 = cv::findChessboardCorners(all_Img_rgt[k], board_size, corners2,
                                              CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
            if (found1)
            {
                cornerSubPix(all_Img_Gray_lft[k], corners1, cv::Size(5, 5), cv::Size(-1, -1),
                             cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
                drawChessboardCorners(all_Img_Gray_lft[k], board_size, corners1, found1);
            }
            if (found2)
            {
                cornerSubPix(all_Img_Gray_rgt[k], corners2, cv::Size(5, 5), cv::Size(-1, -1),
                             cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
                drawChessboardCorners(all_Img_Gray_rgt[k], board_size, corners2, found2);
            }
            std::vector< cv::Point3f > obj;
            if(!obj.empty())
            {
                obj.clear();
            }
            for (int i = 0; i < board_height; i++)
                for (int j = 0; j < board_width; j++)
                    obj.push_back(cv::Point3f((double)j * square_size, (double)i * square_size, 0));

            if (found1 && found2) {
                std::cout << k << ". Found corners!" << std::endl;
                image_points1.push_back(corners1);
                image_points2.push_back(corners2);
                object_points.push_back(obj);
            }
            /*for (int i = 0; i < image_points1.size(); i++) {
                std::vector< cv::Point2f > v1, v2;
                for (int j = 0; j < image_points1[i].size(); j++) {
                  v1.push_back(cv::Point2f((double)image_points1[i][j].x, (double)image_points1[i][j].y));
                  v2.push_back(cv::Point2f((double)image_points2[i][j].x, (double)image_points2[i][j].y));
                }
                left_image_points.push_back(v1);
                right_image_points.push_back(v2);
            }*/
        }









        int flag = 0;
        flag |= CV_CALIB_FIX_INTRINSIC;
        //flag |=CV_CALIB_SAME_FOCAL_LENGTH;
        //flag |=CV_CALIB_ZERO_TANGENT_DIST;
        //flag |= CV_CALIB_FIX_K5;
        cv::stereoCalibrate(object_points, image_points1, image_points1,K1,D1,K2,D2, all_Img_lft[1].size(), R, T, E, F);
        cv::stereoRectify(K1, D1, K2, D2, all_Img_lft[1].size(), R, T, R1, R2, P1, P2, Q);
        cv::initUndistortRectifyMap(K1, D1, R1, P1, all_Img_lft[1].size(), CV_32FC1, map1x, map1y);
        cv::initUndistortRectifyMap(K2, D2, R2, P2, all_Img_lft[1].size(), CV_32FC1, map2x, map2y);
        //cv::calibrateCamera(object_points, image_points1, all_Img[1].size(), K, D, rvecs, tvecs, flag);
        cv::FileStorage fs("calibration.txt", cv::FileStorage::WRITE);
        fs << "MatrixCalib1" << K1;
        fs << "Distorsion1" << D1;
        fs << "MatrixCalib2" << K2;
        fs << "Distorsion2" << D2;
        fs << "FundamentalMatrix" << F;
        fs << "EssentialMatrix" << E;
        fs << "RotationLeftToRight" << R;
        fs << "TranslationLeftToRight" << T;
        fs << "RectificationLeft" << R1;
        fs << "RectificationRight" << R2;
        fs << "ProjectionLeft" << P1;
        fs << "ProjectionRight" << P2;
        fs << "DisparityMap" << Q;
        fs.release();
        printf("Done Calibration\n");

        displayCurrentCalib("calibration.txt");


    }
}

void MainWindow::displayCurrentCalib(QString s)
{

    QFile file(s);
    if(!file.open(QIODevice::ReadOnly)) {
        std::cout << "error" << std::endl;
    }
    QTextStream in(&file);



    QString text;
    while(!in.atEnd()) {
        QString line = in.readLine();
        line.append("\n");
        text.append(line);
    }

    file.close();

    txt->setText(text);
    txt->show();
    layout->addWidget(txt,4,0);
}

void MainWindow::saveCalib()
{
    QString nameCalib = QInputDialog::getText(this,"Entrez Valeur","Nom de votre calibration (sans extension)");
    nameCalib.append(".txt");
    std::string nameCalibBis = nameCalib.toUtf8().constData();
    cv::FileStorage fs(nameCalibBis, cv::FileStorage::WRITE);
    fs << "MatrixCalib1" << K1;
    fs << "Distorsion1" << D1;
    fs << "MatrixCalib2" << K2;
    fs << "Distorsion2" << D2;
    fs << "FundamentalMatrix" << F;
    fs << "EssentialMatrix" << E;
    fs << "RotationLeftToRight" << R;
    fs << "TranslationLeftToRight" << T;
    fs << "RectificationLeft" << R1;
    fs << "RectificationRight" << R2;
    fs << "ProjectionLeft" << P1;
    fs << "ProjectionRight" << P2;
    fs << "DisparityMap" << Q;
    fs.release();
}

void MainWindow::loadCalib()
{
    QString filename =  QFileDialog::getOpenFileName(
                this,
                "Open Calibration File",
                "/home",
                "Texte Files (*.txt *.yml *.xml)");
    if(!filename.isEmpty())
    {
        std::string st = filename.toUtf8().constData();
        cv::FileStorage fs(st, cv::FileStorage::READ);
        fs["MatrixCalib1"] >> K1;
        fs["Distorsion1"] >> D1;
        fs["MatrixCalib2"] >> K2;
        fs["Distorsion2"] >> D2;
        fs["FundamentalMatrix"] >> F;
        fs["EssentialMatrix"] >> E;
        fs["RotationLeftToRight"] >> R;
        fs["TranslationLeftToRight"] >> T;
        fs["RectificationLeft"] >> R1;
        fs["RectificationRight"] >> R2;
        fs["ProjectionLeft"] >> P1;
        fs["ProjectionRight"] >> P2;
        fs["DisparityMap"] >> Q;

        fs.release();
        QString filenameImg =  QFileDialog::getOpenFileName(
                    this,
                    "Ouvrez une image de la taille dont vous voulez calibrer",
                    "/home",
                    "Image (*.png *.jpg *.bmp *.gif *.jpeg)");
        if(!filenameImg.isEmpty())
        {
            std::string st = filenameImg.toUtf8().constData();
            cv::Mat img_size = cv::imread(st);
            cv::initUndistortRectifyMap(K1, D1, R1, P1, img_size.size(), CV_32FC1, map1x, map1y);
            cv::initUndistortRectifyMap(K2, D2, R2, P2, img_size.size(), CV_32FC1, map2x, map2y);
        }

        displayCurrentCalib(filename);
    }

}

void MainWindow::openLeftImg()
{

    cv::Mat imgU1;
    QString filename_lft =  QFileDialog::getOpenFileName(
                this,
                "Open Left Image",
                "/home",
                "Image (*.png *.jpg *.bmp *.gif *.jpeg)");
    if(!filename_lft.isEmpty())
    {
        std::string st_lft = filename_lft.toUtf8().constData();
        img_lft = cv::imread(st_lft);
        //cv::resize(img_lft, img_lft, cv::Size(430, 423), 0, 0, cv::INTER_CUBIC);
        cv::remap(img_lft, imgU1, map1x, map1y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
        /*cv::Mat disp8,disp;

        cv::Ptr<cv::StereoSGBM> sbm = cv::StereoSGBM::create();
                sbm->setP1(24*3*3);
                sbm->setP2(96*3*3);
                sbm->setPreFilterCap(63);
                sbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);



                sbm->compute(all_Img_Gray_lft[1],all_Img_Gray_rgt[1],disp8);
                normalize(disp8, disp, 0, 255, CV_MINMAX, CV_8U);
                imshow("disp",disp);
                imshow("disp8",disp8);*/

        p = QPixmap::fromImage(QImage(imgU1.data, imgU1.cols, imgU1.rows, imgU1.step, QImage::Format_RGB888).rgbSwapped());
        t->setText("Left Image");
        l->setPixmap(p);

        layout->addWidget(t,5,0);
        layout->addWidget(l,6,0);
    }
}

void MainWindow::openRightImg()
{

    cv::Mat imgU2;
    QString filename_rgt = QFileDialog::getOpenFileName(
                this,
                "Open Right Image",
                "/home",
                "Image (*.png *.jpg *.bmp *.gif *.jpeg)");
    if(!filename_rgt.isEmpty())
    {
        std::string st_right = filename_rgt.toUtf8().constData();
        img_right = cv::imread(st_right);
        //cv::resize(img_right, img_right, cv::Size(430, 423), 0, 0, cv::INTER_CUBIC);
        cv::remap(img_right, imgU2, map2x, map2y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
        /*cv::Mat disp8,disp;

        cv::Ptr<cv::StereoSGBM> sbm = cv::StereoSGBM::create();
                sbm->setP1(24*3*3);
                sbm->setP2(96*3*3);
                sbm->setPreFilterCap(63);
                sbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);



                sbm->compute(all_Img_Gray_lft[1],all_Img_Gray_rgt[1],disp8);
                normalize(disp8, disp, 0, 255, CV_MINMAX, CV_8U);
                imshow("disp",disp);
                imshow("disp8",disp8);*/
        p1 = QPixmap::fromImage(QImage(imgU2.data, imgU2.cols, imgU2.rows, imgU2.step, QImage::Format_RGB888).rgbSwapped());
        t1->setText("Right Image");
        l1->setPixmap(p1);
        layout->addWidget(t1,5,1);
        layout->addWidget(l1,6,1);
    }
}
