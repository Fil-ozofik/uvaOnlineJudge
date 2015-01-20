/*
102 - Ecological Bin Packing
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=38
*/
#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <algorithm>

int main(void)
{
	std::ios::sync_with_stdio(false);

	std::array<uint32_t, 9> bin_slots;//b0 g0 c0 b1 g1 c1 b2 g2 c2 
	std::array<uint32_t, 6> answers;
	std::array<std::string, 6> string_answers = { "BCG", "BGC", "CBG", "CGB", "GBC", "GCB" };

	do
	{
		std::cin >> bin_slots[0];
		if(std::cin.eof())
			break;
		for(uint8_t i = 1; i < 9; ++i)
			std::cin >> bin_slots[i];

		answers[0] = bin_slots[3]+bin_slots[6] + bin_slots[2]+bin_slots[8] + bin_slots[1]+bin_slots[4];//BCG
		answers[1] = bin_slots[3]+bin_slots[6] + bin_slots[1]+bin_slots[7] + bin_slots[2]+bin_slots[5];//BGC
		answers[2] = bin_slots[5]+bin_slots[8] + bin_slots[0]+bin_slots[6] + bin_slots[1]+bin_slots[4];//CBG
		answers[3] = bin_slots[5]+bin_slots[8] + bin_slots[1]+bin_slots[7] + bin_slots[0]+bin_slots[3];//CGB
		answers[4] = bin_slots[4]+bin_slots[7] + bin_slots[0]+bin_slots[6] + bin_slots[2]+bin_slots[5];//GBC
		answers[5] = bin_slots[4]+bin_slots[7] + bin_slots[2]+bin_slots[8] + bin_slots[0]+bin_slots[3];//GCB

		const auto& it = std::min_element(answers.cbegin(), answers.cend());
		std::cout << string_answers[std::distance(answers.cbegin(), it)] << " " << *it << std::endl;
	} while(1);
}
