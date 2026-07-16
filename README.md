# romea_core_common

`romea_core_common` is a modular C++ library providing the low-level building blocks used across the ROMEA robotics ecosystem.

The package gathers reusable algorithms, mathematical utilities, geometric models, coordinate transformations, monitoring tools, and spatial data structures commonly required in robotics software development.

It is designed for:

- robotics research and experimentation,
- real-time robotic applications,
- ROS1 and ROS2 integration,
- reusable software architectures.

---

# Features

## Geodesy & Coordinate Systems

Utilities dedicated to geographic localization and coordinate manipulation:

- WGS84 / UTM / ENU conversions
- Geodetic frame transformations
- Earth projection utilities
- Local tangent plane computations
- Pose and frame conversions

Modules:
- `geodesy`
- `coordinates`
- `transform`

---

## Geometry & Mathematics

Core mathematical and geometric algorithms for robotics applications:

- 2D / 3D geometric primitives
- Spatial transformations
- Bounding boxes
- Linear algebra helpers
- Numerical optimization
- Newton-based solvers
- Signal filtering and interpolation

Modules:
- `geometry`
- `math`
- `signal`

---

## Regression & Estimation

Robust estimation and optimization methods:

- Least squares estimation
- RANSAC-based fitting
- Iterative Closest Point (ICP)
- Transformation estimation

Modules:
- `regression`
- `transform/estimation`

---

## Point Set & Spatial Processing

Efficient data structures and algorithms for spatial analysis:

- KD-tree implementation
- Grid containers
- Point set utilities
- Spatial indexing
- Nearest-neighbor search

Modules:
- `pointset`
- `containers/grid`
- `containers/boundingbox`

---

## Control & Robotics Utilities

Reusable robotics-oriented helper functions:

- Motion control utilities
- Frame manipulation
- State handling
- Synchronization helpers

Modules:
- `control`
- `time`

---

## Diagnostics, Monitoring & Logging

Infrastructure for robust robotic applications:

- Runtime diagnostics
- Health monitoring
- Logging utilities
- Performance monitoring
- Debug support

Modules:
- `diagnostic`
- `monitoring`
- `log`

---

## Generic Utilities

Additional reusable components:

- Thread-safe utilities
- Concurrency helpers
- Generic containers
- Lexical conversions

Modules:
- `concurrency`
- `containers`
- `lexical`

---

# Dependencies

The library relies on standard modern C++ tooling and integrates naturally within ROS ecosystems.

Main dependencies typically include:

- C++17
- Eigen
- ROS1 or ROS2 build tools
- CMake

Additional dependencies may be required depending on enabled modules.


---

# **Usage**

1. create a ROS workspace
2. cd worskpace
3. mkdir src
4. wget https://raw.githubusercontent.com/Romea/romea-core-common/refs/heads/main/romea_common_public.repos
5. vcs import src < romea_common_public.repos
6. build packages
   - catkin build for ROS1
   - colcon build for ROS2
7. create your application using this library

# **Contributing**

If you'd like to contribute to this project, here are some guidelines:

1. Fork the repository.
2. Create a new branch for your changes.
3. Make your changes.
4. Write tests to cover your changes.
5. Run the tests to ensure they pass.
6. Commit your changes.
7. Push your changes to your forked repository.
8. Submit a pull request.

# **License**

This project is released under the Apache License 2.0. See the LICENSE file for details.

# **Authors**

The romea_core_common library was developed by **Jean Laneurit** in the context of various research projects carried out at INRAE.

# **Contact**

If you have any questions or comments about romea_core_common library, please contact **[Jean Laneurit](mailto:jean.laneurit@inrae.fr)** 