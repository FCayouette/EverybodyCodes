import std;

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
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day20.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::vector<std::string> map;
		while (in1 >> line)
		{
			for (int i = 1; i < line.size(); ++i)
				if (line[i] == 'T' && line[i - 1] == line[i])
					++part1;
			map.emplace_back(std::move(line));
		}
		
		for (int i = 1; i < map.size()-1; i += 2)
			for (int x = 0; x < map.front().size(); ++x)
				if (map[i][x] == 'T' && map[i + ((i + x) % 2 ? 1 : -1)][x] == 'T')
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

		std::vector<std::string> map;
		Point start, end;

		while (in2 >> line)
		{
			if (auto pos = line.find('E'); pos != std::string::npos)
				end = Point((int)map.size(), (int)pos);
			if (auto pos = line.find('S'); pos != std::string::npos)
				start = Point((int)map.size(), (int)pos);
			map.emplace_back(std::move(line));
		}

		std::set<Point> cur = { start }, next, all = cur;
		bool found = false;
		std::array<Point, 3> deltas = { Point(0,-1), Point(0,1), Point(0,0) };
		while (!found)
		{
			++part2;
			for (Point p : cur)
			{
				deltas.back().x = (p.x+p.y) %2 == 0 ? -1 : 1;
				for (Point d : deltas)
				{
					Point np = p + d;
					if (np == end) { found = true; break; }
					if (np.Within(map) && np.GetAt(map) == 'T' && all.insert(np).second)
						next.insert(np);
				}
			}
			std::swap(cur, next);
			next.clear();
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
		std::vector<std::vector<std::string>> maps;
		maps.reserve(3); 
		maps.emplace_back();
		Point start;

		while (in3 >> line)
		{
			if (auto pos = line.find('S'); pos != std::string::npos)
				start = Point((int)maps.front().size(), (int)pos);
			maps.front().emplace_back(std::move(line));
		}

		for (int r = 0; r < 2; ++r)
		{
			std::vector<std::string>& ref = maps[r], to{ref};
			for (int i = 0; i < ref.size(); ++i)
			{
				Point p = start + Point(-i, i);
				bool up = true;
				for (int j = 0; j < ref.front().size(); ++j)
					if (to[i][j] != '.')
					{
						to[i][j] = p.GetAt(ref);
						p += (up ? Point(-1, 0) : Point(0, -1));
						up = !up;
					}
			}
			maps.emplace_back(std::move(to));
		}

		std::set<Point> cur = { start }, next;
		std::vector<std::set<Point>> all(3);
		all.front().insert(start);
		
		bool found = false;
		std::array<Point, 4> deltas = { Point(0,0), Point(0,-1), Point(0,1), Point(0,0) };
		while (!found)
		{
			++part3;
			int index = part3 % 3;
			for (Point p : cur)
			{
				deltas.back().x = (p.x + p.y) % 2 == 0 ? -1 : 1;
				for (Point d : deltas)
				{
					Point np = p + d;
					if (!np.Within(maps.front())) continue;
					if (char c = np.GetAt(maps[index]); c == 'E')
					{
						found = true;
						break;
					}
					if (np.GetAt(maps[index]) == 'T' && all[index].insert(np).second)
						next.insert(np);
				}
			}
			std::swap(cur, next);
			next.clear();
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}