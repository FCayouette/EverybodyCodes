import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

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
constexpr std::array<Point, 8> moves = { Point(-2, -1), Point(-2, 1), Point(-1, -2), Point(-1,2), Point(1, -2), Point(1, 2), Point(2, -1), Point(2,1) };

using Memo = std::map<u64, u64>;
using DP = std::pair<Memo, Memo>;
u64 MoveDragon(const std::vector<Point>& hides, Point dragon, const std::vector<Point>& sheeps, Point boardSize, DP& memo);

u64 Encode(const std::vector<Point>& sheeps, Point dragon)
{
	u64 code = (dragon.x+1)*8 + (dragon.y+1);
	for (Point s : sheeps)
		code = (code << 6) | ((s.x + 1) * 8 + (s.y + 1));

	return code;
}

u64 Search(const std::vector<Point>& hides, Point dragon, const std::vector<Point>& sheeps, Point boardSize, DP& memo)
{
	u64 state = Encode(sheeps, dragon), result = 0;
	if (auto iter = memo.first.find(state);
		iter != memo.first.cend())
		return iter->second;

	bool couldMove = false;
	for (int i = 0; i < sheeps.size(); ++i)
	{
		Point cur = sheeps[i], np = cur + Point(1, 0);
		if (np.x >= boardSize.x)
		{
			couldMove = true;
			continue;
		}
		bool onHide = std::binary_search(ALLc(hides), np);
		if (onHide || np != dragon)
		{
			couldMove = true;
			// Avoid searching paths where the sheep won't be catchable ever again
			if (onHide)
			{
				bool safe = true;
				for (int i = np.x + 1; safe && i < boardSize.x; ++i)
					safe = std::binary_search(ALLc(hides), Point{i, np.y});
				if (safe) 
					continue;
			}

			std::vector<Point> ns = sheeps;
			ns[i] = np;
			std::sort(ALL(ns));
			result += MoveDragon(hides, dragon, ns, boardSize, memo);
		}
	}
	if (!couldMove)
		result += MoveDragon(hides, dragon, sheeps, boardSize, memo);
	
	memo.first[state] = result;
	return result;
}

u64 MoveDragon(const std::vector<Point>& hides, Point dragon, const std::vector<Point>& sheeps, Point boardSize, DP& memo)
{

	u64 state = Encode(sheeps, dragon), result = 0;
	if (auto iter = memo.second.find(state);
		iter != memo.second.cend())
		return iter->second;
	for (Point m : moves)
		if (Point np = dragon + m; np.x >= 0 && np.y >= 0 && np.x < boardSize.x && np.y < boardSize.y)
		{
			if (std::binary_search(ALLc(hides), np))
				result += Search(hides, np, sheeps, boardSize, memo);
			else
			{
				bool found = false;
				for (int i = 0; i < sheeps.size() && !found; ++i)
					if (sheeps[i] == np)
					{
						found = true;
						std::vector<Point> ns = sheeps;
						if (i != ns.size() - 1)
						{
							std::swap(ns[i], ns[ns.size() - 1]);
							ns.pop_back();
							std::sort(ALL(ns));
						}
						else
							ns.pop_back();

						if (ns.empty()) 
							++result;
						else
							result += Search(hides, np, ns, boardSize, memo);
					}
				if (!found)
					result += Search(hides, np, sheeps, boardSize, memo);
			}
		}
	memo.second[state] = result;
	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day10.exe Part1Filename Part2Filename Part3Filename\n";
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

		std::vector<std::string> board;
		Point start;
		while (in1 >> line)
		{
			if (auto d = line.find('D');
				d != std::string::npos)
				start = Point(board.size(), d);
			board.emplace_back(std::move(line));
		}

		std::set<Point> all = { start }, cur = all, next;

		for (int i = 0; i < 4; ++i)
		{
			for (Point p : cur)
				for (Point d : moves)
					if (Point np = p + d; np.Within(board) && all.insert(np).second)
						next.insert(np);
			
			std::swap(cur, next);
			next.clear();
		}

		for (Point p : all)
			if (p.GetAt(board) == 'S')
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
		u64 part2 = 0;

		std::vector<std::string> board;
		std::vector<Point> hides, sheeps, dragons, next;
		int count = 0;
		while (in2 >> line)
		{
			for (int i = 0; i < line.size(); ++i)
			{
				if (char c = line[i]; c == 'D')
					dragons.emplace_back(count, i);
				else if (c == 'S')
					sheeps.emplace_back(count, i);
				else if (c == '#')
					hides.emplace_back(count, i);
			}
			++count;
			board.emplace_back(std::move(line));
		}

		for (int turn = 0; turn < 20; ++turn)
		{
			for (Point p : dragons)
				for (Point d : moves)
					if (Point np = p + d; np.Within(board))
						next.emplace_back(np);
			std::sort(ALL(next));
			next.erase(std::unique(ALL(next)), next.end());
			std::swap(dragons, next);
			next.clear();
			for (Point p : dragons)
				if (!std::binary_search(ALLc(hides), p))
					if (auto iter = std::find(ALL(sheeps), p); iter != sheeps.end())
					{
						iter->x = board.size() + 1;
						++part2;
					}
			for (Point& s : sheeps)
				if (s.x < board.size())
				{
					++s.x;
					if (!std::binary_search(ALLc(hides), s) && std::binary_search(ALLc(dragons), s))
					{
						s.x = board.size() + 1;
						++part2;
					}
				}	
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

		std::vector<Point> hides, sheeps;
		Point dragon;
		int count = 0, width=0;
		while (in3 >> line)
		{
			for (int i = 0; i < line.size(); ++i)
			{
				if (char c = line[i]; c == 'D')
					dragon = { count, i };
				else if (c == 'S')
					sheeps.emplace_back(count, i);
				else if (c == '#')
					hides.emplace_back(count, i);
			}
			++count;
			width = line.size();
		}
		DP memo;
		part3 = Search(hides, dragon, sheeps, Point(count, width), memo);

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}