#pragma once
#include "Player.h"
#include "Ty.h"
class Landlord;
class Match
{

public:
	void analysisData(Data_Ty&& data);
	void reset();
	bool preStep();
	bool nextStep();
	void firstStep();
	void lastStep();
	Cards_Ty getCardsFromData(Data_Ty&& data);
	Player players[3];
	Vec_BidRecode bidRecodes;
	Vec_Recode_Ty playRecodes;
	Cards_Ty bidCards;
	int recodes_idx_cur = -1;
	int recodes_idx_end = -1;
	int recodes_idx_pre_cur = -1;
	int winer = -1;//暂时有问题
	char landlord = -1;
	Data_Ty name[2]{"",""};//
};

