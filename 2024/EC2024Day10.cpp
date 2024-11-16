import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

constexpr bool Known(char c) { return c >='A' && c <='Z';}

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();

	if (argc < 4)
	{
		std::cout << "Usage: EC2024DayXX.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::string part1 = "";
		std::vector<std::string> map;
		std::string s;
		while (in1 >> s)
		{
			map.push_back(std::move(s));
		}

		for (int x = 2; x < 6; ++x)
			for (int y = 2; y < 6; ++y)
			{
				std::set<char> ver, hor;
				std::vector<char> res;
				ver.insert(map[0][y]);
				ver.insert(map[1][y]);
				ver.insert(map[6][y]);
				ver.insert(map[7][y]);
				hor.insert(map[x][0]);
				hor.insert(map[x][1]);
				hor.insert(map[x][6]);
				hor.insert(map[x][7]);
				std::set_intersection(ALLc(ver), ALLc(hor), std::back_inserter(res));
				part1 += res[0];
			}
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
		std::vector<std::vector<std::string>> maps;
		std::string s;
		for (int i = 0; i < 7; ++i)
		{
			maps.resize(15);
			for (int k = 0; k < 8; ++k)
				for (int j = 0; j < 15; ++j)
				{
					in2 >> s;
					maps[j].push_back(std::move(s));
				}

			for (const auto& map : maps)
				for (int x = 2, pos = 1; x < 6; ++x)
					for (int y = 2; y < 6; ++y)
					{
						std::set<char> ver, hor;
						std::vector<char> res;
						ver.insert(map[0][y]);
						ver.insert(map[1][y]);
						ver.insert(map[6][y]);
						ver.insert(map[7][y]);
						hor.insert(map[x][0]);
						hor.insert(map[x][1]);
						hor.insert(map[x][6]);
						hor.insert(map[x][7]);
						std::set_intersection(ALLc(ver), ALLc(hor), std::back_inserter(res));
						part2 += pos++ * (1 + (res[0] - 'A'));
					}
			maps.clear();
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
		std::vector<std::string> map;
		std::string s;
		while (in3 >> s)
			map.push_back(std::move(s));

		const int X = (map.size()-2)/6, Y = (map.front().size()-2) /6;
		std::vector<std::vector<char>> solved(X, std::vector<char>(Y, 0));

		auto SubMap = [&map](int x, int y) -> std::vector<std::string>
		{
			std::vector<std::string> subMap;
			for (int i = 0, a = x*6; i < 8; ++i, ++a)
				subMap.push_back(map[a].substr(y*6, 8));
			return subMap;
		};

		auto TrySolve = [](std::vector<std::string>& m) -> bool
		{
			bool missing = false, improved = false, addedInfo = false;
			for (int x = 2; x < 6; ++x)
				for (int y = 2; y < 6; ++y)
					if (m[x][y] == '.')
					{
						std::set<char> ver, hor;
						std::vector<char> res;
						if (char c = m[0][y]; c != '?')ver.insert(c);
						if (char c = m[1][y]; c != '?')ver.insert(c);
						if (char c = m[6][y]; c != '?')ver.insert(c);
						if (char c = m[7][y]; c != '?')ver.insert(c);
						if (char c = m[x][0]; c != '?')hor.insert(c);
						if (char c = m[x][1]; c != '?')hor.insert(c);
						if (char c = m[x][6]; c != '?')hor.insert(c);
						if (char c = m[x][7]; c != '?')hor.insert(c);

						std::set_intersection(ALLc(ver), ALLc(hor), std::back_inserter(res));
						if (!res.empty())
						{
							m[x][y] = res.front();
							improved = true;
						}
						else missing = true;
					}

			if (!missing)
				return true;
			do 
			{
				addedInfo = false;
				for (int x = 2; x < 6; ++x)
					for (int y = 2; y < 6; ++y)
						if (m[x][y] == '.')
						{
							std::map<char, int> histogram;
							int count = 0, total = 0;
							for (int i = 0; i < 8; ++i)
							{
								if (char c = m[x][i];
									Known(c))
									{
										++histogram[c];
										++total;
									}
								if (char c = m[i][y];
									Known(c))
									{
										++histogram[c];
										++total;
									}
							}
							if (total == 13)
							{
								char c;
								for (auto& p : histogram)
									if (p.second %2 == 1)
									{
										++count;
										c = p.first;
									}
								
								if (count == 1)
								{
									for (int i = 0; i< 8; ++i)
									{
										if (m[x][i] == '?')
										{
											m[x][y] = m[x][i] = c;
											improved = addedInfo = true;
											break;
										}
										if (m[i][y] == '?')
										{
											m[x][y] = m[i][y] = c;
											improved = addedInfo = true;
											break;
										}
									}
								}
								else if (count == 0)
									std::cout << '!';
							}
						}
			} while (addedInfo);

			return improved;
		};

		u64 part3 = 0;
		bool keepGoing = false;
		do 
		{
			keepGoing = false;
			for (int x = 0; x < X; ++x)
				for (int y = 0; y < Y; ++y)
					if (!solved[x][y])
					{
						auto subMap = SubMap(x, y);
						if (TrySolve(subMap))
						{
							keepGoing = true;
						
							for (int i = 0, a = x*6; i < 8; ++i, ++a)
								for (int j = 0, b = y*6; j < 8; ++j, ++b)
									map[a][b] = subMap[i][j];
						
							bool resolved = true;
							for (int i = 2; i < 6; ++i)
								for (int j = 2; j < 6; ++j)
									if (subMap[i][j] =='.')
										resolved = false;

							if (solved[x][y] = resolved)
								for (int i = 2, pos = 1; i < 6; ++i)
									for (int j = 2; j < 6; ++j)
										part3 += pos++ * (1 + (subMap[i][j] - 'A'));
						}
					}
		} while (keepGoing);

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start));
}