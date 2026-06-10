import std;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

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

struct TreeNode
{
	int id = {};
	std::string info;
	std::pair<std::string, std::string> wp, wl, wr;
	int cL = 0, cR = 0;
	bool strongLeft = false, strongRight = false;

	void ReadNode(std::string line)
	{
		std::vector<std::string> data = SplitAt(std::move(line), ", ");
		id = std::stoi(SplitAtFirst(data[0], "=").second);
		std::string plug = SplitAtFirst(data[1], "=").second, 
		left = SplitAtFirst(data[2], "=").second, 
		right = SplitAtFirst(data[3], "=").second;
		wp = SplitAtFirst(plug, " ");
		wl = SplitAtFirst(left, " ");
		wr = SplitAtFirst(right, " ");
		info = SplitAtFirst(data[4], "=").second;
	}
};

enum class PlacementMode : char
{
	Strong,
	Weak,
	Displacing
};

bool PlaceNode(std::vector<TreeNode>& nodes, int& nIndex, int curIndex, PlacementMode mode)
{
	TreeNode& c = nodes[curIndex];
	TreeNode& n = nodes[nIndex];
	int match = (c.wl.first == n.wp.first) + (int)(c.wl.second == n.wp.second);
	bool skip = false;
	if (c.cL == 0)
	{
		if ((match == 1 && mode != PlacementMode::Strong) || match == 2)
		{
			c.strongLeft = (match == 2);
			c.cL = n.id;
			return true;
		}
	}
	else
	{
		if (match == 2 && !c.strongLeft && mode == PlacementMode::Displacing)
		{
			c.strongLeft = true;
			nIndex = c.cL - 1;
			c.cL = n.id;
			skip = true;
		}
	}
	if (!skip && c.cL && PlaceNode(nodes, nIndex, c.cL - 1, mode))
		return true;

	match = (c.wr.first == nodes[nIndex].wp.first) + (int)(c.wr.second == nodes[nIndex].wp.second);
	skip = false;
	if (c.cR == 0)
	{
		if (match == 2 || (match == 1 && mode != PlacementMode::Strong))
		{
			c.strongRight = (match == 2);
			c.cR = nodes[nIndex].id;
			return true;
		}
	}
	else
	{
		if (match == 2 && !c.strongRight && mode == PlacementMode::Displacing)
		{
			c.strongRight = true;
			int oldcr = c.cR;
			c.cR = nodes[nIndex].id;
			nIndex = oldcr - 1;
			skip = true;
		}
	}
	if (!skip && c.cR && PlaceNode(nodes, nIndex, c.cR - 1, mode))
		return true;
	return false;
}
void IdList(std::vector<TreeNode>& tree, std::vector<int>& list, int cur)
{
	const TreeNode& n = tree[cur - 1];
	if (n.cL)
		IdList(tree, list, n.cL);
	list.push_back(n.id);
	if (n.cR)
		IdList(tree, list, n.cR);
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2026S3D3.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;

	{
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
			return -1;
		}
		int part1 = 0;
	
		std::vector<TreeNode> nodes;
		while (std::getline(in, line))
		{
			TreeNode n;
			n.ReadNode(std::move(line));
			int index = nodes.size();
			nodes.push_back(std::move(n));
			//SetNode(nodes, nodes.size() - 1, 0);
			PlaceNode(nodes, index, 0, PlacementMode::Strong);
		}
	
		std::vector<int> idList;
		IdList(nodes, idList, nodes.front().id);
		for (int i = 0; i < idList.size(); ++i)
			part1 += (i + 1) * idList[i];
		std::cout << std::format("Part 1: {}\n", part1);
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		int part2 = 0;
		std::vector<TreeNode> nodes;
		while (std::getline(in, line))
		{
			TreeNode n;
			n.ReadNode(std::move(line));
			int index = nodes.size();
			nodes.push_back(std::move(n));
			PlaceNode(nodes, index, 0, PlacementMode::Weak);
		}

		std::vector<int> idList;
		IdList(nodes, idList, nodes.front().id);
		for (int i = 0; i < idList.size(); ++i)
			part2 += (i + 1) * idList[i];

		std::cout << std::format("Part 2: {}\n", part2);
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		int part3 = 0;

		std::vector<TreeNode> nodes;
		while (std::getline(in, line))
		{
			TreeNode n;
			n.ReadNode(std::move(line));
			int index = nodes.size();
			nodes.push_back(std::move(n));
			if (index)
				while (!PlaceNode(nodes, index, 0, PlacementMode::Displacing)) {}
		}

		std::vector<int> idList;
		IdList(nodes, idList, nodes.front().id);
		for (int i = 0; i < idList.size(); ++i)
		{
			//std::cout << nodes[idList[i] - 1].info << '\n';
			part3 += (i + 1) * idList[i];
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}