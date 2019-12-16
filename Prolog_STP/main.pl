getNodes([Nodes|_], Nodes).
%returns the nodes list from the network


getEdges([_|[Edges]], Edges).
%returns the edges list from the network


getRootAux([[H|[T]]|L], [_|[Priority]], Res) :- T < Priority, getRootAux(L, [H|[T]], Res), !.
getRootAux([_|L], [Node|[Priority]], Res) :- getRootAux(L, [Node|[Priority]], Res), !.
getRootAux([], [Node|[Priority]], [Node|[Priority]]).

getRoot(Retea, R) :- getNodes(Retea, [N|Nodes]), getRootAux(Nodes, N, [R, _]).
%returns the node with the lowest priority


getPriorityAux(Node, [_|L], P) :- getPriorityAux(Node, L, P), !.
getPriorityAux(Node, [[Node, P]|_], P) :- !.

getPriority(Retea, N, P) :- getNodes(Retea, Nodes), getPriorityAux(N, Nodes, P).
%returns the priority of a node


nodeIsVisited(Node, [[Node, _]|_]) :- !.
nodeIsVisited(Node, [_|Visited]) :- nodeIsVisited(Node, Visited).
%checks if a node was visited already


getCloseEdges([Node, Dist], [[Node, Node2, Cost]|Edges], Visited, Acc, Res) :-  not(nodeIsVisited(Node2, Visited)), 
	C is Cost + Dist, getCloseEdges([Node, Dist], Edges, Visited, [[Node2, Node, C]|Acc], Res), !.
getCloseEdges([Node, Dist], [[Node1, Node, Cost]|Edges], Visited, Acc, Res) :-  not(nodeIsVisited(Node1, Visited)), 
	C is Cost + Dist, getCloseEdges([Node, Dist], Edges, Visited, [[Node1, Node, C]|Acc], Res), !.
getCloseEdges([Node, Dist], [_|Edges], Visited, Acc, Res) :- getCloseEdges([Node, Dist], Edges, Visited, Acc, Res), !.
getCloseEdges(_, [], _, Acc, Acc).
%returns a list of all edges with unvisited destination nodes and an updated cost


getAdjacentEdgesAux(Retea, [N|Nodes], Visited, Acc, Res) :- getEdges(Retea, Edges), getCloseEdges(N, Edges, Visited, [], R), 
	append(R, Acc, L), getAdjacentEdgesAux(Retea, Nodes, Visited, L, Res).
getAdjacentEdgesAux(_, [], _, Res, Res).

getAdjacentEdges(Retea, Visited, Res) :- getAdjacentEdgesAux(Retea, Visited, Visited, [], Res).
%returns a list of all edges coming from any visited node with an unvisited destination


findShortestDistAux(Retea, [[Node1, Node2, Cost]|Edges], [_, _, C], Res) :- C > Cost, findShortestDistAux(Retea, Edges, [Node1, Node2, Cost], Res), !.
findShortestDistAux(Retea, [[N1, M1, Cost]|Edges], [_, M2, C], Res) :- C == Cost, getPriority(Retea, M1, P1), 
	getPriority(Retea, M2, P2), P1 < P2, findShortestDistAux(Retea, Edges, [N1, M1, Cost], Res), !.
findShortestDistAux(Retea, [[_, M1, Cost]|Edges], [N2, M2, C], Res) :- C == Cost, getPriority(Retea, M1, P1), 
	getPriority(Retea, M2, P2), P1 > P2, findShortestDistAux(Retea, Edges, [N2, M2, Cost], Res), !.
findShortestDistAux(Retea, [_|Edges], Min, Res) :- findShortestDistAux(Retea, Edges, Min, Res), !.
findShortestDistAux(_, [], Min, Min).

findShortestDist(Retea, [E|Edges], Res) :- findShortestDistAux(Retea, Edges, E, Res).
%returns the edge having the shortest distance from root to node


findSpanningTreeAux(Retea, Visited, Acc, Res) :- getAdjacentEdges(Retea, Visited, Edges), Edges \= [], 
	findShortestDist(Retea, Edges, [N, M, C]), findSpanningTreeAux(Retea, [[N, C]|Visited], [[M, N]|Acc], Res), !.
findSpanningTreeAux(_, _, Res, Res).

member([N1, N2], [[N1, N2]|_]).
member([N1, N2], [[N2, N1]|Tree]) :- member([N1, N2], [[N1, N2]|Tree]).
member([N1, N2], [_|Tree]) :- member([N1, N2], Tree).
member(_, []).
%checks if an edge is part of the tree


nodeInPath(Node, [Node|_]).
nodeInPath(Node, [_|Path]) :- nodeInPath(Node, Path).
%check if a node is already part of the path


stp(Retea, Root, Res) :- getRoot(Retea, Root), findSpanningTreeAux(Retea, [[Root, 0]], [], Res).


findPath(Dst, Dst, _, Path, Path) :- !.
findPath(Src, Dst, Edges, Acc, Path) :- member([Src, X], Edges), not(nodeInPath(X, Acc)), findPath(X, Dst, Edges, [X|Acc], Path), !.
%finds the path from source to destination


drum(Retea, Src, Dst, Root, Edges, Path) :- stp(Retea, Root, Edges), findPath(Dst, Src, Edges, [Dst], Path), !.
