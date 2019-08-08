#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include "ui_Landlord.h"
#include "Player.h"
#include <QFile>
#include <QTextStream>
#include "Match.h"
#include <QMouseEvent>
#include <QTimer>
#include <QDropEvent>
#include <QPushButton>
#include <QUrl>
#include <QList>
#include <chrono>
#include <QMimeData>
class Landlord : public QMainWindow
{
	Q_OBJECT

public:
	Landlord(QWidget *parent = Q_NULLPTR);
	enum {READY,STOP,BAD,RUNNING};
	bool choisePlayDir();
	bool choiseFile();
	void showFileMenu();
	void disableFileAction(bool set);
	void disableDirAction(bool set);
	void showDirMenu();
	void delTimer();
	void showAll();
	void playGame();
	void stopPlay();
	void clearAll();
	void readAndDelFile(QFile& file);
	void drwaCard(const int cardValue, int x, QWidget* w);
	void showCards();
	void showBid();
	void showNextStep();
	void showPreStep();
	void showBidCards();
	void showCardsOnTable();
	void firstStep();
	void lastStep();
	void mousePressEvent(QMouseEvent* e);
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);

private:
	QWidget* createNewWidget(QWidget* widget);

	int CardGap = 20;
	int CardWeight = 90;
	int CardHigh = 120;
	QWidget* bidCard;
	Ui::LandlordClass ui;
	Match match;
	std::vector<QWidget*> bidList;
	std::vector<QString> dirFile;
	int play_stat = BAD;
	int dir_idx = 0;
	QTimer* timer = nullptr;
	QAction* openDir = nullptr;
	QAction* openAction = nullptr;
	QAction* beginAction = nullptr;
	QAction* lastAction = nullptr;
	QAction* nextAction = nullptr;
	QAction* endAction = nullptr;
	QAction* speedF = nullptr;
	QAction* speedS = nullptr;
};
