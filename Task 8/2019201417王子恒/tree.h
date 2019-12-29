#ifndef _MENCI_TREE_H_
#define _MENCI_TREE_H_

#include <vector>
#include <random>
#include <tuple>
#include <functional>
#include <cassert>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

struct Node {
	int child[2] = {-1, -1};
	char data[K];
};

std::vector<Node> nodes(N);

template <typename T, typename Compare = std::less<T>>
using set = __gnu_pbds::tree<
	T, // Key
	__gnu_pbds::null_type, // Mapped
	Compare, // Cmp_Fn
	__gnu_pbds::rb_tree_tag, // Tag
	__gnu_pbds::tree_order_statistics_node_update // Node_Update
>;

void build(unsigned seed = -1u) {
	if (seed == -1u) seed = std::random_device()();
	std::mt19937 rng{seed};

	set<std::tuple<unsigned, size_t, size_t>> freeEdges;
	freeEdges.insert(std::make_tuple(rng(), 0, 0));
	freeEdges.insert(std::make_tuple(rng(), 0, 1));

	for (size_t i = 1; i < N; i++) {
		std::uniform_int_distribution<> dis(0, freeEdges.size() - 1);
		size_t j = dis(rng);

		auto it = freeEdges.find_by_order(j);
		auto [r, parent, which] = *it;

		nodes[parent].child[which] = i;

		freeEdges.insert(std::make_tuple(rng(), i, 0));
		freeEdges.insert(std::make_tuple(rng(), i, 1));

		freeEdges.erase(it);
	}
}

#ifdef VALIDATE

#include <iostream>

void validate() {
	size_t count = 0;
	std::function<void (int)> dfs = [&](int i) {
		if (i == -1 || i >= N) return;
		dfs(nodes[i].child[0]);
		dfs(nodes[i].child[1]);
		count++;
	};

	dfs(0);
	assert(count == N);
}

#else

#define validate()

#endif

#endif // _MENCI_TREE_H_
