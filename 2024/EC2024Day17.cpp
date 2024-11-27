import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()
using i64 = long long;

template<typename T>
struct Coord
{
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x = {}, y = {};
};

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	if constexpr (std::is_signed_v<T>)
		return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
	else
		return std::max(c1.x, c2.x) - std::min(c1.x, c2.x) + std::max(c1.y, c2.y) - std::min(c1.y, c2.y);
}

using Point = Coord<int>;

std::vector<Point> GetStars(std::ifstream& in)
{
	std::vector<Point> stars;
	std::string line;
	int x = 0;
	while (in >> line)
	{
		for (int i = 0; i < line.size(); ++i)
			if (line[i] == '*')
				stars.emplace_back(x, i);
		++x;
	}
	return stars;
}

int Solve(std::vector<Point> stars)
{
	std::vector<std::vector<int>> distances(stars.size(), std::vector<int>(stars.size(), -1));
	int a, b, minDistance = std::numeric_limits<int>::max();
	for (int i = 0; i < stars.size(); ++i)
		for (int j = i + 1; j < stars.size(); ++j)
		{
			int dist = ManhattanDistance(stars[i], stars[j]);
			distances[i][j] = distances[j][i] = dist;
			if (dist < minDistance)
			{
				a = i, b = j;
				minDistance = dist;
			}
		}

	int result = minDistance + stars.size();
	std::set<int> graph;
	graph.insert(a);
	graph.insert(b);
	distances[a][b] = distances[b][a] = -1;

	std::vector<std::set<int>> subGraphs;
	subGraphs.push_back(std::move(graph));

	while (subGraphs.front().size() < stars.size())
	{
		minDistance = std::numeric_limits<int>::max();
		int groupA, groupB;
		for (int i = 0; i < stars.size(); ++i)
			for (int j = i+1; j < stars.size(); ++j)
				if (int d = distances[i][j]; d > 0 && d < minDistance)
				{
					int sgA = -1, sgB = -1, index = 0;
					for (const auto& sg : subGraphs)
					{
						if (sg.find(i) != sg.cend())
							sgA = index;
						if (sg.find(j) != sg.cend())
							sgB = index;
						++index;
					}

					if (sgA > -1 && sgB == sgA) // cycle
						distances[i][j] = distances[j][i] = -1;
					else
					{
						minDistance = distances[i][j];
						a = i, b = j;
						groupA = sgA;
						groupB = sgB;
					}

				}
		if (groupA == -1 && groupB == -1)
			subGraphs.push_back(std::set<int>{a, b});
		else if (groupA == -1)
			subGraphs[groupB].insert(a);
		else if (groupB == -1)
			subGraphs[groupA].insert(b);
		else
		{
			if (groupA > groupB)
			{
				std::swap(groupA, groupB);
				std::swap(a, b);
			}
			subGraphs[groupA].insert(ALLc(subGraphs[groupB]));
			subGraphs[groupB].clear();
		}
		result += minDistance;
	}

	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day17.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	std::cout << std::format("Part 1: {}\n", Solve(GetStars(in1)));
	

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", Solve(GetStars(in2)));
	

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::vector<Point> stars = GetStars(in3);
		std::set<int> toUse;
		for (int i = 0; i < stars.size(); ++i)
			toUse.insert(i);

		std::vector<std::vector<int>> distances(stars.size(), std::vector<int>(stars.size(), -1));
		int a, b, minDistance = std::numeric_limits<int>::max();
		for (int i = 0; i < stars.size(); ++i)
			for (int j = i + 1; j < stars.size(); ++j)
				distances[i][j] = distances[j][i] = ManhattanDistance(stars[i], stars[j]);

		std::vector<i64> constellationScores;
		while (!toUse.empty())
		{
			std::set<int> cur, toProcess{ *toUse.cbegin() };
			toUse.erase(toUse.begin());
			while (!toProcess.empty())
			{
				int p = *toProcess.cbegin();
				cur.insert(p);
				toProcess.erase(toProcess.begin());
				const auto& dists = distances[p];
				for (int i = 0; i < dists.size(); ++i)
					if (int d = dists[i]; d > 0 && d < 6 && toUse.find(i) != toUse.cend())
					{
						toProcess.insert(i);
						toUse.erase(i);
					}
			}
			
			if (cur.size() > 1)
			{
				std::vector<Point> constellation;
				for (int i : cur)
					constellation.push_back(stars[i]);
				constellationScores.push_back(Solve(std::move(constellation)));
			}
		}

		std::sort(ALL(constellationScores), std::greater<>());
		std::cout << std::format("Part 3: {}\n", std::accumulate(constellationScores.cbegin(), constellationScores.cbegin()+3, 1ll, std::multiplies<>()));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}