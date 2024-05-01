#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace MeshLibrary;
using namespace calc;

int main()
{
    PolygonalMesh mesh;
    string path = "PolygonalMesh";

    if (!importmesh(path, mesh))
    {
        return 1;
    }

    checkedges(mesh);
    checkMarkers0d(mesh);
    checkMarkers1d(mesh);
    checkareas(mesh);
    return 0;
}
