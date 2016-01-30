#include "voxel_grid.h"

#include <cmath>
#include <pcl/common/common.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sstream>
#include <vector>

using pcl::PointCloud;
using pcl::PointXYZ;


namespace iv_descriptor {

// Adds lines for the given primary axis d0. d1 and d2 should be indices of the
// other two axes. For example, to draw lines in the x-axis, set d0 to 0, and d1
// and d2 to 1 and 2, respectively. To draw lines across the y-axis, set d0 to
// 1, and d1 and d2 to 0 and 2, respectively, and so on. The order of d1 and d2
// does not matter. min_d# is the minimum value in the #-axis. Similarly, num_d#
// is the number of lines to be drawn in the #-axis.
void DrawLinesForAxis(pcl::visualization::PCLVisualizer *viewer,
    const float cell_size, const int d0, const int d1, const int d2,
    const float min_d0, const float min_d1, const float min_d2,
    const int num_d0, const int num_d1, const int num_d2) {
  std::vector<float> line_start = {0, 0, 0};
  std::vector<float> line_end = {0, 0, 0};
  line_start[d0] = min_d0;
  line_end[d0] = min_d0 + (num_d0 - 1) * cell_size;
  std::ostringstream id_sstream;
  for (int i = 0; i < num_d1; ++i) {
    const float pos_i = min_d1 + i * cell_size;
    line_start[d1] = pos_i;
    line_end[d1] = pos_i;
    for (int j = 0; j < num_d2; ++j) {
      const float pos_j = min_d2 + j * cell_size;
      line_start[d2] = pos_j;
      line_end[d2] = pos_j;
      PointXYZ p0(line_start[0], line_start[1], line_start[2]);
      PointXYZ p1(line_end[0], line_end[1], line_end[2]);
      id_sstream.str("");
      id_sstream << "line_" << d0 << "_" << i << "_" << j;
      viewer->addLine<PointXYZ>(p0, p1, id_sstream.str());
    }
  }
}

VoxelGrid::VoxelGrid(
    const float cell_size, const PointCloud<PointXYZ> &cloud)
    : cell_size_(cell_size) {
  // Set up the voxel grid dimensions.
  PointXYZ min_bounds;
  PointXYZ max_bounds;
  pcl::getMinMax3D<PointXYZ>(cloud, min_bounds, max_bounds);
  const float length_x = max_bounds.x - min_bounds.x;
  const float length_y = max_bounds.y - min_bounds.y;
  const float length_z = max_bounds.z - min_bounds.z;
  num_cells_x_ = int(ceil(length_x / cell_size_)) + 2;
  num_cells_y_ = int(ceil(length_y / cell_size_)) + 2;
  num_cells_z_ = int(ceil(length_z / cell_size_)) + 2;
  min_x_ = min_bounds.x - cell_size_;
  min_y_ = min_bounds.y - cell_size_;
  min_z_ = min_bounds.z - cell_size_;
  // TODO: Compute the interior voxels.
}

float VoxelGrid::ConvolveAtPoint(const VoxelGrid &filter,
    const float x, const float y, const float z) const {
  // TODO
  return 0;
}

float VoxelGrid::ConvolveAtCell(
    const VoxelGrid &filter, const int x, const int y, const int z) const {
  // TODO
  return 0;
}

void VoxelGrid::AddToViewer(pcl::visualization::PCLVisualizer *viewer) const {
  DrawLinesForAxis(viewer, cell_size_, 0, 1, 2, min_x_, min_y_, min_z_,
      num_cells_x_, num_cells_y_, num_cells_z_);
  DrawLinesForAxis(viewer, cell_size_, 1, 0, 2, min_y_, min_x_, min_z_,
      num_cells_y_, num_cells_x_, num_cells_z_);
  DrawLinesForAxis(viewer, cell_size_, 2, 0, 1, min_z_, min_x_, min_y_,
      num_cells_z_, num_cells_x_, num_cells_y_);
}

};
