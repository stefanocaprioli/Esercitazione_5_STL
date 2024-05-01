#include "PolygonalMesh.hpp"
#include <iostream>

using namespace std;

namespace MeshLibrary {

bool importmesh(const string& path, PolygonalMesh& mesh); //funzione che controlla l'apertura dei file
bool importcell0d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool importcell1d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool importcell2d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool checkedges(const PolygonalMesh& mesh); //controlla che i segmenti siano >0
bool checkMarkers0d(const PolygonalMesh& mesh); //controlla i marker 0d
bool checkMarkers1d(PolygonalMesh& mesh); //controlla i marker 1d
}

namespace calc {
double edgelength(const double& x1, const double& y1,const double& x2, const double& y2);
}
#include "PolygonalMesh.hpp"
#include <iostream>

using namespace std;

namespace MeshLibrary {

bool importmesh(const string& path, PolygonalMesh& mesh); //funzione che controlla l'apertura dei file
bool importcell0d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool importcell1d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool importcell2d(const string& path, PolygonalMesh& mesh); //importa le info sulle celle 0d
bool checkedges(const PolygonalMesh& mesh); //controlla che i segmenti siano >0
bool checkMarkers0d(const PolygonalMesh& mesh); //controlla i marker 0d
bool checkMarkers1d(PolygonalMesh& mesh); //controlla i marker 1d
bool checkareas(PolygonalMesh& mesh); //controlla le aree
}

namespace calc {
double edgelength(const double& x1, const double& y1,const double& x2, const double& y2); //calcola la lunghezza di un segmento
}
