import std.core;

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day09.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int part1 = 0, val;

		constexpr std::array<int, 4> stamps = {10, 5, 3, 1};
		while (in1 >> val)
			for (int i : stamps)
				while (val >= i)
				{	
					++part1;
					val -= i;
				}
		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		constexpr std::array<int, 10> stamps = {1, 3, 5, 10, 15, 16, 20, 24, 25, 30};
		std::vector<int> brightness, minCount;

		int part2 = 0, val, max = 0;
		while (in2 >> val)
		{
			max = std::max(max, val);
			brightness.push_back(val);
		}
		minCount.resize(max+1);
		for (int x = 1; x < max; ++x)
		{
			int min = std::numeric_limits<int>::max();
			for (int s : stamps)
				if (x-s >=0)
					min = std::min(min, minCount[x-s]);
			minCount[x] = min + 1;
		}
		
		for (int b : brightness)
			part2 += minCount[b];
		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		constexpr std::array<int, 18> stamps = { 1, 3, 5, 10, 15, 16, 20, 24, 25, 30, 37, 38, 49, 50, 74, 75, 100, 101 };
		std::vector<int> brightness, minCount;

		int part3 = 0, val, max = 0;
		while (in3 >> val)
		{
			max = std::max(max, val);
			brightness.push_back(val);
		}
		minCount.resize(max / 2 + 51);
		for (int x = 1; x < minCount.size(); ++x)
		{
			int min = std::numeric_limits<int>::max();
			for (int s : stamps)
				if (x - s >= 0)
					min = std::min(min, minCount[x - s]);
			minCount[x] = min + 1;
		}

		for (int b : brightness)
		{
			int l = b/2, h = b - l, min = std::numeric_limits<int>::max();
			do min = std::min(min, minCount[l--] + minCount[h++]);
			while (h-l <= 100);
			part3 += min;
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start));
}