#pragma once
#include <string>
#include <vector>
#include <qdebug.h>
#include <sstream>
using Data_Ty = std::string;
using DataStream_Ty = std::stringstream;
using Cards_Ty = std::vector<int>;
using Recode_Ty = std::pair<int, Cards_Ty>;
using BidRecode = std::pair<char, int>;
using Vec_BidRecode = std::vector<BidRecode>;
using Vec_Recode_Ty = std::vector<Recode_Ty>;
using Vec_Cards_Ty = std::vector<Cards_Ty>;
