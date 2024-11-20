import std.core;

template<typename T>
struct Coord
{
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	T x = {}, y = {};
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day12.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::string s;
		while (in1 >> s)
			map.push_back(std::move(s));

		map.pop_back();
		std::vector<std::pair<char, Point>> launchPoints;

		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
				if (char c = map[i][j];
					c != '.' && c != 'T')
					launchPoints.push_back(std::make_pair(c, Point{ i, j }));
		
		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
				if (map[i][j] == 'T')
					for (auto& [l, start] : launchPoints)
						if (((j - start.y) + (start.x - i)) % 3 == 0)
						{
							part1 += (l -'A' + 1) * ((j - start.y) - (i - start.x)) /3;
							break;
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
		std::string s;
		while (in2 >> s)
			map.push_back(std::move(s));

		map.pop_back();
		std::vector<std::pair<char, Point>> launchPoints;

		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
				if (char c = map[i][j];
					c != '.' && c != 'T' && c != 'H')
					launchPoints.push_back(std::make_pair(c, Point{ i, j }));

		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
				if (char c = map[i][j];
				    c == 'T' || c == 'H')
					for (auto& [l, start] : launchPoints)
						if (((j - start.y) + (start.x - i)) % 3 == 0)
						{
							part2 += (1 + (c == 'H')) * (l - 'A' + 1) * ((j - start.y) - (i - start.x)) / 3;
							break;
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
		int part3 = 0;
		std::vector<Point> meteors;
		int x, y, maxVal = 0;
		while (in3 >> x >> y)
		{
			meteors.emplace_back(x, y);
			maxVal = std::max(maxVal, std::max(x, y));
		}
		maxVal = (maxVal+1) /2;
		std::array<std::vector<std::vector<int>>, 3> traces;
		for (int l = 0; l < 3; ++l)
		{
			auto& trace = traces[l];
			trace.reserve(maxVal);
			trace.push_back(std::vector<int>{});
			for (int p = 1; p < maxVal; ++p)
			{
				std::vector<int> t = {0};
				Point delta{1, 1}, pos{0, l};
				for (int i = 0; i < p; ++i)
				{
					pos += delta;
					t.push_back(pos.y);
				}
				delta = Point{1, 0};
				for (int i = 0; i < p; ++i)
				{
					pos += delta;
					t.push_back(pos.y);
				}
				delta = Point{1,-1};
				pos += delta;
				do 
				{
					t.push_back(pos.y);
					pos += delta;
				} while (pos.y >= 0);
				if (t.size() >= maxVal+1)
					t.resize(maxVal+1);
				trace.push_back(std::move(t));
			}
		}

		Point delta {-1, -1};
		for (Point m : meteors)
		{
			m += delta;
			int t = 1, minPower = std::numeric_limits<int>::max();
			while (t < m.x)
			{
				++t;
				m += delta;
			}
			bool found = false;
			while (m.x >= 0 && m.y >= 0 && !found)
			{
				for (int l = 0; l < 3; ++l)
					for (int p = 1; p < traces[l].size() && (l + 1) * p <= minPower; ++p)
						if (const auto& powerList = traces[l][p]; 
							m.x < powerList.size() && powerList[m.x] == m.y)
						{
							minPower = std::min(minPower, (l + 1) * p);
							found = true;
						}
				++t;
				m+= delta;
			}
			part3 += minPower;
		}

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}