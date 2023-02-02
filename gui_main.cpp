#include <QtCore>
#include <QtGui>
#include <string>
 
class MainWindow : public QWidget
{
 public:
   MainWindow();
 
 private:
   void infoMsg();
   QString printe_statistik(QTime anwesenheit, QTime pausenzeit);

   void stemple_ein();
   void stemple_aus();

   QTime einstempelzeit= {0,0,0};
   QTime anwesenheit = {0,0,0};
   QTime ausstempelzeit={0,0,0};
   QTime pausenzeit = {0,0,0};
   QTime feierabend = {7,36,0};

   QTimeEdit *currentTime;
   QLineEdit *info_text;
};

MainWindow::MainWindow()
{
   setMinimumSize(200, 300);

   QLabel *currentLabel = new QLabel("Uhrzeit:");
   currentTime = new QTimeEdit();
   currentTime->setDisplayFormat("hh:mm");

   QPushButton *pb_1 = new QPushButton();
   pb_1->setText("Close");

   QPushButton *kommen_b = new QPushButton();
   kommen_b->setText("Kommen");

   QPushButton *gehen_b = new QPushButton();
   gehen_b->setText("Gehen");

   QPushButton *info_b = new QPushButton();
   info_b->setText("Info");

   QString filename = "homer.png";
   QLabel* lbl = new QLabel(this);
   /** set content to show center in label */
   lbl->setAlignment(Qt::AlignCenter);
   QPixmap pix;

    /** to check wether load ok */
    if(pix.load(filename)){
        /** scale pixmap to fit in label'size and keep ratio of pixmap */
        //pix = pix.scaled(lbl->size(),Qt::KeepAspectRatio);
        lbl->setPixmap(pix);
    }
 
   QGridLayout *grid1 = new QGridLayout();
   grid1->setContentsMargins(75, 45, 75, 25);   
 
   grid1->addItem(new QSpacerItem(0, 25), 1, 0, 1, 4);
   grid1->addWidget(currentLabel, 4, 1);
   grid1->addWidget(currentTime, 4, 2);
 
   QHBoxLayout *layout1 = new QHBoxLayout();
   layout1->addStretch();
   layout1->addWidget(kommen_b);
   layout1->addStretch();
   layout1->addWidget(gehen_b);
   layout1->addStretch();
   layout1->addWidget(info_b);
   layout1->addStretch();
   layout1->addWidget(pb_1);
   layout1->addStretch();

   QHBoxLayout *layout2 = new QHBoxLayout();
   layout2->addWidget(lbl);
 
   QVBoxLayout *layoutMain = new QVBoxLayout(this);
   layoutMain->addLayout(layout2);
   layoutMain->addSpacing(5);
   layoutMain->addLayout(grid1);
   layoutMain->addSpacing(5);
   layoutMain->addLayout(layout1);

   //Einstempeln
   QObject::connect(kommen_b, &QPushButton::clicked, this, &MainWindow::stemple_ein);

   //Ausstempeln
   QObject::connect(gehen_b, &QPushButton::clicked, this, &MainWindow::stemple_aus);

   //Schließt das Fenster
   QObject::connect(pb_1, &QPushButton::clicked, this, &QWidget::close);


   //Öffnet die Infofunktion
   QObject::connect(info_b,  &QPushButton::clicked, 
         this, &MainWindow::infoMsg);
}

void MainWindow::stemple_ein(){
   QMessageBox::StandardButton reply;
   QTime time = currentTime->time();
   einstempelzeit = time;
   if(anwesenheit.hour()!=0 || anwesenheit.minute()!=0){
      pausenzeit = pausenzeit.addSecs(ausstempelzeit.secsTo(einstempelzeit));
   }
   else{
      pausenzeit = {0,0,0};
   }
   reply = QMessageBox::information(this, "Information", 
      "Gebucht um: " + einstempelzeit.toString()+ "\nArbeitszeit von: " + anwesenheit.toString()+
      "\nPausenzeit von: " + pausenzeit.toString()
   );
}

void MainWindow::stemple_aus(){
   QMessageBox::StandardButton reply;
   QTime time = currentTime->time();
   ausstempelzeit = time;
   if(einstempelzeit.hour()==0 && einstempelzeit.minute()==0){
      anwesenheit = {0,0,0};
   }
   else{
      anwesenheit = anwesenheit.addSecs(einstempelzeit.secsTo(ausstempelzeit));
   }
   reply = QMessageBox::information(this, "Information", 
      "Gebucht um: " + einstempelzeit.toString()+ "\nArbeitszeit von: " + anwesenheit.toString()+
      "\nPausenzeit von: " + pausenzeit.toString()
   );
}

void MainWindow::infoMsg()
{
   QMessageBox::StandardButton reply;
   /*Fallunterscheidung:
   Wenn zuletzt eingestempelt wurde, wird die Arbeitszeit bis jetzt hochgerechnet
   Wenn zuletzt ausgestempelt wurde, wird die Arbeitszeit einfach nur gedruckt mit dem Text, dass man
   ausgestempelt ist*/
   QTime now = QTime::currentTime();
   if (einstempelzeit > ausstempelzeit){
      QTime anw_temp = anwesenheit.addSecs(einstempelzeit.secsTo(now));
      QTime restZeit {0,0,0};
      restZeit= restZeit.addSecs(anw_temp.secsTo(feierabend));
      if(anw_temp < feierabend){
         reply = QMessageBox::information(this, "Information",
         "Arbeitszeit um: " + now.toString() + "\nArbeitszeit von: " + anw_temp.toString()+
         "\nPausenzeit von: " + pausenzeit.toString() +
         "\nFeierabend in: " + restZeit.toString()
         );
      }
      else{
         reply = QMessageBox::information(this, "Information",
         "Arbeitszeit um: " + now.toString() + "\nArbeitszeit von: " + anw_temp.toString()+
         "\nPausenzeit von: " + pausenzeit.toString() +
         "\nDobby is a free elf now");    
      }
   }
   else{
      reply = QMessageBox::information(this, "Information",
      "Arbeitszeit um: " + now.toString() + "\nArbeitszeit von: " + anwesenheit.toString()+
      "\nDu bist ausgestempelt!");    
   }
}

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
 
   MainWindow *window = new MainWindow();
   window->show();
 
   return app.exec();
}