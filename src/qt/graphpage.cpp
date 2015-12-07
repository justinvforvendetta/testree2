#include "graphpage.h"
#include "ui_graphpage.h"

GraphPage::GraphPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphPage)
{


    ui->customPlot = new QCustomPlot();
     ui->customPlot->setObjectName(QString::fromUtf8("customPlot"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(ui->customPlot->sizePolicy().hasHeightForWidth());
     ui->customPlot->setSizePolicy(sizePolicy1);
     ui->customPlot->setMinimumSize(QSize(0, 120));
     ui->customPlot->setBaseSize(QSize(0, 0));
     ui->customPlot->setStyleSheet(QString::fromUtf8(""));

    ui->setupUi(this);


    // CryptoMP - Get Values
   // m_currency=0;
  //  m_interval=0;
    requestData();
    // CryptoMP - Set Ticker Update Timer (5 Minutes)
 /*   timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(updateFiat()));
         timer->start(300000);*/

    // CryptoMP - Connect Buttons
    QObject::connect(ui->bptceur, SIGNAL(clicked()), this, SLOT(onPtceur()));
    QObject::connect(ui->bptcusd, SIGNAL(clicked()), this, SLOT(onPtcusd()));
    QObject::connect(ui->boneday, SIGNAL(clicked()), this, SLOT(onOneday()));
    QObject::connect(ui->bsevenday, SIGNAL(clicked()), this, SLOT(onSevenday()));
    QObject::connect(ui->bftday, SIGNAL(clicked()), this, SLOT(onFtday()));
}


GraphPage::~GraphPage()
{
    delete ui;
}

void GraphPage::updateAlerts(const QString &warnings)
{
    return;
}
// CryptoMP - On received data
void GraphPage::requestReceived(QNetworkReply *reply){

    if (reply->error()) {
        labelFiatlbl->setText("Error");
    }
    else {
        QByteArray bytes = reply->readAll();
        QString replyText (bytes);

        QStringList values = replyText.split("[]");
        QStringList last =  replyText.split("[v]");
        QString euro = last.value( 1 );
        QString usd = last.value( 2 );

        currentEuroExchange = euro.toFloat();
        currentUsdExchange = usd.toFloat();

        // uptrend or downtrend?? if equal no color change...
        if (currentEuroExchange > oldEuroExchange)
            labelFiatlbl->setStyleSheet("QLabel { color : green; }");
        else if (currentEuroExchange < oldEuroExchange)
            labelFiatlbl->setStyleSheet("QLabel { color : red; }");

        // CryptoMP - FIAT Values Update
        QString OutputStr;
        float fullBalance = currentBalance / 100000000;
        OutputStr.sprintf("%.2f€/$%.2f", (fullBalance * currentEuroExchange), (fullBalance * currentUsdExchange));
        labelFiatlbl->setText(OutputStr);

        QDateTime datetime = QDateTime::currentDateTime();
        uint theDate = datetime.toTime_t();

       float lmin=1, lmax=0;
        // Draw Plot
        QVector<double> x(720), y(720);

        if (m_interval == 0) {

            for (int i=0; i<719; i++)
            {
            x[i] = theDate - ((720-i)*120);
            QString euroVal = values.value( i+1 );
            y[i] = euroVal.toFloat();

            if (lmax < y[i]) lmax = y[i];
            if (lmin > y[i]) lmin = y[i];
            }
        }
        else if (m_interval == 1) {

            for (int i=0; i<499; i++)
            {
            x[i] = theDate - ((500-i)*1200);
            QString euroVal = values.value( i+1 );
            y[i] = euroVal.toFloat();

            if (lmax < y[i]) lmax = y[i];
            if (lmin > y[i]) lmin = y[i];
            }
        }
        else if (m_interval == 2) {

            for (int i=0; i<499; i++)
            {
            x[i] = theDate - ((500-i)*2400);
            QString euroVal = values.value( i+1 );
            y[i] = euroVal.toFloat();

            if (lmax < y[i]) lmax = y[i];
            if (lmin > y[i]) lmin = y[i];
            }
        }

        // create graph and assign data to it:
        QLinearGradient lgrad(ui->customPlot->rect().topLeft(),ui->customPlot->rect().bottomLeft());
        lgrad.setColorAt(0.0, QColor(255,80,0));
        lgrad.setColorAt(1.0, QColor(255,150,0));

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(x,y);
        ui->customPlot->graph(0)->setPen(QPen(Qt::red, 2));
        ui->customPlot->graph(0)->setBrush(QBrush(lgrad));
        // give the axes some labels:
        //ui->customPlot->xAxis->setLabel("Valor Ultimos 7 Días / Last 7 Days Value");

        if (m_currency == 0)
            ui->customPlot->yAxis->setLabel("PTC/EUR");
        else
            ui->customPlot->yAxis->setLabel("PTC/USD");
        // set axes ranges, so we see all data:
        // configure bottom axis to show date and time instead of number:
        ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        ui->customPlot->xAxis->setDateTimeFormat("dd/MMM");


        if (m_interval == 0) {
            ui->customPlot->xAxis->setRange(theDate-86400, theDate);
            ui->customPlot->xAxis->setDateTimeFormat("hh:mm (ddd)");
        }
        else if (m_interval == 1)
            ui->customPlot->xAxis->setRange(theDate-600000, theDate);
        else if (m_interval == 2)
           ui->customPlot->xAxis->setRange(theDate-1200000, theDate);

        ui->customPlot->yAxis->setRange(lmin, lmax);


        ui->customPlot->setBackground( QBrush((QColor(240,240,240))) );
        ui->customPlot->replot();

        // Cleanup
        reply->deleteLater();
    }
}

void GraphPage::updateDisplayUnit()
{
   /* if(walletModel && walletModel->getOptionsModel())
    {
      //  if(currentBalance != -1)
       //     setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance);

        // Update txdelegate->unit with the current unit
        //txdelegate->unit = walletModel->getOptionsModel()->getDisplayUnit();

      //  ui->listTransactions->update();
    } */
    return;
}

void GraphPage::requestData(){
    QNetworkRequest request;
    QString theURL;
    theURL.sprintf("http://www.xxxxxx.info/graph-wallet-new.php?mkt=%d&int=%d", 0, 0);
    request.setUrl(QUrl(theURL));


    m_networkManager = new QNetworkAccessManager(this);

    connect(m_networkManager,SIGNAL(finished(QNetworkReply*)),this,
            SLOT(requestReceived(QNetworkReply*)));

    // store actual values
    oldEuroExchange = currentEuroExchange;
    oldUsdExchange = currentUsdExchange;

    return;
    // request data
    m_networkManager->get(request);
    //return;
}

void GraphPage::updateFiat()
{
    requestData();
}


void GraphPage::onPtceur()
{
    m_currency = 0;
    requestData();
}


void GraphPage::onPtcusd()
{
    m_currency = 1;
    requestData();
}

void GraphPage::onOneday()
{
    m_interval = 0;
    requestData();
}

void GraphPage::onSevenday()
{
    m_interval = 1;
    requestData();
}

void GraphPage::onFtday()
{
    m_interval = 2;
    requestData();
}

