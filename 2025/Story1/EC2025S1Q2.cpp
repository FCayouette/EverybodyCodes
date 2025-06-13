import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

struct Node
{
	int value, id = 0;
	char c = '0';
	
	int lower=-1, higher=-1;
	constexpr Node(std::pair<int, char>&& v) : value(v.first) , c(v.second) {}
	constexpr Node(int i, std::pair<int, char>&& v) : id(i), value(v.first), c(v.second) {}
};

std::pair<int, char> ExtractNode(std::string&& s)
{
	auto pos = s.find('[') + 1;
	std::string tmp = s.substr(pos, s.size()-pos - 1);
	pos = tmp.find(',');
	return std::make_pair(stoi(tmp.substr(0, pos)), tmp[tmp.size()-1]);
}

int ExtractID(std::string&& s)
{
	return atoi(s.c_str()+3);
}

void SetNode(std::vector<Node>& tree, int checkIndex)
{
	Node& currNode = tree[checkIndex];
	if (tree.back().value < currNode.value)
	{
		if (currNode.lower == -1)
			currNode.lower = tree.size()-1;
		else SetNode(tree, currNode.lower);
	}
	else
	{
		if (currNode.higher == -1)
			currNode.higher = tree.size()-1;
		else SetNode(tree, currNode.higher);
	}
}

void CountLayers(const std::vector<Node>& tree, int depth, int index, std::vector<int>& counts)
{
	if (counts.size() == depth)
		counts.push_back(1);
	else ++counts[depth];
	if (tree[index].lower >= 0)
		CountLayers(tree, depth+1, tree[index].lower, counts);
	if (tree[index].higher >=0)
		CountLayers(tree, depth+1, tree[index].higher, counts);
}

void PrintLayer(const std::vector<Node>& tree, int target, int depth, int index)
{
	const Node& n = tree[index];
	if (target == depth++)
	{
		std::cout << n.c;
		return;
	}
	if (n.lower >=0)
		PrintLayer(tree, target, depth, n.lower);
	if (n.higher >= 0)
		PrintLayer(tree, target, depth, n.higher);
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S1Q2.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::string l, r, part1;
		std::vector<Node> leftTree, rightTree;
		while (in1 >> l >> l >> l >> r)
		{
			Node left(ExtractNode(std::move(l))), right(ExtractNode(std::move(r)));
			bool wasEmpty = leftTree.empty();
			leftTree.push_back(left);
			rightTree.push_back(right);
			if (!wasEmpty)
			{
				SetNode(leftTree, 0);
				SetNode(rightTree, 0);
			}
		}

		std::vector<int> lHist, rHist;
		CountLayers(leftTree, 0, 0, lHist);
		CountLayers(rightTree, 0, 0, rHist);


		std::cout << "Part 1: ";
		PrintLayer(leftTree, std::distance(lHist.cbegin(), std::max_element(ALLc(lHist))), 0, 0);
		PrintLayer(rightTree, std::distance(rHist.cbegin(), std::max_element(ALLc(rHist))), 0, 0);
		std::cout << '\n';
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		std::string inst, id, l, r, part2;
		std::vector<Node> leftTree, rightTree;

		while (in2 >> inst >> id)
		{
			if (inst[0] == 'A')
			{
				in2 >> l >> r;
				int i = ExtractID(std::move(id));
				Node left(i, ExtractNode(std::move(l))), right(i, ExtractNode(std::move(r)));
				bool wasEmpty = leftTree.empty();
				leftTree.push_back(left);
				rightTree.push_back(right);
				if (!wasEmpty)
				{
					SetNode(leftTree, 0);
					SetNode(rightTree, 0);
				}
			}
			else
			{
				int toSwap = atoi(id.c_str());
				auto lIter = std::find_if(ALL(leftTree), [&toSwap](const Node& n) {return n.id == toSwap;});
				auto rIter = std::find_if(ALL(rightTree), [&toSwap](const Node& n) {return n.id == toSwap;});
				std::swap(lIter->value, rIter->value);
				std::swap(lIter->c, rIter->c);
			}
		}

		std::vector<int> lHist, rHist;
		CountLayers(leftTree, 0, 0, lHist);
		CountLayers(rightTree, 0, 0, rHist);


		std::cout << "Part 2: ";
		PrintLayer(leftTree, std::distance(lHist.cbegin(), std::max_element(ALLc(lHist))), 0, 0);
		PrintLayer(rightTree, std::distance(rHist.cbegin(), std::max_element(ALLc(rHist))), 0, 0);
		std::cout << '\n';
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::string inst, id, l, r, part3;
		int leftRoot = 0, rightRoot = 1;
		std::vector<Node> tree;

		while (in3 >> inst >> id)
		{
			if (inst[0] == 'A')
			{
				in3 >> l >> r;
				int i = ExtractID(std::move(id));
				Node left(i, ExtractNode(std::move(l))), right(i, ExtractNode(std::move(r)));
				bool wasEmpty = tree.empty();
				tree.push_back(left);
				if (!wasEmpty)
					SetNode(tree, leftRoot);
				tree.push_back(right);
				if (!wasEmpty)
					SetNode(tree, rightRoot);
			}
			else
			{
				int toSwap = atoi(id.c_str());
				if (toSwap == 1) // rootSwap
					std::swap(leftRoot, rightRoot);
				else
				{
					std::vector<std::pair<int, bool>> parentHigh;
					for (int i = 0; i < tree.size() && parentHigh.size() < 2; ++i)
					{
						const Node& n = tree[i];
						if (n.lower >= 0 && tree[n.lower].id == toSwap)
							parentHigh.emplace_back(i, false);
						if (n.higher >= 0 && tree[n.higher].id == toSwap)
							parentHigh.emplace_back(i, true);
					}
					int first = !parentHigh.front().second ? tree[parentHigh.front().first].lower : tree[parentHigh.front().first].higher;
					int second = !parentHigh.back().second ? tree[parentHigh.back().first].lower : tree[parentHigh.back().first].higher;
					if (parentHigh.front().second)
						tree[parentHigh.front().first].higher = second;
					else
						tree[parentHigh.front().first].lower = second;
					if (parentHigh.back().second)
						tree[parentHigh.back().first].higher = first;
					else
						tree[parentHigh.back().first].lower = first;
				}
			}
		}

		std::vector<int> lHist, rHist;
		CountLayers(tree, 0, leftRoot, lHist);
		CountLayers(tree, 0, rightRoot, rHist);

		std::cout << "Part 3: ";
		PrintLayer(tree, std::distance(lHist.cbegin(), std::max_element(ALLc(lHist))), 0, leftRoot);
		PrintLayer(tree, std::distance(rHist.cbegin(), std::max_element(ALLc(rHist))), 0, rightRoot);
		std::cout << '\n';
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}