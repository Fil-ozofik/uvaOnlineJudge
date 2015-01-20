/*
101 - The Blocks Problem
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=37
*/
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using Blocks = std::vector<std::vector<uint32_t> >;

void put_on_origin(Blocks& blocks, uint32_t value)
{
	blocks[value].insert(blocks[value].begin(), value);
}

void empty_top(Blocks& blocks, uint32_t value) 
{
	for(auto& pile : blocks)
	{
		auto found = std::find(pile.begin(), pile.end(), value);
		if(found != pile.end())
		{
			//put back element on top of and delete them
			std::for_each(found+1, pile.end(), [&blocks](uint32_t v) { put_on_origin(blocks, v); });
			pile.erase(found+1, pile.end());
			return;
		}
	}
}

bool on_same_pile(const Blocks& blocks, uint32_t from, uint32_t to)
{
	for(const auto& from_pile : blocks)
	{
		auto from_loc = std::find(from_pile.cbegin(), from_pile.cend(), from);
		if(from_loc != from_pile.cend())
		{
			//checks whether from is on the same pile as to (no-action case)
			return std::find(from_pile.cbegin(), from_pile.cend(), to) != from_pile.cend();
		}
	}
	return false;
}

void operate(bool move, Blocks& blocks, uint32_t from, uint32_t to, bool onto)
{
	if(on_same_pile(blocks, from, to))
		return;
	if(onto)
		empty_top(blocks, to);

	for(auto& from_pile : blocks)
	{
		auto from_loc = std::find(from_pile.begin(), from_pile.end(), from);
		if(from_loc != from_pile.end())
		{
			if(move)
			{
				//put back element on top of 'from' and delete them
				std::for_each(from_loc+1, from_pile.end(), [&blocks](uint32_t v) { put_on_origin(blocks, v); });
				from_pile.erase(from_loc, from_pile.end());//DOES erase from!
			}

			//put from on 'to':
			for(auto& to_pile : blocks)
			{
				auto to_loc = std::find(to_pile.begin(), to_pile.end(), to);
				if(to_loc != to_pile.end())
				{
					if(move)
					{
						to_pile.insert(to_pile.end(), from);
					}
					else
					{
						to_pile.insert(to_pile.end(), from_loc, from_pile.end());
						from_pile.erase(from_loc, from_pile.end());
					}
					return;
				}
			}
		}
	}
}

int main(void)
{
	std::ios::sync_with_stdio(false);
	uint32_t amount_blocks, from, to;
	std::cin >> amount_blocks;

	//Sets the blocks
	Blocks blocks;
	blocks.reserve(amount_blocks);
	for(auto i = 0; i < amount_blocks; ++i)
	{
		blocks.emplace_back(1, i);
		blocks.back().reserve(amount_blocks);
	}

	//Performs operations
	std::string operation, position;
	do
	{
		std::cin >> operation;
		if(std::cin.eof() || operation[0]=='q')
			break;
		std::cin >> from;
		std::cin >> position;
		std::cin >> to;

		if(from != to)
			operate(operation[0]=='m', blocks, from, to, position[1]=='n');
	} while(1);
	
	//Displays the results
	for(auto i = 0; i < amount_blocks; ++i)
	{
		std::cout << i << ":";
		if(!blocks[i].empty())
		{
			std::cout << " ";
			for(int j = 0, end_j = blocks[i].size(); j < end_j-1; ++j)
				std::cout << blocks[i][j] << " ";
			std::cout << blocks[i].back() << std::endl;
		}
		else
			std::cout << std::endl;
	}

    return 0;
}

