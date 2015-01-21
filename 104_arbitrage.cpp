/*
104 - Arbitrage
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=40
*/
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

using ConversionTable = std::vector<std::vector<float> >;

class Path
{
public:
	//The ids along the path
	std::vector<uint32_t> ids;
	//The length for longest path calculation (used later on)
	float current_value { 1.f };

	//Constructor for starting, meaningless, paths
	Path(uint32_t id) : ids(1, id) {}
	//Constructor for starting, meaningless, paths
	Path(const Path& from_path, uint32_t id, float multiplier) : ids(from_path.ids), current_value(from_path.current_value * multiplier) 
	{
		ids.push_back(id);
	}

	//Checks whether the path leads to profit for its user
	bool profit() const noexcept
	{
		return ids.size() > 1 && ids.front()==ids.back() && current_value > 1.01f;
	}

	void display() const noexcept
	{
		for(uint32_t i = 0, end = ids.size()-1; i < end; ++i) {
			std::cout << ids[i]+1 << " ";
		}
		std::cout << ids.back()+1 << std::endl;
	}
};

bool expand_and_check_paths(const ConversionTable& table, std::vector<Path>& paths)
{
	const std::vector<Path> previous { paths };
	paths.clear();
	for(const auto& path : previous)
	{
		uint32_t last = path.ids.back();
		for(uint32_t i = 0U, end = table[last].size(); i < end; ++i)
		{
			if(i != last)
			{

				Path new_path { path, i, table[last][i] };
				if(new_path.profit())
				{
					new_path.display();
					return true;
				}
				else if(new_path.ids.size()==2 || new_path.current_value > table[new_path.ids.front()][new_path.ids.back()])
					paths.push_back(std::move(new_path));
			}
		}
	}
	return false;
}

int main(void)
{
	std::ios::sync_with_stdio(false);
	
	uint32_t amount_currencies;
	do
	{
		//Gets inputs
		std::cin >> amount_currencies;
		if(std::cin.eof())
			break;
		ConversionTable table;
		table.reserve(amount_currencies);

		std::vector<float> conversion_row;
		float conversion_value;
		for(uint32_t i = 0U; i < amount_currencies; ++i)
		{
			conversion_row.reserve(amount_currencies);
			for(uint32_t j = 0U; j < amount_currencies; ++j)
			{
				if(i==j)
					conversion_row.push_back(1.f);
				else
				{
					std::cin >> conversion_value;
					conversion_row.push_back(conversion_value);
				}
			}
			table.push_back(std::move(conversion_row));
		}

		//Constructs the first meaningless paths
		std::vector<Path> paths;
		paths.reserve(amount_currencies);
		for(uint32_t i = 0U; i < amount_currencies; ++i)
			paths.emplace_back(i);

		bool displayed_answer { false };
		for(uint32_t i = 0U; i < amount_currencies && !displayed_answer; ++i)
		{
			if(expand_and_check_paths(table, paths))
				displayed_answer = true;
			else
				std::sort(paths.begin(), paths.end(), [](const Path& lhs, const Path& rhs) { return lhs.current_value > rhs.current_value; });
		}		
		if(!displayed_answer)
			std::cout << "no arbitrage sequence exists" << std::endl;
	} while(1);
}