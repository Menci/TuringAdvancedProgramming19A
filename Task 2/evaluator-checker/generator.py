from random import Random
from expression import Expression

class ExpressionGenerator:
    def __init__(self, seed, length):
        self.rng = Random(seed)
        self.length = length

    def random_number(self):
        exp = self.rng.random() * 40 - 20
        x = self.rng.random()
        return x * (10 ** exp)

    def random_operator(self):
        return self.rng.choice("+-*/")
    
    def generate(self):
        tree = [[-1, -1]]
        free_edges = [(0, 0), (0, 1)]

        while len(tree) < self.length:
            e = self.rng.choice(free_edges)
            node, child = e

            k = len(tree)
            tree.append([-1, -1])

            tree[node][child] = k
            free_edges.extend([(k, 0), (k, 1)])
            free_edges.remove(e)

        def dfs(i):
            if i == -1:
                return Expression(type="number", value=self.random_number(), rng=self.rng)
            l = dfs(tree[i][0])
            r = dfs(tree[i][1])
            return Expression(type="complex", left_operand=l, operator=self.random_operator(), right_operand=r, rng=self.rng)

        return dfs(0)
