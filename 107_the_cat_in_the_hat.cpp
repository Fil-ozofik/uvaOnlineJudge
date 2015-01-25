/*
107 - The Cat in the Hat
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=43
*/
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<uint32_t> get_sorted_divisors(uint32_t n)
{
	std::vector<uint32_t> divisors;
	uint32_t max_tested = sqrt(n);
	divisors.reserve(max_tested<<1);

	uint32_t current { 1U };

    while (current <= max_tested)
	{
        if (n % current == 0)
		{
			divisors.push_back(current);
			divisors.push_back(n/current);
		}
		++current;
	}
	//handles perfect square cases:
	if(divisors.size() > 1 && divisors.back()==*(divisors.end()-2))
	{
		divisors.erase(divisors.end()-1);
	}
	
	std::sort(divisors.begin(), divisors.end());

	return divisors;
}

int main(void)
{
	std::ios::sync_with_stdio(false);

	uint32_t initial_height, amount_little_ones;
	do
	{
		std::cin >> initial_height >> amount_little_ones;
		if(std::cin.eof() || (initial_height==0 && amount_little_ones==0))
			break;

		//Handling edge cases
		if(initial_height==1U)
		{
			std::cout << 1-static_cast<int32_t>(amount_little_ones) << " " << 1 << std::endl;
			continue;
		}
		else if(!amount_little_ones || !initial_height)
		{
			std::cout << 1 << " " << initial_height+amount_little_ones << std::endl;
			continue;
		}
		else if(amount_little_ones==1U)
		{
			//initial_height is either a power of 2 or nothing
			uint32_t current { 1U }, power { 0U };

			while(current < initial_height)
			{
				current <<= 1;
				++power;
			}

			if(current == initial_height)
			{
				//is a power of two
				std::cout << power << " " << (current<<1)-1 << std::endl;
			}
			else
			{
				//is nothing
				std::cout << 1 << " " << initial_height+amount_little_ones << std::endl;
			}
			continue;
		}
		
		std::vector<uint32_t> amount_divisors { get_sorted_divisors(amount_little_ones) };
		amount_divisors.erase(amount_divisors.begin());
		uint32_t n { 0U }, m { 1U };
		for(auto& divisor : amount_divisors)
		{
			uint32_t current { divisor };
			//we check if there is a m >= 1 such as divisor^m = amount_little_ones
			m = 1U;
			while(current <= amount_little_ones)
			{
				if(current == amount_little_ones)
				{
					n = divisor;
					break;
				}
				current *= divisor;
				++m;
			}
			if(n)
			{
				//we check that (n+1)^m = ini_height
				if(pow(n+1U, m)==initial_height)
					break;//good catch!!
				else
					n = 0U;//bad catch
			}
		}
		if(!n || n==amount_little_ones)
		{
			std::cout << 1 << " " << initial_height+amount_little_ones << std::endl;
			continue;
		}

		uint32_t height { initial_height }, height_divisor { n+1U }, amount_cats { 1U }, free_cats { 0U }, cumulated_height { 0U };
		while(height != 1U)
		{
			cumulated_height += height*amount_cats;
			free_cats += amount_cats;
			height /= height_divisor;
			amount_cats *= n;
		}
		cumulated_height += amount_little_ones;

		std::cout << free_cats << " " << cumulated_height << std::endl;
	} while(1);
	return 0;
}
