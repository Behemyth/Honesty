import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

namespace
{
	auto suiteGenerator = []() -> Generator
	{
		co_yield Test(
			"outer",
			[]() -> Generator
			{
				co_return;
			});
	};

	// Global creation
	suite suite("outer", suiteGenerator);
	
	// template<class... Ts>
	// struct overload : Ts...
	//{
	//	using Ts::operator()...;
	// };
	//
	// struct Leaf
	//{
	// };
	//
	// struct Node;
	// using Tree = std::variant<Leaf, Node*>;
	//
	// struct Node
	//{
	//	Tree left;
	//	Tree right;
	// };
	//
	// int num_leaves(Tree const& tree)
	//{
	//	return std::visit(
	//		overload(
	//			//see below
	//			[](Leaf const&)
	//			{
	//				return 1;
	//			},
	//			[](this auto const& self, Node* n) -> int
	//			{
	//				return std::visit(self, n->left) + std::visit(self, n->right);
	//			}
	//			),
	//		tree);
	// }
}
