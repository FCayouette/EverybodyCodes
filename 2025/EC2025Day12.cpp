import std;

#define ALLc(x) (x).cbegin(),(x).cend()

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
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S1QX.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::vector<std::string> map;
		while (in1 >> line)
			map.emplace_back(std::move(line));

		std::set<Point> exploded = { Point(0,0) }, cur = exploded, next;
		
		while (!cur.empty())
		{
			for (Point p : cur)
				for (Point d : directions)
					if (Point np = p + d; np.Within(map) && p.GetAt(map) >= np.GetAt(map) && exploded.insert(np).second)
						next.insert(np);

			std::swap(cur, next);
			next.clear();
		}
		std::cout << std::format("Part 1: {}\n", exploded.size());
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::vector<std::string> map;
		while (in2 >> line)
			map.emplace_back(std::move(line));

		std::set<Point> exploded = { Point(0,0) };
		Point lowRight = Point(map.size() - 1, map.front().size()-1);
		exploded.insert(lowRight);
		std::set<Point> cur = exploded, next;

		while (!cur.empty())
		{
			for (Point p : cur)
				for (Point d : directions)
					if (Point np = p + d; np.Within(map) && p.GetAt(map) >= np.GetAt(map) && exploded.insert(np).second)
						next.insert(np);

			std::swap(cur, next);
			next.clear();
		}
		std::cout << std::format("Part 2: {}\n", exploded.size());
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::vector<std::string> map;
		while (in3 >> line)
			map.emplace_back(std::move(line));

		std::array<std::set<Point>, 3> groups;
		std::vector<Point> seen, previousGroup;

		for (int r = 0; r < 3; ++r)
		{
			previousGroup.clear();
			for (int i = 0; i < map.size(); ++i)
				for (int j = 0; j < map.front().size(); ++j)
					if (Point start{ i,j }; !std::binary_search(ALLc(seen), start) && !std::binary_search(ALLc(previousGroup), start) && !groups[r].contains(start))
					{
						std::set<Point> exploded = { start }, cur = exploded, next;
						while (!cur.empty())
						{
							for (Point p : cur)
								for (Point d : directions)
									if (Point np = p + d; np.Within(map) && !std::binary_search(ALLc(seen), np) && p.GetAt(map) >= np.GetAt(map) && exploded.insert(np).second)
										next.insert(np);

							std::swap(cur, next);
							next.clear();
						}
						previousGroup.clear();
						previousGroup.insert(previousGroup.end(), ALLc(exploded));
						if (exploded.size() > groups[r].size())
							std::swap(exploded, groups[r]);
					}
			seen.insert(seen.end(), ALLc(groups[r]));
			std::sort(seen.begin(), seen.end());
		}

		std::cout << std::format("Part 3: {}\n", std::accumulate(ALLc(groups), 0ull, [](size_t total, const std::set<Point>& s) { return total + s.size(); }));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}