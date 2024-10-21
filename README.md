# romea_core_common #

This project is a C++ library that provides versatile collection of tools designed to support the ROMEA robotic ecosystem. It offers a range of utilities for various functionalities:

- **Geodesy Conversions:**
  Tools for accurate transformations between different geodetic coordinate systems, ensuring precise location data.
- **Diagnostics, Monitoring, and Logging:**
  Features that facilitate system health monitoring, event logging, and issue diagnosis, enhancing reliability and performance.
- **Mathematics, Geometry, and Optimization:**
  A comprehensive suite of mathematical and geometric functions, including optimization techniques such as least squares, Newton's method, and Iterative Closest Point (ICP) algorithms.
- **Point Cloud Processing:**
  Utilities for managing and processing point clouds, including structures for KD-trees, grids, and bounding boxes, to facilitate spatial data analysis.
- **Time Management:**
  Tools designed to handle time-related tasks, improving synchronization and scheduling within robotic applications.

## **Usage**

1. create a ROS workspace
2. cd worskpace
3. mkdir src
4. wget https://raw.githubusercontent.com/Romea/romea-core-common/refs/heads/main/romea_common_public.repos
5. vcs import src < romea_common_public.repos
6. build packages
   - catkin build for ROS1
   - colcon build for ROS2
7. create your application using this library

## **Contributing**

If you'd like to contribute to this project, here are some guidelines:

1. Fork the repository.
2. Create a new branch for your changes.
3. Make your changes.
4. Write tests to cover your changes.
5. Run the tests to ensure they pass.
6. Commit your changes.
7. Push your changes to your forked repository.
8. Submit a pull request.

## **License**

This project is released under the Apache License 2.0. See the LICENSE file for details.

## **Authors**

The romea_core_common library was developed by **Jean Laneurit** in the context of various research projects carried out at INRAE.

## **Contact**

If you have any questions or comments about romea_core_common library, please contact **[Jean Laneurit](mailto:jean.laneurit@inrae.fr)** 