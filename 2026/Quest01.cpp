import std;

using u64 = unsigned long long;

[[nodiscard]] std::vector<int> SplitAt(std::string s, std::string token)
{
	std::vector<int> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(std::stoi(s.substr(tokenStart)));
			break;
		}
		results.emplace_back(std::stoi(s.substr(tokenStart, e - tokenStart)));
		tokenStart = e + token.size();
	} while (tokenStart < s.size());

	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest01.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;

	{
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
			return -1;
		}
		u64 part1 = 0, i = 0, square = 1;
		in >> line;
		for (int v : SplitAt(std::move(line), ","))
		{
			while (square < v)
				square += ++i*2 + 1;
			part1 += square - v;
		}
		std::cout << std::format("Part 1: {}\n", part1);
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		u64 part2 = 0, perimeter = 8;
		in >> line;
		for (int v : SplitAt(std::move(line), ","))
		{
			while (perimeter < v)
				perimeter += 4;
			part2 += perimeter - v;
		}
		std::cout << std::format("Part 2: {}\n", part2);
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		u64 part3 = 0, i = 1, square = 8;
		in >> line;
		for (int v : SplitAt(std::move(line), ","))
		{
			while (square < v)
				square += ++i * 8;
			part3 += square - v;
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}