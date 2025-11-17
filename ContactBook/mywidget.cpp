#include "mywidget.h"
#include "ui_mywidget.h"
#include <QMessageBox>
#include<QFile>
#include<QDebug>
#include<QTextStream>
#include<QFileDialog>
QString mFilename = "C:/test/myfile.txt";

void Write(QString Filename, QString str){

    QFile mFile(Filename);

    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out<<str;
    mFile.flush();
    mFile.close();
}
QString Read(QString Filename){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"cou21ld not open file for read";
        return "";
    }

    QTextStream in(&mFile);
    QString text = in.readAll();
    mFile.close();
    return text;
}
MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    QStringList ColTotle;
    ui->tableWidget->setColumnCount(4);
    ColTotle<<QStringLiteral("學號")<<QStringLiteral("班級")<<QStringLiteral("姓名")<<QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}


void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;
    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
}


void MyWidget::on_pushButton_2_clicked()
{
    // Show save file dialog
    QString filename = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("另存新檔"),
                                                    mFilename,
                                                    "Text Files (*.txt);;All Files (*)");

    // If user cancelled the dialog, return
    if(filename.isEmpty()){
        return;
    }

    QString saveFile="";

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            saveFile+=(j==0)?"":",";
            saveFile+=ui->tableWidget->item(i,j)->text();
        }
        saveFile+="\n";
    }
    Write(filename,saveFile);
}

void MyWidget::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), "", tr("CSV Files (*.csv);;All Files (*)"));

    if (fileName.isEmpty()) {
        qDebug() << "No file selected.";
        return;
    }

    // 讀取檔案
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open the file for reading."));
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    if (text.isEmpty()) {
        qDebug() << "No data to import or file is empty.";
        return;
    }

    // 清空現有的表格資料
    ui->tableWidget->setRowCount(0);

    // 按行分割文本
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        // 按逗號分割每一行
        QStringList fields = line.split(",");

        if (fields.size() == 4) { // 確保每行有 4 個欄位
            // 新增一行
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // 為每個欄位設置資料
            for (int i = 0; i < 4; i++) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
}


void MyWidget::on_pushButton_4_clicked()
{
    // Save data first
    on_pushButton_clicked();
    // Then close the application
    close();
    QString text = Read(mFilename);
    if(text.isEmpty()){
        qDebug()<<"No data to import or file is empty";
        return;
    }

    // Clear existing table data
    ui->tableWidget->setRowCount(0);

    // Split by lines
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for(const QString &line : lines){
        // Split by comma
        QStringList fields = line.split(",");

        if(fields.size() == 4){
            // Add a new row
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // Set the data for each column
            for(int i = 0; i < 4; i++){
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
}

