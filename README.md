Spatial partitioning using a k-d tree

As the number of entities in the world increases, it becomes increasingly time consuming to iterate over them to test whether various conditions are met (collisions etc.)

Optimizations are required in order to keep all the checks within the time alloted to a single frame (or at least close enough).

One of the ways to go about it is to create a virtual partitioning system that tracks and groups all the objects based on their position in the world in order to avoid making computations that don't make sense.

A k-dimensional tree (in this case 2D) is one of the structures that can be used for creating such a partition. As can be seen in the example, it greatly reduces the time it takes to iterate through the points, at the cost of a longer initilization duration when creating the structure.

I tried it out by generating 10.000 points in a random donut pattern and then implementing algorithms to locate points that are in a desired range around a certain point, and the nearest point in the system to the current cursor position.
