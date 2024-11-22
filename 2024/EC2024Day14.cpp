import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	auto pos = s.find(token), tSize = token.size();
	while (pos != std::string::npos)
	{
		results.push_back(s.substr(0, pos));
		s = s.substr(std::min(pos + tSize, s.length()));
		pos = s.find(token);
	}
	if (!s.empty())
		results.push_back(std::move(s));
	return results;
}

template<typename T>
struct Coord3D
{
	constexpr bool operator <  (const Coord3D& p) const { if (x < p.x) return true; if (p.x < x) return false; if (y < p.y) return true; if (p.y < y) return false; return z < p.z; }
	constexpr Coord3D operator + (const Coord3D& p) const { return Coord3D(x + p.x, y + p.y, z + p.z); }
	constexpr Coord3D& operator+=(const Coord3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	constexpr Coord3D operator * (T scalar) const { return Coord3D(x * scalar, y * scalar, z * scalar); }
	constexpr bool operator==(const Coord3D& p) const { return x == p.x && y == p.y && z == p.z; }
	T x = {}, y = {}, z = {};
};

using Point = Coord3D<int>;
constexpr std::array<char, 6> dir = {'U', 'D', 'R', 'L', 'F', 'B'};
constexpr std::array<Point, 6> pointDir = {Point(1,0,0), Point(-1,0,0), Point(0,-1, 0), Point(0,1,0), Point(0,0,1), Point(0,0,-1)};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day14.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int part1 = 0, pos = 0;

		std::string line;
		in1 >> line;
		std::vector<std::string> insts = SplitAt(std::move(line), ",");

		for (const std::string& s : insts)
			if (int index = std::find(ALLc(dir), s[0]) - dir.cbegin();
				index < 2)
			{
				pos += std::stoi(s.substr(1)) * (index == 0 ? 1 : -1);
				part1 = std::max(part1, pos);
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
		std::string line;
		std::set<Point> segments;
		while (in2 >> line)
		{
			Point p{ 0,0,0 };
			for (const std::string& s : SplitAt(std::move(line), ","))
			{
				Point d = pointDir[std::find(ALLc(dir), s[0]) - dir.cbegin()];
				for (int i = 0, delta = std::stoi(s.substr(1)); i < delta; ++i)
				{
					p += d;
					segments.insert(p);
				}
			}
		}

		std::cout << std::format("Part 2: {}\n", segments.size());
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		int part3 = std::numeric_limits<int>::max(), maxHeight = 0, minHeight = part3;

		std::set<Point> leaves, segments;
		std::string line;
		while (in3 >> line)
		{
			Point p{ 0,0,0 };
			for (const std::string& s : SplitAt(std::move(line), ","))
			{
				Point d = pointDir[std::find(ALLc(dir), s[0]) - dir.cbegin()];
				for (int i = 0, delta = std::stoi(s.substr(1)); i < delta; ++i)
				{
					p += d;
					segments.insert(p);
				}
			}
			maxHeight = std::max(p.x, maxHeight);
			minHeight = std::min(p.x, minHeight);
			leaves.insert(p);
		}

		Point trunk = pointDir[0] * (minHeight+1);
		for (int i = minHeight+1; i < maxHeight; ++i, trunk += pointDir[0])
			if (segments.find(trunk) != segments.cend())
			{
				std::set<Point> toFind = leaves, cur = {trunk}, guard = cur, next;
				int dist = 0, murkiness = 0;
				while (!toFind.empty() && (++dist * toFind.size() + murkiness) < part3)
				{
					for (Point p : cur)
						for (Point d : pointDir)
							if (Point np = d + p;
								segments.find(np) != segments.cend() && guard.insert(np).second)
							{
								next.insert(np);
								if (auto iter = toFind.find(np);
									iter != toFind.cend())
								{
									murkiness += dist;
									toFind.erase(iter);
								}
							}

					std::swap(next, cur);
					next.clear();
				}
				if (toFind.empty())
					part3 = std::min(part3, murkiness);
			}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}