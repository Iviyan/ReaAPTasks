#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "utils.h"
#include <windows.h>

using std::cout, std::cin, std::endl, std::vector;

struct point {
    char name;
    int x;
    int y;

    point() = default;
    point(char name, int x, int y) : name{name}, x{x}, y{y} {}
};

std::ostream& operator<<(std::ostream& os, const point& p)
{
    os << p.name << "(" << p.x << "," << p.y << ")";
    return os;
}

struct route {
    vector<point> path;
    double length = 0;
};

std::ostream& operator<<(std::ostream& os, const route& r)
{
    if (!r.path.empty())
        os << r.path[0];
    for (int i = 1; i < r.path.size(); i++)
        os << " -> " << r.path[i];
    cout << " = " << r.length;
    return os;
}

double round(double d, int digits) {
    double v = pow(10, digits);
    return round(d * v) / v;
}

template<typename T, typename Predicate>
int findIndex(const std::vector<T> &vec, Predicate pred) {
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    if (it != vec.end()) {
        return std::distance(vec.begin(), it);
    }
    return -1;
}

template<typename T, typename Predicate>
bool find(const std::vector<T> &vec, Predicate pred, T &result) {
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    if (it == vec.end()) return false;
    result = *it;
    return true;
}

void hamiltonianPath(
        vector<point> const &points, double **m, size_t size,
        vector<point> &path, route &shortestRoute
);

int main_() {
    SetConsoleOutputCP( 65001 );

    vector<point> points;

    cout << "Введите список точек." << endl;

    /*for (;;) {
        cout << "Введите имя точки из 1 символа (0 - завершить ввод), и её координаты x, y: ";
        point p = point();
        cin >> p.name;
        if (p.name == '0') break;
        cin >> p.x >> p.y;
        points.push_back(p);
        // TODO: check repeating names
    }*/

    points.emplace_back('a', 1, 2);
    points.emplace_back('b', 4, 3);
    points.emplace_back('c', 2, 3);
    points.emplace_back('d', 2, 5);
    points.emplace_back('e', 3, 2);
    points.emplace_back('f', 1, 4);
    points.emplace_back('j', 1, 4);
    points.emplace_back('k', 1, 4);
    points.emplace_back('l', 1, 4);
    //points.emplace_back('n', 1, 4);

    size_t size = points.size();

    for (auto p: points)
        cout << p << endl;

    auto **m = new double *[size]; // m[y][x]
    for (int i = 0; i < size; i++)
        m[i] = new double[size];

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            point py = points[y];
            point px = points[x];
            double distance = sqrt(pow(px.x - py.x, 2) + pow(px.y - py.y, 2));
            m[y][x] = round(distance, 2);
        }
    }

    print_matrix(m, size, size);
    cout << endl;

    vector<point> path;
    route route = {vector<point>(), 0xffffffff};
    hamiltonianPath(points, m, size, path, route);

    cout << "Результат" << endl << route << endl;

    for (int i = 0; i < size; i++)
        delete[] m[i];
    delete[] m;

    return 0;
}

void hamiltonianPath(
        vector<point> const &points, double **m, size_t size,
        vector<point> &path, route &shortestRoute
) {
    if (path.size() < size) {
        for (int i = 0; i < size; i++) {
            point p = points[i];
            if (std::any_of(path.begin(), path.end(), [&](const auto &item) {
                return item.name == p.name;
            })) {
                continue;
            }

            path.push_back(p);
            hamiltonianPath(points, m, size, path, shortestRoute);
            path.pop_back();
        }
    } else {
        double pathLength = 0;
        for (int i = 0; i < size - 1; i++) {
            point &p1 = path[i];
            point &p2 = path[i + 1];
            int p1Index = findIndex(points, [&](auto &item) { return item.name == p1.name; });
            int p2Index = findIndex(points, [&](auto &item) { return item.name == p2.name; });

            pathLength += m[p1Index][p2Index];
        }

        //cout << route{path, pathLength} << endl;
        if (pathLength < shortestRoute.length) {
            shortestRoute = {path, pathLength};
        }
    }
}