import std;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos) [[unlikely]]
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

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

int PowerMinus(int x)
{
	int result = 1;
	for (int i = 4-x; i>0; --i)
		result *= 10;
	return result;

}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest02.exe Part1Filename Part2Filename Part3Filename\n";
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
		{
			std::map<int, std::vector<int>> weakestThan;
			for (int i = 1; i < 6; ++i)
				weakestThan[i] = std::vector<int>{};
			u64 part1 = 0;
			while (std::getline(in, line))
			{
				std::stringstream ss(line);
				std::string token;
				while (ss >> token)
				{
					if (token[1] == '<')
						weakestThan[token[0]-'0'].push_back(token[2]-'0');
					else
						weakestThan[token[2] - '0'].push_back(token[0] - '0');
				}
				for (int i = 1; i < 6; ++i)
					part1 += i * PowerMinus(weakestThan[i].size());

				weakestThan.clear();
			}

			

			std::cout << std::format("Part 1: {}\n", part1);
		}
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		{
			u64 part2 = 0;
			std::map<int, std::vector<int>> weakestThan;
			for (int i = 1; i < 6; ++i)
				weakestThan[i] = std::vector<int>{};
			u64 part1 = 0;
			while (std::getline(in, line))
			{
				std::stringstream ss(line);
				std::string token;
				while (ss >> token)
				{
					if (token[1] == '<')
						weakestThan[token[0] - '0'].push_back(token[2] - '0');
					else
						weakestThan[token[2] - '0'].push_back(token[0] - '0');
				}
				std::set<int> found;
				int power = 10000000;
				for (int i = 0; i < 10; ++i)
				{
					if (i == 0)
						for (int x = 1; x < 10; ++x)
							if (weakestThan[x].empty())
							{
								found.insert(x);
								part2 += x * 100000000;
								break;
							}
					else
						for (int x = 1; x < 10; ++x)
							if (!found.contains(x))
								if (std::all_of(weakestThan[x].cbegin(), weakestThan[x].cend(), [&found](int x) {return found.contains(x); }))
								{
									found.insert(x);
									part2 += power * x;
									power /= 10;
								}
				}
				weakestThan.clear();
			}
			std::cout << std::format("Part 2: {}\n", part2);
		}
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		{
			u64 part3 = 0;
			std::map<int, std::vector<int>> weakestThan;
			for (int i = 1; i < 6; ++i)
				weakestThan[i] = std::vector<int>{};
			u64 part1 = 0;
			while (std::getline(in, line))
			{
				std::stringstream ss(line);
				std::string token;
				while (ss >> token)
				{
					if (token[1] == '<')
						weakestThan[token[0] - '0'].push_back(token[2] - '0');
					else
						weakestThan[token[2] - '0'].push_back(token[0] - '0');
				}
				std::set<int> found;
				int power = 10000000;
				for (int i = 0; i < 10; ++i)
				{
					if (i == 0)
						for (int x = 1; x < 10; ++x)
							if (weakestThan[x].empty())
							{
								found.insert(x);
								part3 += x * 100000000;
								break;
							}
					else
					{
						for (int x = 1; x < 10; ++x)
							if (!found.contains(x))
								if (std::all_of(weakestThan[x].cbegin(), weakestThan[x].cend(), [&found](int x) {return found.contains(x); }))
								{
									found.insert(x);
									part3 += power * x;
									power /= 10;
								}
					}

				}
				weakestThan.clear();
			}

			std::cout << std::format("Part 3: {}\n", part3);
		}
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}