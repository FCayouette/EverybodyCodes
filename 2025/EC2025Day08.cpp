import std.core;


[[nodiscard]] std::vector<int> GetNails(std::string s, char token)
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
		tokenStart = e + 1;
	} while (tokenStart < s.size());

	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day08.exe Part1Filename Part2Filename Part3Filename\n";
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
		int part1 = 0;
		in1 >> line;

		std::vector<int> indices = GetNails(line, ',');
		for (int i = 1; i < indices.size(); ++i)
			if (std::abs(indices[i] - indices[i-1]) == 16)
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
		int part2 = 0;
		in2 >> line;
		std::vector<int> indices = GetNails(line, ',');
		std::vector<std::pair<int, int>> lines;
		
		for (int i = 1; i < indices.size(); ++i)
		{
			int a = std::min(indices[i], indices[i-1]), b = std::max(indices[i], indices[i - 1]);
			for (auto [c, d] : lines)
			{
				if (a == c || a == d || b == c || b == d) continue;
				int within = 0;

				if (a < c && c < b)
					++within;
				if (a < d && d < b)
					++within;

				if (within == 1)
					++part2;
			}
			lines.push_back({a, b});
		}


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

		in3 >> line;
		std::vector<int> indices = GetNails(line, ',');
		std::vector<std::pair<int, int>> lines;
		for (size_t i = 1; i < indices.size(); ++i)
		{
			int a = indices[i-1], b = indices[i];
			lines.emplace_back(std::min(a,b), std::max(a,b));
		}

		std::sort(lines.begin(), lines.end());

		constexpr int maxNail = 256, spread = 18;
		for (int i = 1; i <= maxNail-(128-spread); ++i)
			for (int j = i + (128-spread), delta = 0; j <= maxNail && delta <= 2*spread+1; ++j, ++delta)
			{
				int count = 0;
				for (auto [a, b] : lines)
				{
					if (a == i || b == i || a == j || b == j)
					{
						if (a==i && b == j) 
							++count;
					}
					else
					{
						int within = 0;
						if (a < i && i < b)
							++within;
						if (a < j && j < b)
							++within;
						count += within % 2;
					}
				}
				part3 = std::max(count, part3);
			}


		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}