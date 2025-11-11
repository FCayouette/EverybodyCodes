import std.core;

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
/*
std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	auto pos = s.find(token), tSize = token.size();
	while (pos != std::string::npos)
	{
		results.push_back(s.substr(0, pos));
		s = s.substr(std::min(pos + tSize, s.length()));
		pos = s.find(token);
	}
	if (!s.empty())
		results.push_back(std::move(s));
	return results;
}
*/
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
		std::cout << "Usage: EC2025Day06.exe Part1Filename Part2Filename Part3Filename\n";
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
		in1 >> line;

		for (int i = 0; i < line.size(); ++i)
			if (line[i] == 'A')
				for (int j = i+1; j < line.size(); ++j)
					if (line[j] == 'a')
						++part1;


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
		in2 >> line;

		for (int i = 0; i < line.size(); ++i)
			if (line[i] <= 'C')
				for (int j = i + 1, diff = line[i]-'A'; j < line.size(); ++j)
					if (line[j] == ('a'+diff))
						++part2;



		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		u64 part3 = 0;

		in3 >> line;
		for (char x = 0; x < 3; ++x)
		{
			std::vector<int> mentors, novices;
			for (int i = 0; i < line.size(); ++i)
			{
				if (line[i] == 'A' + x)
					mentors.push_back(i);
				else if (line[i] == 'a' + x)
					novices.push_back(i);
			}
			for (int n : novices)
			{
				int factor = 1000;
				for (int i = 1, m = n + i; i <= 1000; ++i, ++m)
				{
					if (m >= line.size())
					{
						--factor;
						m = 0;
					}
					if (std::binary_search(ALLc(mentors), m))
						part3 += factor;
				}
				factor = 1000;
				for (int i = 0, m = n; i <= 1000; ++i, --m)
				{
					if (std::binary_search(ALLc(mentors), m))
						part3 += factor;
					if (m == 0)
					{
						m = line.size();
						--factor;
					}
				}
					
			}
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}