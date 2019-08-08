#pragma once
#include "Ty.h"
#include <QWidget>
#include <algorithm>
#include <QLabel>
class Player
{
public:
	void playCards(Cards_Ty& cards);
	void undoCards();
	void insertCards(Cards_Ty&& cards);
	void insertCards(Cards_Ty& cards);
	void reset();
	Cards_Ty handCards_;
	Vec_Cards_Ty playRecode_;//´òÅÆ¼ÇÂ¼
	QWidget* cards_widget;
	QWidget* table_widget;
	QLabel* arror;
	Data_Ty name;
};

