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
	constexpr Coord& operator-=(const Coord& p) { x -= p.x; y -= p.y; return *this; }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(-1, 0), Point(0, 1), Point(1, 0), Point(0,-1) };


template<int N>
int Surround(Point pos, std::set<Point> toCover, std::set<Point> visited, const std::array<Point, N>& sequence)
{
	int dir = 0, result = 0, minX = std::numeric_limits<int>::max(), minY = minX, maxX = 0, maxY = 0;
	for (const Point& p : toCover)
	{
		minX = std::min(minX, p.x), minY = std::min(minY, p.y);
		maxX = std::max(maxX, p.x), maxY = std::max(maxY, p.y);
	}

	while (!toCover.empty())
	{
		Point np = pos + sequence[dir];
		if (visited.insert(np).second)
		{
			pos = np;
			if (auto iter = toCover.find(pos); iter != toCover.end())
				toCover.erase(iter);
			if (pos.x < minX)
				minX = pos.x;
			else if (pos.x > maxX)
				maxX = pos.x;
			else if (pos.y < minY)
				minY = pos.y;
			else if (pos.y > maxY)
				maxY = pos.y;

			{
				// Must fill any holes
				std::set<Point> unbounded;
				for (Point d : directions)
					if (!visited.contains(pos + d))
					{
						std::set<Point> cur = { pos + d }, next, guard = cur;
						while (!cur.empty())
						{
							for (Point a : cur)
								for (Point b : directions)
									if (Point c = a + b; !visited.contains(c) && !guard.contains(c))
									{
										if (unbounded.contains(c) || c.x < minX || c.x > maxX || c.y < minY || c.y > maxY)
										{
											unbounded.merge(guard);
											goto quickOut;
										}
										else if (guard.insert(c).second)
											next.insert(c);
									}
							std::swap(cur, next);
							next.clear();
						}
						for (Point p : guard)
						{
							if (auto iter = toCover.find(p); iter != toCover.end())
								toCover.erase(iter);
							visited.insert(p);
						}
					quickOut:
					}
			}
			++result;
		}
		dir = (dir + 1) % N;
	}

	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2026S3D2.exe Part1Filename Part2Filename Part3Filename\n";
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
		int x = 0, dir = 0;
		Point me, target;
		while (in >> line)
		{
			if (auto pos = line.find('#'); pos != std::string::npos)
				target = Point(x, (int)pos);
			if (auto pos = line.find('@'); pos != std::string::npos)
				me = Point(x, (int)pos);
			++x;
		}

		std::set<Point> visited = { me };
		while (!visited.contains(target))
		{
			Point np = me + directions[dir];
			if (visited.insert(np).second)
				me = np;
			dir = (dir + 1) % 4;
		}
		std::cout << std::format("Part 1: {}\n", visited.size()-1);
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		int x = 0;
		Point me, bone;
		while (in >> line)
		{
			if (auto pos = line.find('#'); pos != std::string::npos)
				bone = Point(x, (int)pos);
			if (auto pos = line.find('@'); pos != std::string::npos)
				me = Point(x, (int)pos);
			++x;
		}

		std::set<Point> visited = { me, bone }, neighbours;
		for (Point p : directions)
			neighbours.insert(bone + p);

		std::cout << std::format("Part 2: {}\n", Surround(me, neighbours, visited, directions));
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		int x = 0;
		int minX = 0, maxX = 0, minY = 0, maxY = 0;
		Point me;
		std::vector<Point> bones;
		while (in >> line)
		{
			for (int i = 0; i < line.size(); ++i)
				if (line[i] == '#')
				{
					bones.emplace_back(x, i);
					maxY = std::max(maxY, i);
				}
			if (auto pos = line.find('@'); pos != std::string::npos)
				me = Point(x, (int)pos);
			++x;
		}
		maxX = x;

		std::set<Point> visited = { me }, neighbours, check = visited, cur = visited, next;
		visited.insert(bones.cbegin(), bones.cend());
		for (Point b : bones)
			for (Point p : directions)
				if (Point c = b + p; !visited.contains(c))
					neighbours.insert(c);
		while (!cur.empty())
		{
			for (Point p : cur)
				for (Point d : directions)
					if (Point np = p + d; np.x >= -1 && np.y >= -1 && np.x <= maxX+1 && np.y <= maxY+1 && !visited.contains(np) && check.insert(np).second)
						next.insert(np);
			std::swap(cur, next);
			next.clear();
		}
		
		std::set_intersection(neighbours.cbegin(), neighbours.cend(), check.cbegin(), check.cend(), std::inserter(next, next.cend()));
		std::swap(next, neighbours);

		constexpr std::array<Point, 12> sequence = { Point(-1, 0), Point(-1, 0), Point(-1, 0),
													 Point(0, 1), Point(0, 1), Point(0, 1), 
													 Point(1, 0), Point(1, 0), Point(1, 0), 
													 Point(0,-1), Point(0,-1), Point(0,-1) };
		std::cout << std::format("Part 3: {}\n", Surround(me, neighbours, visited, sequence));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}