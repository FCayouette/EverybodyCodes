import std.core;

using u64 = unsigned long long;

[[nodiscard]] std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(s.substr(tokenStart));
			break;
		}
		results.emplace_back(s.substr(tokenStart, e - tokenStart));
		tokenStart = e + token.size();
	} while (tokenStart < s.size());

	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day03.exe Part1Filename Part2Filename Part3Filename\n";
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
		u64 part1 = 0;
		in1 >> line;
		std::set<int> numbers;
		for (const auto& s : SplitAt(std::move(line), ","))
			numbers.insert(std::stoi(s));
		std::cout << std::format("Part 1: {}\n", std::accumulate(numbers.cbegin(), numbers.cend(), 0));
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 0;
		in2 >> line;
		std::set<int> numbers;
		for (const auto& s : SplitAt(std::move(line), ","))
			numbers.insert(std::stoi(s));
		auto iter = numbers.begin();
		for (int i = 0; i < 20; ++i, ++iter)
			part2 += *iter;
		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		int part3 = 0;
		std::map<int, int> counts;
		in3 >> line;
		for (const auto& s : SplitAt(std::move(line), ","))
			part3 = std::max(part3, ++counts[std::stoi(s)]);

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}