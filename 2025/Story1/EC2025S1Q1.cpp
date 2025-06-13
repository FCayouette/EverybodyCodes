import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;

u64 Eni(u64 n, u64 times, u64 mod)
{
	std::string result;
	u64 score = 1;
	for (int i = 0; i < times; ++i)
	{
		score = (score * n) % mod;
		result = std::to_string(score) + result;
	}
	return atoll(result.c_str());
}

// From https://stackoverflow.com/questions/8496182/calculating-powa-b-mod-n
// Primality testing approach
template <std::integral T>
[[nodiscard]] constexpr T ModuloExp(T base, T exp, T mod)
{
	T result = 1;
	while (exp)
	{
		if (exp & 1)
			result = (result * base) % mod;
		base = (base * base) % mod;
		exp /= 2;
	}
	return result % mod;
}

u64 ShortEni(u64 n, u64 times, u64 mod)
{
	u64 score = ModuloExp(n, times-5, mod);
	std::string result;
	for (int i = 0; i < 5; ++i)
	{
		score = (score * n) % mod;
		result = std::to_string(score) + result;
	}
	return stoll(result);
}

constexpr u64 SumEni(u64 n, u64 times, u64 mod)
{
	std::vector<u64> cycle;
	u64 score = 1, startIndex = 0;
	while (true)
	{
		score = (score * n) % mod;
		if (auto iter = std::find(ALLc(cycle), score);
			iter != cycle.cend())
		{
			startIndex = std::distance(cycle.cbegin(), iter);
			break;
		}
		cycle.push_back(score);
	}
	u64 cycleSize = cycle.size() - startIndex;
	std::partial_sum(ALLc(cycle), cycle.begin());
	u64 windup = startIndex == 0 ? 0 : cycle[startIndex-1], repeats = (times-startIndex) / cycleSize;
	u64 result = windup + repeats * (cycle.back()-windup);
	if (u64 i = (times - startIndex) % cycleSize)
		result += cycle[i-1+startIndex] - windup;
	return result;
}

u64 GetMaxScore(std::ifstream& in, u64 (*eniMethod)(u64, u64, u64))
{
	u64 result = 0;
	std::string a, b, c, x, y, z, m;
	while (in >> a >> b >> c >> x >> y >> z >> m)
	{
		u64 M = atoll(m.c_str()+2);
		result = std::max(result, eniMethod(atoll(a.c_str() + 2), atoll(x.c_str() + 2), M) + eniMethod(atoll(b.c_str() + 2), atoll(y.c_str() + 2), M) + eniMethod(atoll(c.c_str() + 2), atoll(z.c_str() + 2), M));
	}
	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S1Q1.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	std::cout << std::format("Part 1: {}\n", GetMaxScore(in1, &Eni));

	std::ifstream in2(argv[2]);
	std::cout << std::format("Part 2: {}\n", GetMaxScore(in2, &ShortEni));

	std::ifstream in3(argv[3]);
	std::cout << std::format("Part 3: {}\n", GetMaxScore(in3, &SumEni));
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}