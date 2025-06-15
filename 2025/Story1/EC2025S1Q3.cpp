import std.core;

using u64 = unsigned long long;

template<typename T>
struct Coord
{
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	T x = {}, y = {};
};
using Point = Coord<u64>;

Point GetCoord(const std::string& x, const std::string& y)
{
	return Point(stoll(x.substr(2)), stoll(y.substr(2)));
}

// From https://www.geeksforgeeks.org/multiply-large-integers-under-large-modulo/
template <std::integral T>
[[nodiscard]] constexpr T ModuloMul(T a, T b, T mod)
{
	T result = 0;
	a %= mod;
	while (b)
	{
		if (b & 1)
			result = (result + a) % mod;
		a = (2 * a) % mod;
		b /= 2;
	}
	return result;
}

// From https://stackoverflow.com/questions/8496182/calculating-powa-b-mod-n
// Primality testing approach
// Modified to avoid multiplication overflow
template <std::integral T>
[[nodiscard]] constexpr T ModuloExp(T base, T exp, T mod)
{
	T result = 1;
	while (exp)
	{
		if (exp & 1)
			result = ModuloMul(result, base, mod);
		base = ModuloMul(base, base, mod);
		exp /= 2;
	}
	return result % mod;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S1QX.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		u64 part1 = 0;
		std::vector<Point> points;
		std::string x, y;
		while (in1 >> x >> y)
			points.push_back(GetCoord(x, y));

		for (int i = 0; i < 100; ++i)
			for (Point& p : points)
			{
				if (p.y == 1)
					std::swap(p.x, p.y);
				else
					p += Point(1, -1);
			}

		for (const Point& p : points)
			part1 += p.x + 100 * p.y;

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
		std::vector<Point> cycles;
		std::string x, y;

		while (in2 >> x >> y)
		{
			Point p = GetCoord(x, y);
			cycles.push_back(Point(p.x + p.y - 1, p.y - 1));
		}
		
		Point cycleOffset = cycles.front();
		for (int i = 1; i < cycles.size(); ++i)
		{
			u64 a = 0, b = 0;
			u64 c1 = cycleOffset.x, t1 = cycleOffset.y;
			u64 c2 = cycles[i].x, t2 = cycles[i].y;
			while (t1 != t2)
			{
				while (t1 < t2)
				{
					++a;
					t1 += c1;
				}
				while (t2 < t1)
				{
					++b;
					t2 += c2;
				}
			}
			cycleOffset.x = c1 * c2;
			cycleOffset.y = t1;
		}

		std::cout << std::format("Part 2: {}\n", cycleOffset.y);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::string x, y;
		in3 >> x >> y;
		Point p = GetCoord(x, y);
		u64 cycle = p.x + p.y - 1, remainder = p.y - 1, totient = cycle-1, multiples = 1;
		while (in3 >> x >> y)
		{
			p = GetCoord(x, y);
			u64 c1 = cycle, r1 = remainder, c2 = p.x + p.y - 1, r2 = p.y - 1;
			cycle *= c2;
			remainder = (ModuloMul(r1, ModuloMul(ModuloExp(c2, totient - 1, c1), c2, cycle), cycle) + // Inverse by Euler's theorem
				ModuloMul(r2, ModuloMul(ModuloExp(c1, c2 - 2, c2), c1, cycle), cycle)) % cycle; // Inverse by Fermat's little theorem
			multiples = multiples * (c2 - 1) + c1;
			totient = cycle - multiples;
		}
		std::cout << std::format("Part 3: {}\n", remainder);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}