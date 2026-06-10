import std;

#define ALL(x) (x).begin(),(x).end()

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

struct Color
{
	int r = {}, g = {}, b = {}, s = {};
};

Color ExtractColor(std::string s)
{
	Color color;
	for (char c : s)
	{
		if (c == 'r' || c == 'R')
			color.r = color.r * 2 + (c == 'R');
		else if (c == 'g' || c == 'G')
			color.g = color.g * 2 + (c == 'G');
		else if (c == 'b' || c == 'B')
			color.b = color.b * 2 + (c == 'B');
		else if (c == 's' || c == 'S')
			color.s = color.s * 2 + (c == 'S');
	}

	return color;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2026S3D1.exe Part1Filename Part2Filename Part3Filename\n";
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
		int part1 = 0;
		while (std::getline(in, line))
		{
			auto [val, colors] = SplitAtFirst(std::move(line), ":");
			Color color = ExtractColor(std::move(colors));
			if (color.g > color.r && color.g > color.b)
				part1 += std::stoi(val);
				
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
		int part2 = 0;
		int maxS = 0, lowCol = 0;
		while (std::getline(in, line))
		{
			auto [val, colors] = SplitAtFirst(std::move(line), ":");
			Color c = ExtractColor(std::move(colors));
			int col = c.r + c.g + c.b;
			if (c.s > maxS)
			{
				maxS = c.s;
				lowCol = col;
				part2 = std::stoi(val);
			}
			if (c.s == maxS)
				if (col < lowCol)
				{
					maxS = c.s;
					lowCol = col;
					part2 = std::stoi(val);
				}
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
		int part3 = 0;

		int maxS = 0, lowCol = 0;
		std::array<std::pair<int, int>, 6> groups;
		while (std::getline(in, line))
		{
			auto [val, colors] = SplitAtFirst(std::move(line), ":");
			Color c = ExtractColor(std::move(colors));
			int index = 0, value = std::stoi(val);
			if (c.s == 31 || c.s == 32)
				continue;
			if (c.s > 32)
				index += 3;
			if (c.r > c.g && c.r > c.b)
			{
				++groups[index].first;
				groups[index].second += value;
			}
			else if (c.g > c.r && c.g > c.b)
			{
				++groups[++index].first;
				groups[index].second += value;
			}
			else if (c.b > c.r && c.b > c.g)
			{
				++groups[index += 2].first;
				groups[index].second += value;
			}
		}

		std::sort(ALL(groups));
		std::cout << std::format("Part 3: {}\n", groups.back().second);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}