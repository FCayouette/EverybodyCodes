import std;

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day11.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	std::string line;
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		u64 part1 = 0;

		std::vector<int> ducks;
		int col;
		while (in1 >> col)
			ducks.push_back(col);

		bool moved = false;
		int count = 0;
		do {
			moved = false;
			for (int i = 1; i < ducks.size(); ++i)
				if (ducks[i] < ducks[i - 1])
				{
					moved = true;
					++ducks[i];
					--ducks[i - 1];
				}
			if (moved)
				++count;
		} while (moved && count < 10);

		for (int r = count; r < 10; ++r)
			for (int i = 1; i < ducks.size(); ++i)
				if (ducks[i] > ducks[i - 1])
				{
					++ducks[i - 1];
					--ducks[i];
				}
		
		for (int i = 0; i < ducks.size(); ++i)
			part1 += (i + 1) * ducks[i];

		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 0;
		std::vector<int> ducks;
		int col, target = 0;
		while (in2 >> col)
		{
			target += col;
			ducks.push_back(col);
		}

		bool moved = false;
		int count = 0;
		do {
			moved = false;
			for (int i = 1; i < ducks.size(); ++i)
				if (ducks[i] < ducks[i - 1])
				{
					moved = true;
					++ducks[i];
					--ducks[i - 1];
				}
			if (moved)
				++count;
		} while (moved);

		target /= ducks.size();
		for (int d : ducks)
			if (d < target)
				count += target - d;
			else break;

		std::cout << std::format("Part 2: {}\n", count);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		u64 part3 = 0, col, target= 0;
		std::vector<u64> ducks;
		while (in3 >> col)
		{
			target += col;
			ducks.push_back(col);
		}
		target /= ducks.size();
		for (int i = 0; i < ducks.size(); ++i)
			if (ducks[i] < target)
				part3 += target - ducks[i];
			else break;

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}