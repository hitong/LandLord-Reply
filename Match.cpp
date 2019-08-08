#include "Match.h"
#include "Landlord.h"

void Match::analysisData(Data_Ty&& data)
{
	if (data[0] == 'D') {//DEAL A1,2,3....
		players[data[5] - 'A'].insertCards(std::move(getCardsFromData(data.substr(6))));
	}
	else if (data[0] == 'B') {//BID A1
		bidRecodes.emplace_back(std::make_pair(data[4], data[5]));
	}
	else if (data[0] == 'L') {//LEFTOVER C25,41,26
		Cards_Ty bidCards = getCardsFromData(data.substr(10));
		players[data[9] - 'A'].insertCards(bidCards);
		sort(players[data[9] - 'A'].handCards_.begin(), players[data[9] - 'A'].handCards_.end());
		for (auto& card: bidCards) {
			this->bidCards.push_back(card);
		}
		landlord = data[9];
	}
	else if (data[0] == 'P') {//PLAY C2,7,8,14,18,22,27,30
		playRecodes.emplace_back(std::make_pair(data[5] - 'A', std::move(getCardsFromData(data.substr(6)))));
	}
	else if (data[0] == 'G') {//GAMEOVER B
		winer = data[data.size() - 1];
		recodes_idx_cur = 0;
		recodes_idx_end = playRecodes.size();
		//文档读取完毕
	}
	else {
		//其他协议指令暂不支持
	}
}

void Match::reset()
{
	for (auto& player:players) {
		player.reset();
	}
	bidCards.clear();
	bidRecodes.clear();
	playRecodes.clear();
	recodes_idx_cur = -1;
	recodes_idx_end = -1;
	recodes_idx_pre_cur = -1;
	winer = -1;
	landlord = -1;
}


bool Match::preStep()
{
	if (recodes_idx_cur > 0) {
		players[playRecodes[recodes_idx_cur-1].first].undoCards();
		recodes_idx_pre_cur = (recodes_idx_cur---2) / 3 * 3;
		return true;
	} 
	return false;
}

bool Match::nextStep()
{
	if (recodes_idx_cur < recodes_idx_end) {
		players[playRecodes[recodes_idx_cur].first].playCards(playRecodes[recodes_idx_cur].second);
		recodes_idx_pre_cur = (recodes_idx_cur++) / 3 * 3;
		return true;
	}
	return false;
}

void Match::firstStep()
{
	while (preStep());
}

void Match::lastStep()
{
	while (nextStep());
}

Cards_Ty Match::getCardsFromData(Data_Ty&& data)
{
	DataStream_Ty ss;
	Cards_Ty cards;
	int tmp;
	char ch;
	ss << data;
	while (true) {
		ss >> tmp;
		ss >> ch;
		cards.emplace_back(tmp);

		if (ss.eof()) {
			break;
		}
	}
	sort(cards.begin(), cards.end());
	return cards;
}
