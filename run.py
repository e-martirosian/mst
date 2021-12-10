import networkx as nx
import numpy as np

graph = open("outputs/graph.txt", "r")
n, m = map(int, graph.readline().strip().split(' '))

G = nx.Graph()

arr = set()

for _ in range(m):
    a, b, w = map(int, graph.readline().strip().split(' '))
    G.add_edge(a, b, weight=w)
    arr.add(max(a, b) * 1000 + min(a, b))

T=nx.minimum_spanning_tree(G)
print(len([a['weight'] for a in np.array(sorted(T.edges(data=True)))[:,2]]))

print(len(arr))

nx.draw(G, with_labels=True, font_weight='bold')

