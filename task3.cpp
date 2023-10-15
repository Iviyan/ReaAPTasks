#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "utils.h"
#include <windows.h>

#define PRINTALL
//#define TESTDATA

using std::cout, std::cin, std::endl, std::vector;

struct point {
    char name;
    int index;
    int x;
    int y;

    point() = default;
    point(char name, int x, int y) : name{name}, x{x}, y{y}, index{0} {}
};

std::ostream &operator<<(std::ostream &os, const point &p) {
    os << p.name << "(" << p.x << "," << p.y << ")";
    return os;
}

struct route {
    vector<point> path;
    double length = 0;
};

std::ostream &operator<<(std::ostream &os, const route &r) {
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

void hamiltonianPath(
        vector<point> const &points, double const *const *m, size_t size,
        vector<point> &path, route &shortestRoute);

void hamiltonianCycle(
        vector<point> const &points, double const *const *m, size_t size,
        vector<point> &path, route &shortestRoute);

route hamiltonianPath(vector<point> const &points, double const *const *m, size_t size) {
    vector<point> path;
    route route = {vector<point>(), 0xffffffff};
    hamiltonianPath(points, m, size, path, route);
    return route;
}

route hamiltonianCycle(vector<point> const &points, double const *const *m, size_t size) {
    vector<point> path;
    route route = {vector<point>(), 0xffffffff};
    hamiltonianCycle(points, m, size, path, route);
    return route;
}

int main_() {
    SetConsoleOutputCP(65001);

    vector<point> points;

    cout << "Введите список точек." << endl;

#ifndef TESTDATA
    for (;;) {
        cout << "Введите имя точки из 1 символа (0 - завершить ввод), и её координаты x, y: ";
        point p = point();
        cin >> p.name;
        if (p.name == '0') break;
        if (findIndex(points, [&](auto &item) { return item.name == p.name; }) != -1) {
            cout << "Данное имя точки уже использовано!" << endl;
            continue;
        }
        cin >> p.x >> p.y;
        points.push_back(p);
    }
#else
    points.emplace_back('a', 1, 2);
    points.emplace_back('b', 4, 3);
    points.emplace_back('c', 2, 3);
    points.emplace_back('d', 2, 5);
    /*points.emplace_back('e', 3, 2);
    points.emplace_back('f', 1, 4);
    points.emplace_back('j', 1, 4);
    points.emplace_back('k', 1, 4);
    points.emplace_back('l', 1, 4);
    points.emplace_back('n', 1, 4);*/
#endif

    for (int i = 0; i < points.size(); i++)
        points[i].index = i;

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

    {
        route route = hamiltonianPath(points, m, size);

#ifdef PRINTALL
        cout << endl;
#endif

        cout << "Кратчайший гамильтонов путь: " << endl << route << endl << endl;
    }

    {
        route route = hamiltonianCycle(points, m, size);


#ifdef PRINTALL
        cout << endl;
#endif

        cout << "Кратчайший гамильтонов цикл: " << endl << route << endl;
    }

    for (int i = 0; i < size; i++)
        delete[] m[i];
    delete[] m;

    return 0;
}

void hamiltonianPath(
        vector<point> const &points, double const *const *m, size_t size,
        vector<point> &path, route &shortestRoute
) {
    if (path.size() < size) {
        for (int i = 0; i < size; i++) {
            point p = points[i];

            if (findIndex(path, [&](auto &item) { return item.name == p.name; }) != -1)
                continue;

            path.push_back(p);
            hamiltonianPath(points, m, size, path, shortestRoute);
            path.pop_back();
        }
    } else {
        double pathLength = 0;
        for (int i = 0; i < size - 1; i++) {
            point &p1 = path[i];
            point &p2 = path[i + 1];

            pathLength += m[p1.index][p2.index];
        }
        pathLength = round(pathLength, 2);

#ifdef PRINTALL
        cout << route{path, pathLength} << endl;
#endif
        if (pathLength < shortestRoute.length) {
            shortestRoute = {path, pathLength};
        }
    }
}

void hamiltonianCycle(
        vector<point> const &points, double const *const *m, size_t size,
        vector<point> &path, route &shortestRoute
) {
    if (path.size() < size) {
        for (int i = 0; i < size; i++) {
            point p = points[i];

            if (findIndex(path, [&](auto &item) { return item.name == p.name; }) != -1)
                continue;

            path.push_back(p);
            hamiltonianCycle(points, m, size, path, shortestRoute);
            path.pop_back();
        }
    } else {
        double pathLength = 0;
        for (int i = 0; i < size; i++) {
            point &p1 = path[i];
            point &p2 = i == size - 1 ? path[0] : path[i + 1];

            pathLength += m[p1.index][p2.index];
        }
        pathLength = round(pathLength, 2);

#ifdef PRINTALL
        {
            vector<point> fullPath = path;
            fullPath.push_back(path[0]);
            cout << route{fullPath, pathLength} << endl;
        }
#endif
        if (pathLength < shortestRoute.length) {
            vector<point> fullPath = path;
            fullPath.push_back(path[0]);
            shortestRoute = {fullPath, pathLength};
        }
    }
}