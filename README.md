# Spatial partitioning using a k-d tree

![range detector in 2D space](https://github.com/RaduHaulica/KDTree-visualization/blob/974a268220da8d45c825354cefd1181541b1ca21/KDTree/media/kdtreerange%20finder.gif)

As the number of entities in the world increases, it becomes increasingly time consuming to iterate over them to test whether various conditions are met (collisions etc.)

Optimizations are required in order to keep all the checks within the time alloted to a single frame (or at least close enough).

One of the ways to go about it is to create a virtual partitioning system that tracks and groups all the objects based on their position in the world in order to avoid making computations that don't make sense.

A k-dimensional tree (in this case 2D) is one of the structures that can be used for creating such a partition. As can be seen in the examples, it greatly reduces the time it takes to iterate through the points, at the cost of a longer initilization duration when creating the structure.

I tried it out by generating 10.000 points in a random donut pattern and then implementing algorithms to locate points that are in a desired range around a certain point (above), and the nearest point in the system to the current cursor position (below).

![find closest point in 2D space](https://github.com/RaduHaulica/KDTree-visualization/blob/46aaef1ee71e70762d7c0967fbdbbc1a79713e89/KDTree/media/kdtree%20closest%20point%20better.gif)

Only the light blue points are used in the computation to find the closest point to the cursor, greatly reducing the number of items that need to be processed to find the desired result.

When the tree is created it sorts all the elements by their k-axis coordinate at each level, picking the median as the root and repeating the process for the remaining elements on each side of the root.

![k-d tree spatial partitioning](https://github.com/RaduHaulica/KDTree-visualization/blob/46aaef1ee71e70762d7c0967fbdbbc1a79713e89/KDTree/media/kdtree%20spatial%20partinioning%20helpers.gif)

![k-d tree spatial partitioning tree visualization](https://github.com/RaduHaulica/KDTree-visualization/blob/46aaef1ee71e70762d7c0967fbdbbc1a79713e89/KDTree/media/kdtree%20spatial%20partinioning%20tree.gif)