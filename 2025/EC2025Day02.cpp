import std.core;

using u64 = unsigned long long;
using i64 = long long;

struct comp
{
	constexpr comp operator += (const comp& c) { x += c.x; y += c.y; return *this; }
	constexpr comp operator *= (const comp& c) { i64 tmp = x * c.x - y * c.y; y = x * c.y + y * c.x; x = tmp; return *this; }
	constexpr comp operator /= (const comp& c) { x /= c.x; y /= c.y; return *this; }

	i64 x = {}, y = {};
};

std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

constexpr bool Compute(comp start)
{
	comp c{ 0,0 };
	constexpr comp div{ 100000,100000 };
	for (int i = 0; i < 100; ++i)
	{
		c *= c;
		c /= div;
		c += start;
		if (std::abs(c.x) > 1000000 || std::abs(c.y) > 1000000)
			return false;
	}
	return true;
}

comp Parse(std::string line)
{
	auto split = SplitAtFirst(line.substr(3, line.size() - 4), ",");
	return comp{ std::stoll(split.first), std::stoll(split.second) };
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day02.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		in1 >> line;
		comp val= { 0,0 }, div = { 10,10 }, a=Parse(std::move(line));

		for (int i = 0; i < 3; ++i)
		{
			val *= val;
			val /= div;
			val += a;
		}
		std::cout << std::format("Part 1: [{},{}]\n", val.x, val.y);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 0;
		in2 >> line;
		comp a = Parse(std::move(line));
		
		for (int x = 0; x <= 1000; x += 10)
			for (int y = 0; y <= 1000; y += 10)
				if (Compute({ a.x + x, a.y + y }))
					++part2;

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
		in3 >> line;
		comp a = Parse(std::move(line));

		for (int x = 0; x <= 1000; x++)
			for (int y = 0; y <= 1000; y++)
				if (Compute({ a.x + x, a.y + y }))
					++part3;

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}