import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day03.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	using Map = std::vector<std::vector<int>>;

	auto Sides = [](const Map& m, int x, int y) -> bool
	{
		return m[x-1][y] >= m[x][y] && m[x][y-1] >= m[x][y] && m[x][y+1] == m[x][y] && m[x+1][y] == m[x][y];
	};

	auto All = [](const Map& m, int x, int y) -> bool
	{
		for (int i = -1; i < 2; ++i)
			for (int j = -1; j < 2; ++j)
				if (m[x + i][y + j] < m[x][y])
					return false;
		return true;
	};

	auto Dig = [](std::ifstream& in, std::function<bool(const Map&, int, int)> eval) -> int
	{
		int result = 0;
		Map map;
		std::string s;
		while (in >> s)
		{
			std::vector<int> dig;
			for (char c : s)
				dig.push_back(c == '#');
			result += std::accumulate(ALLc(dig), 0);
			map.emplace_back(std::move(dig));
		}

		bool dig;
		int step = 1;
		do
		{
			dig = false;
			for (int x = 1; x < map.size() - 1; ++x)
				for (int y = 1; y < map[0].size() - 1; ++y)
					if (map[x][y] == step && eval(map, x, y))
					{
						dig = true;
						++map[x][y];
						++result;
					}
			++step;
		} while (dig);

		return result;
	};

	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	
	std::cout << std::format("Part 1: {}\n", Dig(in1, Sides));

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", Dig(in2, Sides));

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}

	std::cout << std::format("Part 3: {}\n", Dig(in3, All));
	return 0;
}