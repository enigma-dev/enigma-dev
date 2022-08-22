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

#include <map>
#include <set>
#include <ostream>
#include <vector>

namespace egm {
namespace algorithm {

struct Coords {
  double x, y, z;
  Coords(int xx, int yy, int zz): x(xx), y(yy), z(zz) {}
  Coords(double xx, double yy, double zz): x(xx), y(yy), z(zz) {}

  // Construct from anything with x()/y()/z()
  template<typename T, typename enable = decltype(T().x() + T().y()),
                       typename prefer = decltype(T().z())>
  Coords(const T &inst): x(inst.x()), y(inst.y()), z(inst.z()) {}

  // Construct from anything with x()/y()
  template<typename T, typename enable = decltype(T().x() + T().y()),
                       typename hack = decltype(T().background_name()),
                       typename omg = int> // Work around lack of concepts; allow Tile
  Coords(const T &t): x(t.x()), y(t.y()), z(0) {}

  bool operator< (const Coords &o) const {
    return
        x < o.x || (!(o.x < x) && (
        y < o.y || (!(o.y < y) && (
        z < o.z ))));
  }
  Coords PositiveDelta(const Coords &o) const {
    if (*this < o)
      return { o.x - x, o.y - y, o.z - z };
    return { x - o.x, y - o.y, z - o.z };
  }
  Coords operator-(const Coords &o) const {
    return { x - o.x, y - o.y, z - o.z };
  }
  bool operator==(const Coords &o) const {
    return x == o.x && y == o.y && z == o.z;
  }
  bool operator!=(const Coords &o) const {
    return x != o.x || y != o.y || z != o.z;
  }
};

std::ostream &operator<<(std::ostream &s, const Coords &c);

struct Line {
  Coords slope;
  std::vector<Coords> points;
  Line(Coords slope_in, std::set<Coords> points_in);
  const Coords &first() const { return points.front(); }
  const Coords &last() const { return points.back(); }
};

struct ConnectedComponents {
  typedef std::set<Coords> Component;
  std::map<Coords, size_t> connections;
  std::map<size_t, Component> components;
  size_t max_cid = 0;

  void Connect(Coords a, Coords b);
  long Score() const;
  long ScoreB() const;
};

std::vector<Line> ConnectTheDots(const std::multiset<Coords> &cloud);

}  // namespace algorithm
}  // namespace egm
