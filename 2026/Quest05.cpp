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
	
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 8> directions = { Point(1,1), Point(1, 0), Point(1,-1), Point(0, 1), Point(0,-1), Point(-1,-1), Point(-1, 0), Point(-1,1)};

size_t Compute(std::ifstream& in, int part)
{
	std::vector<std::string> map;
	std::string line;
	while (in >> line)
		map.emplace_back(std::move(line));
	std::unordered_set<Point> cur = { Point{(int)map.size() / 2, (int)map.front().size() / 2} }, next, land = cur, sand, trees;

	while (!cur.empty())
	{
		for (Point p : cur)
			for (Point d : directions)
				if (Point potential = p + d; potential.Within(map) && !land.contains(potential))
				{
					int count = 0;
					for (Point delta : directions)
						if (Point t = potential + delta; t.Within(map) && (land.contains(t) || t.GetAt(map) == '1'))
							++count;
					if (count >= 5)
						next.insert(potential);
				}
		land.insert(next.cbegin(), next.cend());
		std::swap(cur, next);
		next.clear();
	}
	if (part == 1) return land.size();

	bool goOn = true;
	do {
		goOn = false;
		next.clear();
		for (Point p : land)
			if (!sand.contains(p) && !next.contains(p))
			{
				int count = 0;
				for (Point d : directions)
				{
					Point np = p + d;
					if (!land.contains(np) || !np.Within(map))
					{
						count += 5;
						break;
					}
					if (sand.contains(np))
						++count;
				}
				if (count >= 5)
				{
					next.insert(p);
					goOn = true;
				}
			}

		sand.insert(next.cbegin(), next.cend());
		next.clear();
	} while (goOn);
	if (part == 2) return sand.size();
	
	for (int i = 2; i < map.size() - 2; ++i)
		for (int j = 2; j < map.front().size() - 2; ++j)
		{
			bool good = true;
			for (int x = -2; x <= 2 && good; ++x)
				for (int y = -2; y <= 2 && good; ++y)
				{
					if ((x == 0 && y == 0) || (std::abs(x) == 2 && std::abs(y) == 2)) continue;
					if (Point p{ i + x, j + y }; !land.contains(p) || sand.contains(p))
						good = false;
				}
			if (good)
				trees.insert(Point{ i,j });
		}

	for (int step = 0; step < 2; ++step)
	{
		next.clear();
		for (Point t : trees)
			for (Point d : directions)
				if (Point p = t + d; !trees.contains(p))
				{
					int count = 0;
					for (Point delta : directions)
					{
						Point test = p + delta;
						if (sand.contains(test)) { count = 0; break; }
						if (trees.contains(test)) ++count;
					}
					if (count >= 4)
						next.insert(p);
				}
		trees.insert(next.cbegin(), next.cend());

	}
	return trees.size();
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest05.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::cout << std::format("Part 1: {}\n", Compute(in, 1));
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		std::cout << std::format("Part 2: {}\n", Compute(in, 2));
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		std::cout << std::format("Part 3: {}\n", Compute(in, 3));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}