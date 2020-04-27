/** Copyright (C) 2018 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "connected-components.h"

#include <cmath>
#include <iostream>

using std::max;
using std::min;

namespace egm {
namespace algorithm {

constexpr bool kIncrediblyVerboseDebugOutput = false;
constexpr bool kMinorlyVerboseDebuggingOutput = true;

std::ostream &operator<<(std::ostream &s, const Coords &c) {
  if (c.z) return s << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return s << "(" << c.x << ", " << c.y << ")";
}

Line::Line(Coords slope_in, std::set<Coords> points_in): slope(slope_in) {
  if (points_in.empty()) return;
  points.reserve(points_in.size());

  auto it = points_in.begin();
  const Coords *lastp = &*it;
  points.push_back(*lastp);
  while (++it != points_in.end()) {
    if (*it - *lastp != slope) {
      std::cerr << "Connected components is broken or unsorted!" << std::endl
                << *it << " - "  << *lastp << " ≠ " << slope << std::endl
                << "Complete list of points in this segment:" << std::endl;
      for (auto i2 = points_in.begin(); i2 != points_in.end(); ++i2) {
        std::cerr << " - " << i2->x << ", " << i2->y << std::endl;
      }
      break;
    }
    points.push_back(*it);
    lastp = &*it;
  }
}

void ConnectedComponents::Connect(Coords a, Coords b) {
  auto ai = connections.find(a);
  auto bi = connections.find(b);
  if (ai == connections.end()) {
    if (bi == connections.end()) {
      const size_t nid = max_cid++;
      connections[a] = connections[b] = nid;
      components[nid].insert(a);
      components[nid].insert(b);
    } else {
      size_t component = bi->second;
      connections[a] = component;
      components[component].insert(a);
    }
  } else {
    if (bi == connections.end()) {
      size_t component = ai->second;
      connections[b] = component;
      components[component].insert(b);
    } else {
      size_t aid = ai->second, bid = bi->second;
      if (aid == bid) return;
      auto &ac = components[aid], &bc = components[bid];
      if (aid < bid) {
        for (const Coords &c : bc) {
          connections[c] = aid;
          ac.insert(c);
        }
        bc.clear();
        components.erase(bid);
      } else {
        for (const Coords &c : ac) {
          connections[c] = bid;
          bc.insert(c);
        }
        ac.clear();
        components.erase(aid);
      }
    }
  }
}

long ConnectedComponents::Score() const {
  long score = 0;
  for (const auto &p : components) score += p.second.size() - 3;
  return score;
}

long ConnectedComponents::ScoreB() const {
  long score = 0;
  std::map<long, long> sizes;
  for (const auto &p : components) ++sizes[p.second.size()];
  long consider = 5;
  for (auto rit = sizes.rbegin(); rit != sizes.rend(); ++rit) {
    if (rit->first < consider) {
      consider -= rit->first;
      score += rit->second * rit->first;
    } else {
      score += rit->second * consider;
      break;
    }
  }
  return score;
}

static inline double nz(double x) { return x ? x : 1; }
std::vector<Line> ConnectTheDots(const std::multiset<Coords> &cloud) {
  if (cloud.size() < 3) return {};

  double xmin, xmax, ymin, ymax, zmin, zmax;
  xmin = xmax = cloud.begin()->x;
  ymin = ymax = cloud.begin()->y;
  zmin = zmax = cloud.begin()->z;
  for (const Coords &c : cloud) {
    if (c.x < xmin) xmin = c.x;
    if (c.y < ymin) ymin = c.y;
    if (c.z < zmin) zmin = c.z;
    if (c.x > xmax) xmax = c.x;
    if (c.y > ymax) ymax = c.y;
    if (c.z > zmax) zmax = c.z;
  }

  const double xspan = xmax - xmin, yspan = ymax - ymin, zspan = zmax - zmin;
  const double scale = std::max(std::max(xspan, yspan), zspan);

  if (scale <= .01) return {};

  const int hf = std::max(4, int(cloud.size() / 75));
  const int cx = std::max(1, int(ceil(hf * xspan / scale)));
  const int cy = std::max(1, int(ceil(hf * yspan / scale)));
  const int cz = std::max(1, int(ceil(hf * zspan / scale)));
  std::vector<std::vector<std::vector<std::vector<Coords>>>> cells;
  cells.resize(cx);
  for (int i = 0; i < cx; ++i) {
    cells[i].resize(cy);
    for (int j = 0; j < cy; ++j) {
      cells[i][j].resize(cz);
    }
  }

  const double
      xgrid = nz(xspan / cx),  // These zeros don't matter, since the deltas
      ygrid = nz(yspan / cy),  // will always be 0 if the denominator would have
      zgrid = nz(zspan / cz);  // been zero. We just want 0/0 to be 0, here.

  for (const Coords &c : cloud) {
    int cellx = std::min(int((c.x - xmin) / xgrid), cx - 1);
    int celly = std::min(int((c.y - ymin) / ygrid), cy - 1);
    int cellz = std::min(int((c.z - zmin) / zgrid), cz - 1);
    if (kIncrediblyVerboseDebugOutput) {
      std::cout << c << " in space"
                << " [ " << Coords{xmin, ymin, zmin} << " .. "
                         << Coords{xmax, ymax, zmax} << " ] "
                << "to cell " << Coords(cellx, celly, cellz)
                << " out of " << Coords(cx, cy, cz) << std::endl;
    }
    cells[cellx][celly][cellz].push_back(c);
  }

  // Don't spend too much time on this.
  for (int i = 0; i < cx; ++i) {
    for (int j = 0; j < cy; ++j) {
      for (int k = 0; k < cz; ++k) {
        if (cells[i][j][k].size() > 100) return {};
        if (kIncrediblyVerboseDebugOutput) {
          std::cout << "Cell" << Coords(i,j,k) << ": [";
          for (const Coords &c : cells[i][j][k]) std::cout << c << ", ";
          std::cout << "]" << std::endl;
        }
      }
    }
  }

  // We maintain a different set of connected components for each distance
  // connecting them. This way, all connected components are colinear.
  std::map<Coords, ConnectedComponents> edges;
  for (int i = 0; i < cx; ++i)
  for (int j = 0; j < cy; ++j)
  for (int k = 0; k < cz; ++k) {
    for (size_t l = 0; l < cells[i][j][k].size(); ++l) {
      for (int m = i; m < i + 2 && m < cx; ++m)
      for (int n = j; n < j + 2 && n < cy; ++n)
      for (int o = k; o < k + 2 && o < cz; ++o) {
        for (size_t p = 0; p < cells[m][n][o].size(); ++p)
        if (cells[i][j][k][l] != cells[m][n][o][p]) {
          Coords dist = cells[i][j][k][l].PositiveDelta(cells[m][n][o][p]);
          edges[dist].Connect(cells[i][j][k][l], cells[m][n][o][p]);
          if (kIncrediblyVerboseDebugOutput) {
            std::cout << "edges[" << dist << "].Connect(" << cells[i][j][k][l]
                      << ", " << cells[m][n][o][p] << ")" << std::endl;
          }
        }
      }
    }
  }

  long high_score = 0;
  typedef std::pair<const Coords, ConnectedComponents> Grouping;
  Grouping *best_grouping = nullptr;
  for (Grouping &dist : edges) {
    long score = dist.second.Score();
    if (kIncrediblyVerboseDebugOutput) {
      std::string sep;
      std::cout << "Group " << dist.first << " has "
                << dist.second.components.size() << " components [";
      for (const auto &c : dist.second.components) {
        std::cout << sep << c.second.size(); sep = ", ";
      }
      std::cout << "] and scored " << score << " / " << high_score << std::endl;
    }
    if (score > high_score) {
      high_score = score;
      best_grouping = &dist;
    }
  }

  if (!best_grouping) return {};

  if (kMinorlyVerboseDebuggingOutput) {
    std::cout << "INFO Best grouping chosen: stride = " << best_grouping->first
              << ", score = " << high_score << std::endl;
  }

  std::vector<Line> res;
  res.reserve(best_grouping->second.components.size());
  for (const auto &id_group : best_grouping->second.components) {
    if (id_group.second.size() > 2) {
      res.push_back(Line(best_grouping->first, id_group.second));
    }
  }
  return res;
}

}  // namespace algorithm
}  // namespace egm
