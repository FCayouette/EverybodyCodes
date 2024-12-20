import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int Solve(std::vector<std::string> map, Point start)
{
	int result = std::numeric_limits<int>::max();

	std::vector<Point> points = { start };
	std::map<char, std::vector<int>> herbs;
	std::set<char> types;

	for (int x = 0; x < map.size(); ++x)
	{
		const std::string& line = map[x];
		for (int i = 0; i < line.size(); ++i)
			if (line[i] >= 'A' && line[i] <= 'Z')
			{
				Point p(x, i);
				herbs[line[i]].push_back(points.size());
				points.push_back(p);
				types.insert(line[i]);
			}
	}

	auto CanPass = [&map](const Point& p) -> bool
		{
			if (p.x < 0 || p.y < 0 || p.x >= map.size() || p.y >= map.front().size())
				return false;
			char c = map[p.x][p.y];
			if (c == '#' || c == '~')
				return false;
			return true;
		};

	std::vector<std::vector<int>> distances(points.size(), std::vector<int>(points.size(), 0));
	for (int i = 0; i < points.size() - 1; ++i)
	{
		std::set<Point> toFind, cur = { points[i] }, guard = cur, next;
		for (int j = i + 1; j < points.size(); ++j)
			toFind.insert(points[j]);

		int dist = 0;
		while (!toFind.empty())
		{
			++dist;
			for (Point p : cur)
				for (Point d : directions)
					if (Point np = p + d; CanPass(np) && guard.insert(np).second)
					{
						if (auto iter = toFind.find(np); iter != toFind.cend())
						{
							int x = std::find(ALLc(points), np) - points.cbegin();
							distances[i][x] = distances[x][i] = dist;
							toFind.erase(iter);
						}
						next.insert(np);
					}
			std::swap(cur, next);
			next.clear();
		}
	}

	std::vector<char> searchOrder{ types.cbegin(), types.cend() };

	auto FindMin = y_combinator([&](auto&& FindMin, int depth, int index, int cost) ->int
		{
			if (cost + distances[0][index] > result)
				return result;
			if (depth == searchOrder.size() - 1)
				return cost + distances[0][index];
			int minCost = std::numeric_limits<int>::max();
			++depth;
			for (int i : herbs[searchOrder[depth]])
				minCost = std::min(minCost, FindMin(depth, i, cost + distances[index][i]));
			return minCost;
		});

	do
	{
		int searchResult = std::numeric_limits<int>::max();
		for (int index : herbs[searchOrder.front()])
			searchResult = std::min(searchResult, FindMin(0, index, distances[0][index]));
		result = std::min(searchResult, result);
	} while (std::next_permutation(ALL(searchOrder)));
	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day15.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::vector<std::string> map;
	std::string line;

	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int part1 = 0;
		std::getline(in1, line);
		Point start{ 0, (int)line.find('.') };
		map.push_back(std::move(line));
		while (std::getline(in1, line))
			map.push_back(std::move(line));

		std::set<Point> cur = { start }, guard = cur, next;
		bool goOn = true;
		while (goOn)
		{
			++part1;
			for (Point p : cur)
				for (Point d : directions)
				{
					Point np = p + d;
					if (np.x >= 0 && np.y >= 0 && np.x < map.size() && np.y < map.front().size() && map[np.x][np.y] != '#' && guard.insert(np).second)
					{
						if (map[np.x][np.y] == 'H')
						{
							goOn = false;
							break;
						}
						next.insert(np);
					}
				}
			std::swap(next, cur);
			next.clear();
		}
		std::cout << std::format("Part 1: {}\n", 2 * part1);
	}
	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		map.clear();
		while (std::getline(in2, line))
			map.push_back(std::move(line));

		std::cout << std::format("Part 2: {}\n", Solve(std::move(map), {0, (int)map.front().find('.')}));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	map.clear();
	std::set<char> seenTypes;
	while (std::getline(in3, line))
	{
		for (char c : line)
			if (c >= 'A' && c <= 'Z')
				seenTypes.insert(c);
		map.push_back(std::move(line));
	}

	std::vector<int> separators;
	for (int i = 1; i < map.front().size() - 2; ++i)
	{
		bool wall = true;
		for (int j = 0; wall && j < map.size() - 2; ++j)
			wall = (map[j][i] == '#' || map[j][i + 1] == '#');
		if (wall)
			separators.push_back(i);
	}
	int part3 = 0;
	for (int i = 0; i < 3; ++i)
	{
		std::vector<std::string> subMap;
		int s, len;
		if (i == 0) s = 0, len = separators.front() + 2;
		else if( i == 1) s = separators.front() + 1, len = separators.back() - s + 1;
		else s = separators.back(), len = map.front().size()-s+1;

		for (const std::string& l : map)
			subMap.push_back(l.substr(s, len));

		if (i == 1)
		{
			subMap[subMap.size() - 2][0] = (*seenTypes.crbegin()) + 1;
			subMap[subMap.size() - 2][subMap.front().size() - 1] = (*seenTypes.crbegin()) + 2;
			part3 += Solve(std::move(subMap), {0, (int)(subMap.front().find('.'))});
		}
		else
			part3 += Solve(std::move(subMap), {(int)map.size()-2, !i ? separators.front()+1: 0});
	}
	std::cout << std::format("Part 3: {}\n", part3);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}