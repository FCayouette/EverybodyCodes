import std.core;

template<typename T>
struct Coord
{
	constexpr bool  operator < (const Coord& p) const { if (x < p.x) return true; if (p.x < x) return false; return y < p.y; }
	constexpr bool  operator > (const Coord& p) const { if (x > p.x) return true; if (p.x > x) return false; return y > p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr bool  operator ==(const Coord& p) const { return x == p.x && y == p.y; }
	T x = {}, y = {};
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	if constexpr (std::is_signed_v<T>)
		return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
	else
		return std::max(c1.x, c2.x) - std::min(c1.x, c2.x) + std::max(c1.y, c2.y) - std::min(c1.y, c2.y);
}

template <typename Info_t, typename Cost_t, bool Unique>
struct AStarInfo
{
	constexpr AStarInfo(const Info_t& i, Cost_t c) : info(i), cost(c), cost_plus_Eval{} {}
	constexpr AStarInfo(const Info_t& i, Cost_t c, Cost_t eval) : info(i), cost(c), cost_plus_Eval(c + eval) {}

	constexpr bool operator > (const AStarInfo& right) const { return cost_plus_Eval > right.cost_plus_Eval || (cost_plus_Eval == right.cost_plus_Eval && info > right.info); }
	constexpr bool RepeatSetCheck(const AStarInfo& right) const
	{
		if constexpr (Unique) return info < right.info;
		else return info < right.info || (info == right.info && cost < right.cost);
	}
	Info_t info;
	Cost_t cost, cost_plus_Eval;
};

template <typename ASI, typename Expand, typename Estimate, typename TargetEquality>
class AStar
{
public:
	using Cost_t = decltype(ASI::cost);

	AStar(const ASI& start, const Expand& ex, const Estimate& es, const TargetEquality& eq) : expand(ex), estimate(es), equality(eq)
	{
		Initialize(std::vector<ASI>{start});
	}

	AStar(const std::vector<ASI>& start, const Expand& ex, const Estimate& es, const TargetEquality& eq) : expand(ex), estimate(es), equality(eq)
	{
		Initialize(start);
	}

	void Initialize(const ASI& start)
	{
		Initialize(std::vector<ASI>{start});
	}

	void Initialize(const std::vector<ASI>& start)
	{
		std::priority_queue<ASI, std::vector<ASI>, std::greater<ASI>> empty;
		std::swap(toEvaluate, empty);
		repeatGuard.clear();
		for (const ASI& asi : start)
			toEvaluate.push(asi);
	}

	std::optional<Cost_t> Evaluate()
	{
		while (!toEvaluate.empty())
		{
			ASI current = toEvaluate.top();
			if (equality(current))
				return current.cost;

			toEvaluate.pop();
			if (repeatGuard.insert(current).second)
			{
				auto toAdd = expand(current);
				for (ASI& a : toAdd)
				{
					a.cost_plus_Eval = a.cost + estimate(a);
					toEvaluate.push(a);
				}
			}
		}
		return std::nullopt;
	}

protected:
	struct Comparator { bool operator()(const ASI& l, const ASI& r) const { return l.RepeatSetCheck(r); } };
	std::set<ASI, Comparator> repeatGuard;
	std::priority_queue<ASI, std::vector<ASI>, std::greater<ASI>> toEvaluate;
	Expand expand;
	Estimate estimate;
	TargetEquality equality;
};

constexpr bool IsValid(char c)
{
	return c >= '0' && c <='9' || c == 'S' || c == 'E';
}

int ToVal(char c)
{
	if (std::isdigit(c))
		return c-'0';
	return 0;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day13.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}

	std::vector<std::string> map;
	using ASI = AStarInfo<Point, int, true>;
	auto Expand = [&map](const ASI& asi)
	{
		const Point& p = asi.info;
		std::vector<ASI> result;
		for (Point d : directions)
			if (Point np = p + d;
				np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size() && IsValid(map[np.x][np.y]))
			{
				int a = ToVal(map[p.x][p.y]), b = ToVal(map[np.x][np.y]);
				result.emplace_back(np, asi.cost + std::min(std::abs(a - b), 10 + std::min(a, b) - std::max(a, b)) + 1);
			}
		return result;
	};

	for (int inputFile = 1; inputFile < 4; ++inputFile)
	{
		std::ifstream in(argv[inputFile]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[inputFile]);
			return -1;
		}
		{
			map.clear();
			std::string s;
			std::vector<Point> starts;
			Point end;
			while (std::getline(in, s))
			{
				auto pos = s.find('S');
				while (pos != std::string::npos)
				{
					starts.emplace_back((int)map.size(), (int)pos);
					pos = s.find('S', pos + 1);
				}
				pos = s.find('E');
				if (pos != std::string::npos)
					end = { (int)map.size(), (int)pos };

				map.push_back(std::move(s));
			}

			std::vector<ASI> startPoints;
			for (Point p : starts)
				startPoints.emplace_back(p, 0, ManhattanDistance(p, end));

			AStar aStar{ startPoints, Expand, [end](const ASI& p) { return ManhattanDistance(p.info, end); }, [end](const ASI& p) { return p.info == end; } };

			std::cout << std::format("Part {}: {}\n", inputFile, *aStar.Evaluate());
		}
	}
	
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}