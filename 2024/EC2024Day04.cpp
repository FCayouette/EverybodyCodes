import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day04.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	u64 part1 = 0, part2 = 0;
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::vector<int> nails;
		int v;
		while (in1 >> v)
			nails.push_back(v);
		part1 = std::accumulate(ALLc(nails), -*std::min_element(ALLc(nails)) * nails.size());
		
	}
	std::cout << std::format("Part 1: {}\n", part1);

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::vector<int> nails;
		int v;
		while (in2 >> v)
			nails.push_back(v);
		
		part2 = std::accumulate(ALLc(nails), -*std::min_element(ALLc(nails)) * nails.size());
	}
	std::cout << std::format("Part 2: {}\n", part2);

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::vector<i64> nails;
		i64 v;
		while (in3 >> v)
			nails.push_back(v);		

		i64 total = std::accumulate(ALLc(nails), 0ll), mean = total / nails.size(), part3 = std::numeric_limits<i64>::max();
		std::sort(ALL(nails));
		auto iter1 = std::find_if(ALLc(nails), [mean](i64 v) { return v >= mean; });
		--iter1;
		while (true)
		{
			i64 run = 0;
			for (i64 i : nails)
				run += std::abs(i - *iter1);

			if (run >= part3)
				break;
			part3 = run;
			++iter1;
		}
		
		std::cout << std::format("Part 3: {}\n", part3);
	}

	return 0;
}
