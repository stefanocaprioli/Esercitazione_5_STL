# pragma once

#include <iostream>
#include <vector>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace MeshLibrary {
struct PolygonalMesh
{
    unsigned int NumberCell0d = 0; //numero di celle 0d
    vector<unsigned int> IdCell0d = {}; //identificatori celle 0d
    vector<Vector2d> CoordinatesCell0d = {}; //vettori cordinate delle celle 0d
    map<unsigned int, list<unsigned int>> Cell0dMarkers = {}; //mappa dei marker 0d

    unsigned int NumberCell1d = 0; //numero celle 1d
    vector<unsigned int> IdCell1d = {}; //identificatori celle 1d
    vector<Vector2i> VerticesCell1d = {}; //vettori degli id delle celle di partenza e arrivo dei segmenti 1d
    map<unsigned int, list<unsigned int>> Cell1dMarkers = {}; //mappa dei marker 1d

    unsigned int NumberCell2d = 0; //numero celle 2d
    vector<unsigned int> IdCell2d = {}; //identificatori celle 2d
    map<unsigned int, list<unsigned int>> NumVerticesCell2d = {}; //mappa che associa i possibili numeri di vertici ai rispettivi id
    vector<vector<unsigned int>> VerticesCell2d = {}; //vettori degli id dei vertici (0d) dei poligoni 2d
    map<unsigned int, list<unsigned int>> NumEdgesCell2d = {}; //mappa che associa i possibili numeri di lati ai rispettivi id
    vector<vector<unsigned int>> EdgesCell2d = {}; //vettori degli id dei lati (1d) dei poligoni 2d
    // non memorizziamo una mappa dei marker 2d dato che sono ovviamento tutti 0 (in uno spazio 2 dimensionale i poligoni non possono essere al bordo)
};
}
