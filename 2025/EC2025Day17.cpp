import std;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

[[nodiscard]] std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(s.substr(tokenStart));
			break;
		}
		results.emplace_back(s.substr(tokenStart, e - tokenStart));
		tokenStart = e + token.size();
	} while (tokenStart < s.size());

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
	constexpr Coord operator - (const Coord& p) const { return Coord(x - p.x, y - p.y); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	friend auto operator <=>(const Coord&, const Coord&) = default;

	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

struct PathVal
{
	int estimatedCost, costTo;
	Point p;
	bool operator >(const PathVal& r) const { if (estimatedCost > r.estimatedCost) return true;
	if (estimatedCost < r.estimatedCost) return false;
	if (costTo > r.costTo) return true;
	if (costTo < r.costTo) return false;
	return p > r.p;
	}
};

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	if constexpr (std::is_signed_v<T>)
		return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
	else
		return (c2.x < c1.x ? c1.x - c2.x : c2.x - c1.x) + (c2.y < c1.y ? c1.y - c2.y : c2.y - c1.y);
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day17.exe Part1Filename Part2Filename Part3Filename\n";
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
		u64 part1 = 0;
		std::vector<std::string> map;
		while (in1 >> line)
			map.emplace_back(std::move(line));

		Point center = { map.size()/2, map.front().size()/2};
		int radiusSq = 10 * 10;
		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
			{
				Point pos{ i, j }, p = pos - center;
				if (p.x * p.x + p.y * p.y <= radiusSq && pos != center)
					part1 += pos.GetAt(map) - '0';
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
		std::vector<std::string> map;
		Point center;
		while (in2 >> line)
		{
			if (auto pos = line.find('@'); pos != std::string::npos)
				center = Point{ (int)map.size(), (int)pos };
			map.emplace_back(std::move(line));
		}
	
		u64 maxLava = 0, maxRequired = center.x*center.x + center.y*center.y;
		std::vector<u64> radiusValues = {0}, lavaValues;
		int a = 1;
		while (true)
		{
			radiusValues.push_back(a * a);
			++a;
			if (radiusValues.back() >= maxRequired) break;
		}
		lavaValues.resize(radiusValues.size(), 0);		

		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
			{
				Point pos{ i, j }, p = pos - center;
				if (pos == center) continue;
				u64 r = p.x * p.x + p.y * p.y;
				auto rIter = radiusValues.begin(), lIter = lavaValues.begin();
				while (r > *rIter)
				{
					++rIter, ++lIter;
				}
				*lIter += pos.GetAt(map) - '0';
			}

		for (int i = 1; i < lavaValues.size(); ++i)
			if (lavaValues[i] > maxLava)
			{
				maxLava = lavaValues[i];
				part2 = maxLava * i;
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
		u64 part3 = 0;

		std::vector<std::string> map;
		Point volcano, start;
		while (in3 >> line)
		{
			if (auto pos = line.find('@'); pos != std::string::npos)
				volcano = Point{ (int)map.size(), (int)pos };
			if (auto pos = line.find('S'); pos != std::string::npos)
				start = Point{ (int)map.size(), (int)pos };
			map.emplace_back(std::move(line));
		}

		auto GetCost = [volcano, start, &map](Point from, Point to, int clearance) ->int 
			{
				int square = clearance * clearance;
				std::priority_queue<PathVal, std::vector<PathVal>, std::greater<PathVal>> queue;
				queue.emplace(ManhattanDistance(from, to), 0, from);

				std::map<Point, int> guard;
				int cost = 10000000;
				while (true)
				{
					if (queue.top().estimatedCost > cost)
						return cost;
					PathVal pv = queue.top();
					queue.pop();
					if (pv.p == to)
						cost = pv.costTo;
					else if (auto iter = guard.find(pv.p); iter == guard.cend() || iter->second > pv.costTo)
					{
						guard[pv.p] = pv.costTo;
						for (Point d : directions)
						{
							Point np = pv.p + d, dist = np - volcano;;
							
							if (np.Within(map) && (dist.x * dist.x + dist.y * dist.y) > square)
							{
								char c = np.GetAt(map);
								int delta = c == 'S' ? 0 : c - '0';
								queue.emplace(pv.costTo + delta + ManhattanDistance(np, to), pv.costTo + delta, np);
							}
						}
					}
				}
			};

		int radius = 30;
		while (!part3)
		{
			++radius;
			int maxTime = 30 * radius, score;
			Point left = volcano + Point(0, -radius), down = volcano + Point(radius+1, 0), right = volcano + Point(0, radius);
			score = GetCost(start, left, radius-1);
			if (score < maxTime)
				score += GetCost(left, down, radius-1);
			if (score < maxTime)
				score += GetCost(down, right, radius-1);
			if (score < maxTime)
				score += GetCost(right, start, radius-1);
			if (score < maxTime)
				part3 = (radius - 1) * score;
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}