import std.core;

using i64 = long long;

template<typename T>
struct Coord
{
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 8> deltas = {Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0, 1), Point(1, 1), Point(1, 0), Point(1, -1), Point(0, -1) };

std::string Solve(std::ifstream& in, int loops = 1)
{
	std::string line, ops;
	std::vector<std::string> message;

	std::getline(in, ops);
	while (in >> line)
		message.push_back(std::move(line));

	for (int n = 0, opIndex = 0; n < loops; ++n, opIndex = 0)
		for (int i = 1; i < message.size() - 1; ++i)
			for (int j = 1; j < message.front().size() - 1; ++j)
			{
				Point p {i, j};
				std::array<char, 8> c;
				for (int i = 0; i < 8; ++i)
				{
					Point np = p + deltas[i];
					c[i] = message[np.x][np.y];
				}
				if (ops[opIndex++ % ops.size()] == 'R')
					std::rotate(c.rbegin(), c.rbegin() + 1, c.rend());
				else
					std::rotate(c.begin(), c.begin() + 1, c.end());
				for (int i = 0; i < 8; ++i)
				{
					Point np = p + deltas[i];
					message[np.x][np.y] = c[i];
				}
			}

	for (const std::string& s : message)
		if (auto pos = s.find('>'); pos != std::string::npos)
			return s.substr(pos+1, s.find('<') - (pos+1));
	return "";
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day19.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	std::cout << std::format("Part 1: {}\n", Solve(in1));

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", Solve(in2, 100));

	using Remap = std::vector<std::vector<Point>>;
	auto Add = [](const Remap& a, const Remap& b)
	{
		Remap result = a;
		for (int i = 0; i < a.size(); ++i)
			for (int j = 0; j < a.front().size(); ++j)
			{
				const Point& p = a[i][j];
				result[i][j] = b[p.x][p.y];
			}
		return result;
	};

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::string line, ops;
		std::vector<std::string> message, work;
		
		Remap takeFrom;
		std::getline(in3, ops);
		while (in3 >> line)
		{
			std::vector<Point> points;
			points.reserve(line.size());
			for (int i = 0, x = message.size(); i < line.size(); ++i)
				points.emplace_back(x, i);
			message.push_back(std::move(line));
			takeFrom.push_back(std::move(points));
		}
		work = message;

		for (int i = 1, opIndex = 0; i < takeFrom.size() - 1; ++i)
			for (int j = 1; j < takeFrom.front().size() - 1; ++j)
			{
				Point p{i, j};
				std::array<Point, 8> c;
				for (int i = 0; i < 8; ++i)
				{
					Point np = p + deltas[i];
					c[i] = takeFrom[np.x][np.y];
				}
				if (ops[opIndex++ % ops.size()] == 'R')
					std::rotate(c.rbegin(), c.rbegin() + 1, c.rend());
				else
					std::rotate(c.begin(), c.begin() + 1, c.end());
				for (int i = 0; i < 8; ++i)
				{
					Point np = p + deltas[i];
					takeFrom[np.x][np.y] = c[i];
				}
			}

		i64 toDo = 1'048'576'000ll;
		while (toDo)
		{
			if (toDo % 2ll)
			{
				for (int i = 0; i < message.size(); ++i)
					for (int j = 0; j < message.front().size(); ++j)
					{
						const Point& p = takeFrom[i][j];
						work[i][j] = message[p.x][p.y];
					}
				std::swap(work, message);
			}
			takeFrom = Add(takeFrom, takeFrom);
			toDo >>= 1;
		}
		
		for (const std::string& s : message)
			if (auto pos = s.find('>');
				pos != std::string::npos)
			{
				std::cout << std::format("Part 3: {}\n", s.substr(pos+1, s.find('<') - pos - 1));
				break;
			}
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}