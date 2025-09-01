import std.core;

constexpr std::array<char, 3> order = { 'R', 'G', 'B' };

int CirclePop(std::string&& line, int multiple)
{
	std::string ring;
	ring.reserve(line.size() * multiple);
	while (multiple--)
		ring += line;

	int toPop = ring.size(), rounds = 0;
	auto start = ring.begin(), mid = start + toPop / 2;

	while (true)
	{
		if (*start == order[rounds++ % 3] && toPop % 2 == 0)
		{
			*(mid++) = '#';
			--toPop;
		}
		else if (toPop % 2)
			++mid;
		if (--toPop)
			while (*(++start) == '#');
		else
			return rounds;
	}
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S2Q2.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int part1 = 0;
		in1 >> line;
		auto iter = line.cbegin();
		while (iter != line.cend())
		{
			char c = order[part1++ % 3];
			while (*iter == c)
				if (++iter == line.cend())
					goto done;
			++iter;
		}
	done:
		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::string ring;
		in2 >> line;
		std::cout << std::format("Part 2: {}\n", CirclePop(std::move(line), 100));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::string ring;
		in3 >> line;
		std::cout << std::format("Part 3: {}\n", CirclePop(std::move(line), 100000));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}