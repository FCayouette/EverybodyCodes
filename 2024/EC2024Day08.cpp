import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day08.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		u64 part1 = 0;
		int blocks = 0;
		in1 >> blocks;

		for (int i = 1; true; ++i)
			if (int square = i * i;
				square > blocks)
			{
				part1 = square - blocks;
				part1 *= 2*i -1;
				break;
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
		u64 part2 = 0, blocks = 20240000, acolytes = 1111, priests;
		in2 >> priests;
		
		//blocks = 50, acolytes = 5, priests = 3;
		u64 count = 1, thickness = 1, steps = 1;
		while (count < blocks)
		{
			thickness = thickness * priests % acolytes;
			count += thickness * (++steps * 2 - 1);
		}
		part2 = (count - blocks) * (steps * 2 - 1);

		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		i64 highP, highA = 10, blocks = 202'400'000LL;
		//blocks = 202'400'000'000LL; // the old problem
		in3 >> highP;

		//highP = 2, highA = 5, blocks = 160; // example
		i64 thickness = 1, count = 1, width = 1, total = 1;
		std::vector<i64> thicknesses = {1};
		
		while (count < blocks)
		{
			thickness = (thickness * highP) % highA + highA;
			for (i64& t : thicknesses)
				t += thickness;
			thicknesses.push_back(thickness);
			width += 2LL;
			count = total += thickness * width;
			if (count > blocks)
				for (size_t i = 0; i < thicknesses.size()-1; ++i)
					count -= (highP * width % highA * thicknesses[i] % highA) * (i==0 ? 1LL: 2LL);
		}

		std::cout << std::format("Part 3: {}\n", count - blocks);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start));
}