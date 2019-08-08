#include "Player.h"


void Player::playCards(Cards_Ty& cards)
{
	playRecode_.emplace_back(cards);
	auto start = std::set_difference(
		handCards_.begin(), handCards_.end(), cards.begin(), cards.end(), 
		handCards_.begin());
	handCards_.erase(start, handCards_.end());
}

void Player::undoCards()
{
	if (playRecode_.empty()) {
		return;
	}
	else {
		Cards_Ty&& tmp = std::move(playRecode_[playRecode_.size() - 1]);
		if (tmp[0] != -1) {
			Cards_Ty cards;
			auto start = set_union(handCards_.begin(), handCards_.end(), tmp.begin(), tmp.end(),std::back_inserter(cards));
			handCards_.resize(cards.size(), 0);
			copy(cards.begin(), cards.end(), handCards_.begin());
		}
		playRecode_.pop_back();
	}
}

void Player::insertCards(Cards_Ty&& cards)
{
	for (auto&& v: cards) {
		handCards_.emplace_back(std::move(v));
	}
}

void Player::insertCards(Cards_Ty& cards)
{
	for (auto&& v : cards) {
		handCards_.emplace_back(std::move(v));
	}
}


void Player::reset()
{
	handCards_.clear();
	playRecode_.clear();
}
