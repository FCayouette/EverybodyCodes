import std.core;

using u64 = unsigned long long;

constexpr std::pair<u64, u64> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(1ull, 1ull);
	return std::make_pair(std::stoull(s.substr(0, pos)), std::stoull(s.substr(pos + token.size())));
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day04.exe Part1Filename Part2Filename Part3Filename\n";
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
		u64 start, end;
		in1 >> start;
		while (in1 >> end);

		std::cout << std::format("Part 1: {}\n", 2025 * start / end);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		double start, end;
		in2 >> start;
		while (in2 >> end);

		std::cout << std::format("Part 2: {}\n", std::ceil(end * 10000000000000. / start));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		u64 start, end, factor = 1;
		in3 >> start;
		start *= 100;
		while (in3 >> line)
		{
			if (line.find('|') == std::string::npos)
			{
				end = std::stoull(line);
				break;
			}
			auto p = SplitAtFirst(std::move(line), "|");
			factor *= p.second / p.first;
		}
		
		std::cout << std::format("Part 3: {}\n", start * factor / end);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}