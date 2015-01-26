/*
108 - Maximum Sum
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=44
*/
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::ios::sync_with_stdio(false);
	
	std::vector<std::vector<int32_t> > datas;
	uint32_t size_row;
	do
	{
		std::cin >> size_row;
		if(std::cin.eof())
			break;
		
		//Get the data
		datas.reserve(size_row);
		for(auto i = 0U; i < size_row; ++i)
		{
			std::vector<int32_t> row;
			row.reserve(size_row);
			int value;
			for(auto j = 0U; j < size_row; ++j)
			{
				std::cin >> value;
				row.push_back(value);
				if(i)
					row.back() += datas[i - 1][j];
				if(j)
					row.back() += *(row.end()-2);//~datas[i][j-1]
				if(i && j)
					row.back() -= datas[i-1][j-1];
			}
			datas.push_back(std::move(row));
		}

		//Find the maximum!
		int32_t current_maximum { -1270001 }, tested_maximum {};
		for(auto i = 0U; i < size_row; ++i)
		{
			for(auto j = 0U; j < size_row; ++j)
			{
				for(auto m = i; m < size_row; ++m)
				{
					for(auto n = j; n < size_row; ++n)
					{
						tested_maximum = datas[m][n];
						if(i)
							tested_maximum -= datas[i-1][n];
						if(j)
							tested_maximum -= datas[m][j-1];
						if(j && i)
							tested_maximum += datas[i-1][j-1];
						if(tested_maximum > current_maximum)
							current_maximum = tested_maximum;
					}
				}
			}
		}

		//Displays result
		std::cout << current_maximum << std::endl;

		//Clear datas for next iteration
		datas.clear();
	} while(1);

	return 0;
}