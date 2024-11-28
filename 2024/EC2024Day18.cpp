import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x = {}, y = {};
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day18.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int part1 = 0;
		std::vector<std::string> map;
		std::string line;
		std::set<Point> trees;
		while(std::getline(in1, line))
		{
			for (int i = 0; i < line.size(); ++i)
				if (line[i] == 'P')
					trees.emplace((int)map.size(), i);
			map.push_back(std::move(line));
		}

		Point start = {-1,-1};
		if (auto pos = map.front().find('.'); pos != std::string::npos)
			start = {0, (int)pos};
		else if (auto p = map.back().find('.'); p != std::string::npos)
			start = {0, (int)p};
		else
			for (int i = 1; i < map.size() && start.x < 0; ++i)
			{
				const std::string& s = map[i];
				if (s.front() == '.')
					start = {i, 0};
				else if (s.back() == '.')
					start = {i, (int)(s.size()-1)};
			}
		
		std::set<Point> cur = {start}, guard, next;
		while (!trees.empty())
		{
			++part1;
			for (Point p : cur)
				for (Point d : directions)
					if (Point np = p + d; 
						np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size() &&
						map[np.x][np.y] != '#' && guard.insert(np).second)
					{
						next.insert(np);
						if (map[np.x][np.y] == 'P')
							trees.erase(np);
					}
			std::swap(cur, next);
			next.clear();
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
		int part2 = 0;
		std::vector<std::string> map;
		std::string line;
		std::set<Point> trees;
		while (std::getline(in2, line))
		{
			for (int i = 0; i < line.size(); ++i)
				if (line[i] == 'P')
					trees.emplace((int)map.size(), i);

			map.push_back(std::move(line));
		}

		std::set<Point> cur, guard, next;
		if (auto pos = map.front().find('.'); pos != std::string::npos)
			cur.emplace(0, (int)pos);
		if (auto p = map.back().find('.'); p != std::string::npos)
			cur.emplace(0, (int)p);
		else
			for (int i = 1; i < map.size(); ++i)
			{
				const std::string& s = map[i];
				if (s.front() == '.')
					cur.emplace(i, 0);
				if (s.back() == '.')
					cur.emplace(i, (int)(s.size() - 1));
			}
		
		while (!trees.empty())
		{
			++part2;
			for (Point p : cur)
				for (Point d : directions)
				{
					Point np = p + d;
					if (np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size() &&
						map[np.x][np.y] != '#' && guard.insert(np).second)
					{
						next.insert(np);
						if (map[np.x][np.y] == 'P')
							trees.erase(np);
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
		std::vector<std::string> map;
		std::string line;
		std::vector<Point> trees;
		std::map<Point, int> distances;
		while (std::getline(in3, line))
		{
			for (int i = 0; i < line.size(); ++i)
				if (char c = line[i]; c == 'P')
					trees.emplace_back((int)map.size(), i);
			map.push_back(std::move(line));
		}

		for (Point start : trees)
		{
			int step = 0;
			std::set<Point> cur{start}, guard{start}, next;
			
			while (!cur.empty())
			{
				++step;
				for (Point p : cur)
					for (Point d : directions)
						if (Point np = p + d; 
							np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size() &&
							map[np.x][np.y] != '#' && guard.insert(np).second)
						{
							next.insert(np);
							if (map[np.x][np.y] == '.')
								distances[np] += step;
						}
				std::swap(next, cur);
				next.clear();
			}
		}
		int part3 = std::numeric_limits<int>::max();
		for (auto [_, d] : distances)
			part3 = std::min(part3, d);

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}