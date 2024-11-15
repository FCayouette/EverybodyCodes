import std.core;

#define ALL(x) (x).begin(),(x).end()
using u64 = unsigned long long;
using i64 = long long;

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; if (p.x < x) return false; return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	constexpr bool operator!=(const Coord& p) const { return x != p.x || y != p.y; }
	T x={}, y={};
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

constexpr int Power(char c)
{
	return c == '+' ? 1 : c == '-' ? -1 : 0;
}

i64 RunTrack(const std::vector<int>& track, const std::vector<int>& deltas, int laps)
{
	i64 total = 0, power = 10;
	for (int loop = 0, i = 0; loop < laps; ++loop)
		for (int t : track)
		{
			power = std::max(0ll, power + (!t ? deltas[i % deltas.size()] : t));
			++i;
			total += power;
		}
	return total;
}

std::vector<int> GetDeltas(std::string s)
{
	std::vector<int> result;
	for (int i = 0; i < s.length(); i +=2)
		result.push_back(Power(s[i]));
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day07.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::string line, part1 = "";
		std::vector<std::pair<i64, std::string>> plans;
		std::vector<int> track = {0};
		while (in1 >> line)
		{
			auto pos = line.find(':');
			std::vector<int> deltas = GetDeltas(line.substr(pos+1));
			plans.emplace_back(RunTrack(track, deltas, 10), line.substr(0, pos));
		}
		std::sort(ALL(plans), std::greater<>{});
		std::cout << std::format("Part 1: {}\n", std::accumulate(plans.cbegin(), plans.cend(), std::string{}, [](std::string a, auto b){ return a + b.second; }));
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::string line;
		std::vector<std::pair<int, std::string>> plans;
		std::vector<int> track;

		std::string s = "-=++=-==++=++=-=+=-=+=+=--=-=++=-==++=-+=-=+=-=+=+=++=-+==++=++=-=-=---=++==-";
		std::string r = "--==++++==+=+++-=+=-=+=-+-=+-=+-=+=-=+=--=+++=++=+++==++==--=+=++==+++-";
		std::reverse(ALL(r));
		s += r;
		s += "-=+=+=-S";
		for (char c : s)
			track.push_back(Power(c));

		while (in2 >> line)
		{
			auto pos = line.find(':');
			std::vector<int> deltas = GetDeltas(line.substr(pos + 1));
			plans.emplace_back(RunTrack(track, deltas, 10), line.substr(0, pos));
		}
		std::sort(ALL(plans), std::greater<>{});
		std::cout << std::format("Part 2: {}\n", std::accumulate(plans.cbegin(), plans.cend(), std::string{}, [](std::string a, auto b){ return a + b.second; }));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		constexpr std::array<std::string_view, 10> raceTrack = 
		{
			"S+= +=-== +=++=     =+=+=--=    =-= ++=     +=-  =+=++=-+==+ =++=-=-=--",
			"- + +   + =   =     =      =   == = - -     - =  =         =-=        -",
			"= + + +-- =-= ==-==-= --++ +  == == = +     - =  =    ==++=    =++=-=++",
			"+ + + =     +         =  + + == == ++ =     = =  ==   =   = =++=       ",
			"= = + + +== +==     =++ == =+=  =  +  +==-=++ =   =++ --= + =          ",
			"+ ==- = + =   = =+= =   =       ++--          +     =   = = =--= ==++==",
			"=     ==- ==+-- = = = ++= +=--      ==+ ==--= +--+=-= ==- ==   =+=    =",
			"-               = = = =   +  +  ==+ = = +   =        ++    =          -",
			"-               = + + =   +  -  = + = = +   =        +     =          -",
			"--==++++==+=+++-= =-= =-+-=  =+-= =-= =--   +=++=+++==     -=+=++==+++-"
		};

		std::set<Point> seen;
		std::vector<int> track;
		Point cur = {0,1}, origin = {0,0};
		auto toPower = [&raceTrack](Point& p) { return Power(raceTrack[p.x][p.y]); };
		track.push_back(toPower(cur));
		seen.insert(cur);
		cur += directions[1];
		seen.insert(cur);
		do
		{
			track.push_back(toPower(cur));
			for (const Point& d : directions)
				if (Point n = cur + d; 
					n.x >= 0 && n.y >= 0 && n.x < 10 && n.y < raceTrack.front().size() && 
					raceTrack[n.x][n.y] != ' ' && seen.insert(n).second)
					{	
						cur = n;
						break;
					}
		} while (cur != origin);
		track.push_back(0);
		
		std::string line, perm = "===---+++++";
		in3 >> line;
		std::vector<int> deltas = GetDeltas(line.substr(line.find(':') + 1));
		
		i64 toBeat = RunTrack(track, deltas, 11), part3 = 0;
		
		std::sort(ALL(perm));
		do 
		{
			deltas.clear();
			for (char c : perm)
				deltas.push_back(Power(c));
			
			part3 += RunTrack(track, deltas, 11) > toBeat;
		} while (std::next_permutation(ALL(perm)));
		std::cout << std::format("Part 3: {}\n", part3);
		//worst : =++++--=-+=
		//best  : ==---++++=+
	}
	return 0;
}
