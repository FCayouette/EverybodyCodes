import std.core;

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	T x = {}, y = {};
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

struct Glider
{
	Point p = {};
	char dir = 0;
	bool operator < (const Glider& g) const {if (p < g.p) return true; if (g.p < p) return false; return dir < g.dir;};
};

struct Racer : public Glider
{
	char stage = 0;

	constexpr bool IncorrectStage(char c) const
	{
		switch (c)
		{
			case 'A': return stage != 0;
			case 'B': return stage != 1;
			case 'C': return stage != 2;
			case 'S': return stage != 3;
			default: return false;
		}
	}

	bool operator < (const Racer& g) const {
		if (p < g.p) return true; if (g.p < p) return false; if (dir < g.dir) return true;
		if (g.dir < dir) return false; return stage < g.stage;
	}
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day20.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::string line;
		std::vector<std::string> map;
		Glider start;
		while (std::getline(in1, line))
		{
			if (auto pos = line.find('S'); pos != std::string::npos)
				start.p = {(int)map.size(), (int)pos};
			map.push_back(std::move(line));
		}

		std::set<Glider> cur = {start}, next;
		std::map<Glider, int> heights, nextHeights;
		heights[start] = 1000;

		for (int step = 0; step < 100; ++step)
		{
			for (const Glider& g : cur)
			{
				Point p = g.p;
				int height = heights[g];
				for (int d = 0, dir = g.dir; d < 4; ++d, dir = (dir +1) % 4)
				{
					if (d == 2) continue;
					if (Point np = p + directions[dir]; np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size())
					{
						char c = map[np.x][np.y];
						if (c == '#') continue;
						int delta = (c == '+' ? 1 : c == '-' ? -2 : -1);
						Glider ng {np, dir};
						next.insert(ng);
						nextHeights[ng]  = std::max(nextHeights[ng], height + delta);
					}
				}
			}
			std::swap(next, cur);
			std::swap(heights, nextHeights);
			next.clear();
			nextHeights.clear();
		}

		int part1 = 0;
		for (const auto& iter : heights)
			part1 = std::max(part1, iter.second);

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
		std::string line;
		std::vector<std::string> map;
		Racer start;
		while (std::getline(in2, line))
		{
			if (auto pos = line.find('S'); pos != std::string::npos)
				start.p = { (int)map.size(), (int)pos };
			map.push_back(std::move(line));
		}

		std::set<Racer> cur = { start }, next;
		std::map<Racer, int> heights, nextHeights;
		heights[start] = 10000;

		bool found = false;
		do 
		{
			++part2;
			for (const Racer& r : cur)
			{
				Point p = r.p;
				int height = heights[r];
				for (int d = 0, dir = r.dir; d < 4; ++d, dir = (dir + 1) % 4)
				{
					if (d == 2) continue;
					Point np = p + directions[dir];
					if (np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size())
					{
						char c = map[np.x][np.y];
						if (c == '#') continue;
						if (r.IncorrectStage(c)) continue;
						int delta = (c == '+' ? 1 : c == '-' ? -2 : -1);
						int nh = height + delta;
						if (c == 'S')
						{
							if (nh < 10000) continue;
							found = true;
							break;
						}
						Racer nr{ np, dir, r.stage };
						if (c >= 'A' && c <= 'C')
							++nr.stage;
						next.insert(nr);
						nextHeights[nr] = std::max(nextHeights[nr], height + delta);
					}
				}
			}
			std::swap(cur, next);
			std::swap(heights, nextHeights);
			next.clear();
			nextHeights.clear();
			
		} while (!found);

		std::cout << std::format("Part 2: {}\n", part2);
	}
	
	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::string line;
		std::vector<std::string> map;
		Point start;
		while (std::getline(in3, line))
		{
			if (auto pos = line.find('S'); pos != std::string::npos)
				start = { (int)map.size(), (int)pos};
			map.push_back(std::move(line));
		}
		int minDrop = -100000, minDropIndex = 0;
		for (int col = 1; col < map.front().size(); ++col)
		{
			int drop= 0;
			for (int x = 0; x < map.size(); ++x)
			{
				char c = map[x][col];
				drop += (c == '+' ? 1 : c == '-' ? -2 : c == '#'? -1000 : -1);
			}
			if (drop >= minDrop)
			{
				if (drop > minDrop)
					minDropIndex = col;
				else if (std::abs(start.y - col) < std::abs(start.y - minDropIndex))
						minDropIndex = col;
				minDrop = drop;
			}
		}

		int altitude = 384400 - std::abs(start.y - minDropIndex), part3 = 0;
		line.clear();
		for (const std::string& s : map)
			line += s[minDropIndex];
		while (altitude > 0)
		{
			++part3;
			char c = line[part3 % line.size()];
			altitude += (c == '.' ? -1 : 1);				
		}
		
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}