/*
100 - The 3n + 1 problem
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=36
*/
#include <iostream>

int main()
{
	std::ios::sync_with_stdio( false );
	int i, j, k, l;

	while( std::cin >> i >> j )
	{
		if( i < j )
		{
			k = i;
			l = j;
		}
		else
		{
			k = j;
			l = i;
		}

		int max_cycle_length = 0;
		int cycle_length;
		bool validIndex = false;

		while( k <= l )
		{
			unsigned int n = k;
			cycle_length = 1;

			while( n != 1 )
			{
				if( ( n & 1 ) == 1 )
				{
					n += ( n << 1 ) + 1;
					n >>= 1;
					cycle_length += 2;
				}
				else
				{
					n >>= 1;
					++cycle_length;
				}
			}

			if( cycle_length > max_cycle_length )
				max_cycle_length = cycle_length;

			k++;
		}

		std::cout << i << " " << j << " " << max_cycle_length << std::endl;
	}

	return 0;
}