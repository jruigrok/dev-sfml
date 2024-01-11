## About:

- Large-scale particle simulation using graph-based partitioning and multithreading for optimized collision detection.
- Physics calculated using Verlet integration for realistic simulations with high speed.
- Graphics implemented using the SFML library, with all objects in a vertex array rendered simultaneously.
- Includes simple spring constraints for simulating small rigid bodies, soft bodies, and ropes.

## Performance:

- Can handle over 100k particles at over 60fps using 4 physics substeps on most midrange laptops.
- Lowering substeps increases performance but may result in a more unstable simulation.

## Controls:

- Press 'p' to pause the simulation, use the mouse to move around, and scroll wheel to zoom in.
- Basic controls for creating particles:
  - Press '1' for place static object mode, click and hold to drop static objects.
  - Press '2' for place object mode, click and hold to drop objects.
  - Press '+' to increase the area when placing objects.
  - Press '-' to decrease the area when placing objects.
  - Press 'up arrow' to increase gravitational force.
  - Press 'down arrow' to decrease gravitational force.