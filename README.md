<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
<p>I have taken input for 100 nodes from a csv file and given image of the  graph for your reference</p>

<p>Just clone the repository and run Drone.cpp file</p>

<p>Algorithm :<br>
    &nbsp;&nbsp;&nbsp;&nbsp;1.	File Reading and Graph Construction:
Read a CSV file containing edge information.
Construct an adjacency list &nbsp;&nbsp;&nbsp;&nbsp;representation of the graph.
<br>
&nbsp;&nbsp;&nbsp;&nbsp;2.	Dijkstra's Algorithm:
function Dijkstra(Graph, src):
    dist[] := array of distances initialized to infinity, size V (number &nbsp;&nbsp;&nbsp;&nbsp;of vertices)
    parent[] := array of parents initialized to -1, size V
    dist[src] := 0

    pq := empty priority queue (min-heap) prioritized by dist[] values
    PriorityQueue.push((src, 0))

    while PriorityQueue is not empty:
        u := PriorityQueue.pop().node

        for each neighbor v of u in Graph:
            new_distance := dist[u] + weight(u, v)
            if new_distance < dist[v]:
                dist[v] := new_distance
                parent[v] := u
                PriorityQueue.push((v, dist[v]))

    return dist[], parent[]
3.	Drone Operations:
o	Execute operations (Takeoff, Survey, ReturnToHome, Land, Failure) using threads.
o	Ensure synchronization such that operations complete in the expected order.
</p>


<p>Time Complexity Analysis:<br><br>
File Reading and Graph Construction (MissionPlanning constructor):<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; File Reading: Reading each line from the file involves a constant amount of work per line, so if there are E edges, &nbsp;&nbsp;&nbsp;&nbsp;this part would take O(E) time.<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Graph Construction: After reading the file, constructing the adjacency matrix involves iterating over the edges &nbsp;&nbsp;&nbsp;&nbsp;again, which also takes O(E) time.
Therefore, the overall time complexity for file reading and graph construction is O(E).<br><br>
1.	Dijkstra's Algorithm (MissionPlanning::dijkstra)<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Priority Queue Operations: Each node can be pushed and popped from the priority queue at most once. &nbsp;&nbsp;&nbsp;&nbsp;Since the priority queue operations (push, pop, top) take O(log V) time and we might perform up to E operations (in the worst case where all edges are processed), the total complexity for priority queue operations is O(E log V).<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Edge Relaxations: Each edge is processed exactly once in the worst case, contributing O(E) time.<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Overall: The time complexity of Dijkstra's algorithm is dominated by the priority queue operations in sparse &nbsp;&nbsp;&nbsp;&nbsp;graphs, hence it is O(E log V).<br><br>
2.	Execution of Drone Operations (executeOperation function):<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Each operation (takeoff, survey, returnHome, land) executes a single cout operation, which is constant time, O(1).<br><br>
3.	Threads and Join Operations:<br>
&nbsp;&nbsp;&nbsp;&nbsp; Creating and managing threads (std::thread) involves a constant amount of work per thread creation, but the join &nbsp;&nbsp;&nbsp;&nbsp;operation waits until the thread completes, which depends on the operations running inside the thread.<br><br>
&nbsp;&nbsp;&nbsp;&nbsp; Joining Threads: The join operations on threads in the main function are sequential and wait for each thread to &nbsp;&nbsp;&nbsp;&nbsp;complete before moving to the next, but they do not add to the time complexity of the core operations significantly.<br><br>

Overall Time Complexity: The main computational tasks (file reading & graph construction and Dijkstra's algorithm) are efficiently handled with time complexities of O(E) and O(E log V) respectively, where E is the number of edges and V is the number of vertices.
</p>
<img src="graph.png" alt="">
</body>
</html>
