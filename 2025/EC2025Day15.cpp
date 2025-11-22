import std;

#define ALL(x) (x).begin(),(x).end()

using u64 = unsigned long long;

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
	constexpr Coord operator * (T scalar) const { return Coord(x * scalar, y * scalar); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

template <typename T>
[[nodiscard]] constexpr bool Within(T min, T val, T max)
{
	return min <= val && val <= max;
}

u64 Search(std::string&& line)
{
	auto insts = SplitAt(std::move(line), ",");

	int dir = 2;
	Point start{ 0,0 }, cur = start;
	using Line = std::pair<Point, Point>;
	std::vector<Line> vertLines, horLines;
	std::vector<int> ix = { -1, 0, 1 }, iy = { 0 };
	bool first = true, wentRight = true;
	for (const auto& s : insts)
	{
		dir = (dir + (s[0] == 'L' ? 1 : 3)) % 4;

		Point d = directions[dir], end = cur + d * std::stoi(s.substr(1));
		if (first)
		{
			first = false;
			wentRight = s[0] == 'R';
			cur.y += d.y;
		}
		if (dir % 2 == 0)
		{
			vertLines.emplace_back(Point(std::min(cur.x, end.x), cur.y), Point(std::max(cur.x, end.x), cur.y));
			ix.push_back(end.x - 1);
			ix.push_back(end.x + 1);
		}
		else
		{
			horLines.emplace_back(Point(cur.x, std::min(cur.y, end.y)), Point(cur.x, std::max(cur.y, end.y)));
			iy.push_back(end.y + 1);
			iy.push_back(end.y - 1);
		}
		cur = end;
	}
	ix.push_back(cur.x);
	iy.push_back(cur.y);
	std::sort(ALL(ix));
	std::sort(ALL(iy));
	ix.erase(std::unique(ALL(ix)), ix.end());
	iy.erase(std::unique(ALL(iy)), iy.end());

	const Point target = cur;
	std::map<Point, u64> processed;
	std::unordered_map<int, std::vector<int>::const_iterator> reverseX, reverseY;
	for (auto iter = ix.cbegin(); iter != ix.cend(); ++iter)
		reverseX[*iter] = iter;
	for (auto iter = iy.cbegin(); iter != iy.cend(); ++iter)
		reverseY[*iter] = iter;
	std::vector<std::pair<Point, u64>> phase, next;
	phase.emplace_back( start, 0ull);

	const auto ex = std::prev(ix.cend()), ey = std::prev(iy.cend());
	int rounds = 0;
	while (processed.find(target) == processed.cend())
	{
		++rounds;
		for (const auto& si : phase)
		{
			auto iterX = reverseX[si.first.x], iterY = reverseY[si.first.y] ;
			if (iterX != ix.cbegin())
			{
				Point n = { *std::prev(iterX), si.first.y };
				bool valid = true;
				if (n != target)
					for (const Line& l : horLines)
						if (Within(n.x, l.first.x, si.first.x) && Within(l.first.y, n.y, l.second.y))
						{ valid = false; break;	}
				if (valid)
					next.emplace_back(n, si.second + si.first.x - n.x);
			}
			if (iterX != ex)
			{
				Point n = { *std::next(iterX), si.first.y };
				bool valid = true;
				if (n != target)
					for (const Line& l : horLines)
						if (Within(si.first.x, l.first.x, n.x) && Within(l.first.y, n.y, l.second.y))
						{ valid = false; break; }

				if (valid)
					next.emplace_back(n, si.second + n.x - si.first.x);
			}
			if (iterY != iy.cbegin() && (si.second || wentRight))
			{
				Point n = { si.first.x, *std::prev(iterY) };
				bool valid = true;
				if (n != target)
					for (const Line& l : vertLines)
						if (Within(n.y, l.first.y, si.first.y) && Within(l.first.x, n.x, l.second.x))
						{ valid = false; break; }
				if (valid)
					next.emplace_back(n, si.second + si.first.y - n.y);
			}
			if (iterY != ey && (si.second || !wentRight))
			{
				Point n = { si.first.x, *std::next(iterY) };
				bool valid = true;
				if (n != target)
					for (const Line& l : vertLines)
						if (Within(si.first.y, l.first.y, n.y) && (Within(l.first.x, n.x, l.second.x)))
						{ valid = false; break; }
				if (valid)
					next.emplace_back(n, si.second + n.y - si.first.y);
			}
		}

		phase.clear();
		std::sort(ALL(next));
		for (const auto& searchInfo : next)
			if (auto iter = processed.find(searchInfo.first); iter == processed.cend() || iter->second > searchInfo.second)
			{
				processed[searchInfo.first] = searchInfo.second;
				phase.push_back(searchInfo);
			}
		next.clear();
	}
	return processed[target];
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day15.exe Part1Filename Part2Filename Part3Filename\n";
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
		in1 >> line;
		std::cout << std::format("Part 1: {}\n", Search(std::move(line)));
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		in2 >> line;
		std::cout << std::format("Part 2: {}\n", Search(std::move(line)));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		in3 >> line;
		std::cout << std::format("Part 3: {}\n", Search(std::move(line)));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}