import std;

using u64 = unsigned long long;

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
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	auto& GetAt(U& grid) const { return grid[x][y]; }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> diagonals = { Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1) };

int Simulate(std::vector<std::string>&& map, int turns)
{
	std::vector<std::string> next = map;
	int result = 0;
	for (int t = 0; t < turns; ++t)
	{
		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map[i].size(); ++j)
			{
				Point p(i, j);
				int count = 0;
				for (Point d : diagonals)
					if (Point np = p + d; np.Within(map) && np.GetAt(map) == '#')
						++count;

				if (p.GetAt(map) == '#')
				{
					if (count % 2 == 1)
					{
						++result;
						p.GetAt(next) = '#';
					}
					else p.GetAt(next) = '.';
				}
				else if (count % 2 == 0)
				{
					++result;
					p.GetAt(next) = '#';
				}
				else p.GetAt(next) = '.';
			}
		std::swap(map, next);
	}
	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day14.exe Part1Filename Part2Filename Part3Filename\n";
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
		
		std::cout << std::format("Part 1: {}\n", Simulate(std::move(map), 10));
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

		std::cout << std::format("Part 2: {}\n", Simulate(std::move(map), 2025));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{

		std::vector<std::string> map, next, pattern;
		for (int i = 0; i < 17; ++i)
			map.emplace_back("#################");
		next = map;

		while (in3 >> line)
			pattern.emplace_back(line.substr(0, line.size() / 2));

		pattern.resize(pattern.size() / 2);

		std::vector<std::pair<int, int>> scorePattern;
		u64 part3 = 0, cycle = 0;

		for (u64 t = 2; t <= 1000000000ull; ++t)
		{
			int activeCount = 0;
			for (int i = 0; i < map.size(); ++i)
				for (int j = 0; j < map[i].size(); ++j)
				{
					Point p(i, j);
					int count = 0;
					for (Point d : diagonals)
					{
						Point np = p + d;
						if (np.x == 17) np.x = 16;
						if (np.y == 17) np.y = 16;
						if (np.Within(map) && np.GetAt(map) == '#')
							++count;
					}

					if (p.GetAt(map) == '#')
					{
						if (count % 2 == 1)
						{
							++activeCount;
							p.GetAt(next) = '#';
						}
						else p.GetAt(next) = '.';
					}
					else if (count % 2 == 0)
					{
						++activeCount;
						p.GetAt(next) = '#';
					}
					else p.GetAt(next) = '.';
				}
			std::swap(map, next);

			bool found = true;
			for (int i = 17 - pattern.size(), j = 0; i < 17 && found; ++i, ++j)
				found = map[i].substr(map[i].size() - pattern[j].size()) == pattern[j];

			if (found)
			{
				if (int score = 4 * activeCount; scorePattern.empty() || scorePattern.front().second != score)
					scorePattern.emplace_back(t, score);
				else
				{
					cycle = t - scorePattern.front().first;
					break;
				}
			}
		}

		for (auto [t, s] : scorePattern)
			part3 += s * ((1000000000ull - t) / cycle + 1);

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}