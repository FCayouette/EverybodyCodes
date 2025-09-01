import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using i64 = long long;

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

template <typename T>
constexpr bool IsEqual(T a, T b)
{
	if constexpr (std::is_floating_point_v<T>)
		return (a < b ? b - a : a - b) < static_cast<T>(0.000001);
	else
		return a == b;
}

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 5> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1), Point(0,0) };

struct Die
{
	std::vector<int> values;
	i64 seed, pulse, roll = 1, face = 0;
	int GenerateNext()
	{
		i64 spin = roll * pulse;
		face = (face + spin) % values.size();
		pulse = (pulse + spin) % seed + 1 + roll++ + seed;
		return values[face];
	}
};

Die ReadDie(std::string line)
{
	std::vector<std::string> tokens =  SplitAt(line, " ");
	std::vector<std::string> v = SplitAt(tokens[1].substr(7, tokens[1].size()-8), ",");
	std::vector<int> values;
	for (const std::string& s : v)
		values.push_back(std::stoi(s));
	int seed= std::stoi(tokens[2].substr(5));
	return Die {values, seed, seed};
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S2Q3.exe Part1Filename Part2Filename Part3Filename\n";
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
		i64 part1 = 0, total = 0;
		std::vector<Die> dices;
		while (std::getline(in1, line))
			dices.emplace_back(ReadDie(std::move(line)));
		
		
		while(total < 10000 && ++part1)
			for (auto& d : dices)
				total += d.GenerateNext();
		
		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::string part2;
		using PosDie = std::pair<int, Die>;
		std::vector<PosDie> posDices;
		
		while (std::getline(in2, line))
		{
			if (line.empty())
				break;
			posDices.emplace_back(0, ReadDie(std::move(line)));
		}

		in2 >> line;
		int count = 0;
		
		while (count < posDices.size())
			for (int i = 0; i < posDices.size(); ++i)
			{
				int& p = posDices[i].first;
				Die& die = posDices[i].second;
				if (p < line.size())
					if (int v = die.GenerateNext(); 
						(char)('0' + v) == line[p] && ++p == line.size())
					{
						if (part2.empty())
							part2 = std::format("{}", i + 1);
						else
							part2 += std::format(",{}", i + 1);

						++count;
					}
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
		std::vector<Die> dices;
		while (std::getline(in3, line))
		{
			if (line.empty())
				break;
			dices.emplace_back(ReadDie(std::move(line)));
		}

		std::vector<std::vector<int>> map, usedMap;
		while (in3 >> line)
		{
			std::vector<int> row, useRow(line.size(), 0);
			usedMap.emplace_back(std::move(useRow));

			for (char c : line)
				row.push_back((int)(c-'0'));
			map.emplace_back(std::move(row));
		}

		for (auto& die : dices)
		{
			int v = die.GenerateNext();
			
			std::vector<Point> cur, next;
			for (int x = 0; x < map.size(); ++x)
				for (int y = 0; y < map.front().size(); ++y)
					if (map[x][y] == v)
						cur.emplace_back(x, y);

			while (!cur.empty())
			{
				v = die.GenerateNext();
				for (Point p : cur)
				{
					usedMap[p.x][p.y] = 1;
					for (Point d : directions)
						if (Point np = p + d;
							np.Within(map) && np.GetAt(map) == v)
								next.push_back(np);
				}
				std::sort(ALL(next));
				next.erase(std::unique(ALL(next)), next.end());
				std::swap(cur, next);
				next.clear();
			}
		}
		int part3 = 0;
		for (const auto& row : usedMap)
			part3 += std::accumulate(ALLc(row), 0);
		std::cout << std::format("Part 3: {}\n", part3);

		/*
		// print image
		line.clear();
		for (int x = 0; x < map.size(); ++x, std::cout << line << '\n', line.clear())
			for (int y = 0; y < map.front().size(); ++y)
				line += (usedMap[x][y] ? '#' : ' ');
		*/
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}