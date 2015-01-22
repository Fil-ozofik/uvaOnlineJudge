/*
105 - The Skyline Problem
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=41
*/
#include <cstdint>
#include <iostream>

int main(void)
{
	std::ios::sync_with_stdio(false);

	//Gets input and initialization of the process
	uint32_t left, height, right, max_right = 1U;
	uint32_t heights[10001] = {0U};
	while(1)
	{
		std::cin >> left;
		if(std::cin.eof())
			break;
		std::cin >> height;
		std::cin >> right;
		if(max_right < right)
			max_right = right;

		for(uint32_t i = left; i < right; ++i)
		{
			if(heights[i] < height)
				heights[i] = height;
		}
	};
	uint32_t current_height { heights[1] };
	std::cout << 1 << " " << current_height;
	for(uint32_t i = 1; i < max_right; ++i)
	{
		if(heights[i] != current_height)
		{
			current_height = heights[i];
			std::cout << " " << i << " " << current_height;
		}
	}
	std::cout << " " << max_right << " " << 0 << std::endl;
	return 0;
}

