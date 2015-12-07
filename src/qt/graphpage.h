#ifndef GRAPHPAGE_H
#define GRAPHPAGE_H
#include <QWidget>
#include <QNetworkReply>
#include <QWidget>
#include "qcustomplot.h"
#include <QtGui/QLabel>
namespace Ui {
    class GraphPage;
}

class GraphPage : public QWidget
{
    Q_OBJECT

public:
    explicit GraphPage(QWidget *parent = 0);
    ~GraphPage();
    // CryptoMP - Get EUR,USD,BTC Value
    void requestData();

public slots:
  //  void setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance);
    // CryptoMP - Get EUR,USD,BTC Value
    void requestReceived(QNetworkReply *reply);
    void updateFiat();
    void onPtceur();
    void onPtcusd();
    void onOneday();
    void onSevenday();
    void onFtday();

private:
    Ui::GraphPage *ui;
   // QCustomPlot *customPlot;
    QLabel *labelFiat;
    QLabel *labelFiatlbl;
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;
    // CryptoMP - EUR,USD,BTC Value
    float currentEuroExchange, oldEuroExchange;
    float currentUsdExchange, oldUsdExchange;
    QNetworkAccessManager *m_networkManager;
    QTimer *timer;
    int m_currency;
    int m_interval;

private slots:
    void updateDisplayUnit();
    void updateAlerts(const QString &warnings);





};

#endif // GRAPHPAGE_H
