#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QToolButton>
#include <QMouseEvent>
#include <QGraphicsBlurEffect>
#include <QFont>
#include <QFontDatabase>
#include <stdio.h>
#include <cstdio>
#include <QUrl>
#include <string>
#include <iostream>
#include <sstream>
#include <QDebug>
#include <QSettings>
#include <windows.h>
#include <wininet.h>
#include <stdlib.h>
#include <QTimer>
#include <QSplashScreen>
#include <QDesktopServices>
#include <urlmon.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <WinNt.h>
#include "includes.h"
#include <QtCore>
#include <QDesktopWidget>
#include <QDateTime>
#include <cstdlib>
#include <winuser.h>
#include <conio.h>
#include <QFile>
#include <QProcess>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

//Herhangi bir değişiklik yaptığında yapman gereken Build butonuna tıklayıp rebuild all demen.
//Daha sonra exe release klasörüne gelecek
//Bu saatten sonra exe hazır demek fakat exenin çalışması için gerekli bazı şeyler var?
//Bu dosyalar olmadan exe çalışmaz ona dikkat et
//Şuanda kullanıma hazır bu kadar :D build edermisin buldum roblemi :D hadi inşallah
//Bu kadar :D kb az 55 mb tı herşey tamam dimi* anlamadığın bişi vrmı ? exe olarak her zaman release klasörnde ki exeyi kullanıcan tamam anladım herşeyi ama build ederken 150 kb cıkarıyor normalde 55 mb  çünkü
//Ben gerekli dll leri birleştiriyorum
//Dikkatli izle şimdi build ettin exe çıktı
//Çıkacak olan exenin ismini istediin şey yapabilirsin ikinci satırdaki
//Daha sonra add butonuna tıklıyorsun
//Add files - tüm dll leri seçip ok butonuna basıyorum - daha sonra process butonuna basıorum- bekliyorum daha sonra close butonuna basıyorum ve exe hazır eksik dll eklemişiz :D


//#pragma comment(lib, "urlmon.lib")
//#pragma comment(lib,"wininet.lib")
//RC_ICONS = clienticon.ico
//RC_ICONS = clienticon.ico

QByteArray MainWindow::QResolveUrl(QString URL) {
    QNetworkAccessManager LManager;
      QNetworkReply* reply = LManager.get(QNetworkRequest(QUrl(URL)));
      QEventLoop event;
      connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
      event.exec();
      return reply->readAll();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    QString buyukbaslik = "https://gitlab.com/fural007/autoupdate/raw/master/buyukbaslik";
    QString buyukbasliksonuc = QResolveUrl(buyukbaslik);

    ui->label_3->setText(buyukbasliksonuc);

    QString kucukbaslik = "https://gitlab.com/fural007/autoupdate/raw/master/kucukbaslik";
    QString kucukbasliksonuc = QResolveUrl(kucukbaslik);

    ui->label_4->setText(kucukbasliksonuc);

    ui->pushButton->setVisible(false);

    QTimer::singleShot(2000, this, SLOT(myfunction()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString newupdateurl = "https://gitlab.com/fural007/autoupdate/-/raw/master/yedekurlupload"; //Gitlap verilerininç ekildiği yer
    QString newupdateurlsonuc = QResolveUrl(newupdateurl);

    if(newupdateurlsonuc == NULL){
        ui->label->setText("Baglantı sorunu!"); //Bağlantı sorunu var ise çıkan yazı
    }else{

        ui->label->setText("Dosyalar kontrol ediliyor!"); //Dosyalar ilk indirildiğinde çıkan yazı

        QNetworkAccessManager LManager;
          QNetworkReply* reply = LManager.get(QNetworkRequest(QUrl(newupdateurlsonuc)));
          QEventLoop event;
      connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
      connect(reply, &QNetworkReply::downloadProgress, this, [this, reply]( qint64 ist, qint64 max) {
        ui->progressBar->setRange(0, max);
        ui->progressBar->setValue(ist);
        QString sA = QString::number((double) ist/1024/1024);
    ui->label_2->setText(sA + " MB");

      });
      event.exec();

      ui->label->setText("Dosyalar çıkartılıyor!"); //Dosyalar çıkartılırken çıkan yazı

      QString dirpath = qApp->applicationDirPath();

                  QFile file(dirpath +"/updated.zip"); //Dokunma!
                      if (!file.open(QIODevice::WriteOnly))
                          return;
                      file.write(reply->readAll());
                      file.close();

                      const QString program = dirpath +"/unzip.exe"; //Dokunma!

                      const QStringList arguments = QStringList() << "-oq" << "-d" << dirpath << dirpath +"/updated.zip";
                      QProcess process;
                      process.setWorkingDirectory(dirpath);
                      process.start(program, arguments);
                      process.waitForFinished(-1);
                      qDebug() << process.readAllStandardOutput();


                      QString extractedzip = dirpath +"/updated.zip"; //Dokunma!

                      QFile::remove(extractedzip);

                      ui->label->setText("Dosya Kontrolü Başarıyla Tamamlandı!"); //Burada güncelleme bitince yazan yazı

                      QString program23 = dirpath +"/Aefnet.exe"; //Burası yol ve başlaması gereken exe mesela resourceste başlatacaksan /Resources/exeismi.exe eğer kendi olduğu dizinde başlayacaksa sadece /exismi.exe yazman yeterli
                      QStringList arguments23;

                      QProcess *process23 = new QProcess(this);
                      process23->start(program23, arguments23);




                      ExitProcess(0);
                      exit(-1);
                      TerminateProcess(GetCurrentProcess(), TRUE);

    }
}

void MainWindow::myfunction()
{
 ui->pushButton->click();
}

void MainWindow::on_pushButton_2_clicked()
{


}
