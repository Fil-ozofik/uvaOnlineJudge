/*
100 - The 3n + 1 problem
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=36
*/
#include <cstdint>
#include <unordered_map>

static std::unordered_map<int32_t, int32_t> memory;

int32_t cycle_length(int32_t n) noexcept
{
    if(memory.count(n) == 1)
		return memory[n];

    if(n & 1)
    {
        int32_t next { (n << 1) + n + 1 };
        int32_t v { 1 + cycle_length(next) };
        memory[n] = v;
        return v;
    }
    int32_t next { n >> 1 };
    int32_t v { 1 + cycle_length(next) };
    memory[n] = v;
    return v;
}


int main(void)
{
    memory[1] = 1;
	int32_t start, end, current_max, tested;
	while(scanf("%d %d", &start, &end) != EOF)
	{
		current_max = 1;
		for(uint32_t i = start<end ? start : end, end_i = (start>end ? start : end)+1; i != end_i; ++i)
		{
			tested = cycle_length(i);
			if(tested > current_max)
				current_max = tested;
		}
		printf("%d %d %d\n", start, end, current_max);
	}
    return 0;
}