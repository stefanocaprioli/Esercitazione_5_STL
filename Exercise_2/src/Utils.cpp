#include "Utils.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <Eigen/Eigen>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace Eigen;

namespace calc {
double edgelength(const double& x1, const double& y1,const double& x2, const double& y2)
{
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
}

namespace MeshLibrary {
bool importmesh(const string& path, PolygonalMesh& mesh)
{
    bool check = true;
    if(!importcell0d(path, mesh)) //prova ad importare il file Cell0d.csv
    {
        cerr << "File Cell0Ds.csv not found" << endl;
        check = false;
    }

    if(!importcell1d(path, mesh)) //prova ad importare il file Cell1d.csv
    {
        cerr << "File Cell1Ds.csv not found" << endl;
        check = false;
    }

    if(!importcell2d(path, mesh)) //prova ad importare il file Cell2d.csv
    {
        cerr << "File Cell2Ds.csv not found" << endl;
        check = false;
    }

    return check;
}

bool importcell0d(const string& path, PolygonalMesh& mesh)
{
    ifstream file(path + "/Cell0Ds.csv");
    if (file.fail()) //controlla se si apre il file
    {
        return false;
    }

    list<string> lines; //crea una lista di righe nella quale memorizzare il file
    string line;
    while(getline(file,line)) //legge una riga del file
    {
        lines.push_back(line); //aggiunge la riga alla lista
    }

    lines.pop_front(); //elimina la prima riga di intestazione
    mesh.NumberCell0d = lines.size(); //il numero di righe fornisce il numero di celle 0d

    //riscaliamo i vettori in base alla dimensione fornita:
    mesh.CoordinatesCell0d.reserve(mesh.NumberCell0d);
    mesh.IdCell0d.reserve(mesh.NumberCell0d);

    for(const string& line :lines)
    {
        istringstream convert(line); //crea un oggetto stringstream nel quale viene memorizzata la riga

        stringstream ss;
        string token;
        string coordinate1;
        string coordinate2;
        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;

        // leggiamo ogni elemento separato da ; con getline e lo mettiamo nella variabile
        getline(convert, token, ';');
        ss << token;
        ss >> id;
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> marker;
        ss.clear();

        getline(convert, coordinate1, ';');
        getline(convert, coordinate2, ';');

        coordinates(0) = stod(coordinate1);
        coordinates(1) = stod(coordinate2);

        //aggiungiamo i nuovi elementi ai vettori:
        mesh.CoordinatesCell0d.push_back(coordinates);
        mesh.IdCell0d.push_back(id);

        //aggiorniamo la mappa dei marker
        if (mesh.Cell0dMarkers.find(marker) == mesh.Cell0dMarkers.end()) //controlla se il marker è già presente nella mappa
        {
            mesh.Cell0dMarkers.insert({marker, {id}}); //crea una coppia chiave(marker)/valore(lista di id)
        }
        else
        {
            mesh.Cell0dMarkers[marker].push_back(id); //aggiunge un nuovo valore alla lista degli id associata al marker
        }
    }

    file.close(); //chiude il file
    return true;
}

bool importcell1d(const string& path, PolygonalMesh& mesh)
{
    //apriamo e memorizziamo il file come lista di righe
    ifstream file(path + "/Cell1Ds.csv");
    if (file.fail())
    {
        return false;
    }

    list<string> lines;
    string line;
    while(getline(file,line))
    {
        lines.push_back(line);
    }

    lines.pop_front();
    mesh.NumberCell1d = lines.size();

    //riscaliamo i vettori
    mesh.IdCell1d.reserve(mesh.NumberCell1d);
    mesh.VerticesCell1d.reserve(mesh.NumberCell1d);

    for (const string& line : lines)
    {
        //leggiamo i valori dalle linee
        istringstream convert(line);

        stringstream ss;
        string token;
        unsigned int id;
        unsigned int marker;
        Vector2i extremes;

        getline(convert, token, ';');
        ss << token;
        ss >> id;
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> marker;
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> extremes(0);
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> extremes(1);
        ss.clear();

        //inseriamo i valori letti nei vettori
        mesh.IdCell1d.push_back(id);
        mesh.VerticesCell1d.push_back(extremes);

        //aggiorniamo la mappa dei marker
        if (mesh.Cell1dMarkers.find(marker) == mesh.Cell1dMarkers.end())
        {
            mesh.Cell1dMarkers.insert({marker, {id}});
        }
        else
        {
            mesh.Cell1dMarkers[marker].push_back(id);
        }
    }

    file.close(); //chiude il file
    return true;
}

bool importcell2d(const string& path, PolygonalMesh& mesh)
{
    //apriamo e memorizziamo il file in una lista di righe
    ifstream file(path + "/Cell2Ds.csv");
    if (file.fail())
    {
        return false;
    }

    list<string> lines;
    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    lines.pop_front();
    mesh.NumberCell2d = lines.size();

    //riscaliamo i vettori
    mesh.IdCell2d.reserve(mesh.NumberCell2d);
    mesh.VerticesCell2d.reserve(mesh.NumberCell2d);
    mesh.EdgesCell2d.reserve(mesh.NumberCell2d);

    for (const string& line : lines)
    {
        // leggiamo i valori dalle righe
        istringstream convert(line);

        stringstream ss;
        string token;
        unsigned int id;
        unsigned int marker;
        unsigned int numvert;
        vector<unsigned int> vertices;
        unsigned int numed;
        vector<unsigned int> edges;

        getline(convert, token, ';');
        ss << token;
        ss >> id;
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> marker;
        ss.clear();

        getline(convert, token, ';');
        ss << token;
        ss >> numvert;
        ss.clear();
        vertices.resize(numvert);

        //dato che il numero di vertici e lati è variabile, dobbiamo leggerli con un ciclo
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            getline(convert, token, ';');
            ss << token;
            ss >> vertices[i];
            ss.clear();
        }

        getline(convert, token, ';');
        ss << token;
        ss >> numed;
        ss.clear();
        edges.resize(numed);

        //leggiamo i lati con un ciclo come i vertici
        for (size_t i = 0; i < edges.size(); ++i)
        {
            getline(convert, token, ';');
            ss << token;
            ss >> edges[i];
            ss.clear();
        }

        //aggiungiamo gli elementi ai vettori
        mesh.IdCell2d.push_back(id);
        mesh.VerticesCell2d.push_back(vertices);
        mesh.EdgesCell2d.push_back(edges);

        //aggiorniamo le mappe del numero di lati e vertici
        if (mesh.NumVerticesCell2d.find(numvert) == mesh.NumVerticesCell2d.end())
        {
            mesh.NumVerticesCell2d.insert({numvert, {id}});
        }
        else
        {
            mesh.NumVerticesCell2d[numvert].push_back(id);
        }

        if (mesh.NumEdgesCell2d.find(numed) == mesh.NumEdgesCell2d.end())
        {
            mesh.NumEdgesCell2d.insert({numed, {id}});
        }
        else
        {
            mesh.NumEdgesCell2d[numed].push_back(id);
        }
    }
    file.close(); //chiude il file
    return true;
}

bool checkedges(const PolygonalMesh& mesh)
{
    bool check = true; //variabile di controllo
    double epsilon = numeric_limits<double>::epsilon();
    for(size_t i = 0; i<mesh.VerticesCell1d.size(); ++i)
    {
        // controlliamo le lunghezze
        double length = calc::edgelength(
            mesh.CoordinatesCell0d[mesh.VerticesCell1d[i][0]](0),
            mesh.CoordinatesCell0d[mesh.VerticesCell1d[i][0]](1),
            mesh.CoordinatesCell0d[mesh.VerticesCell1d[i][1]](0),
            mesh.CoordinatesCell0d[mesh.VerticesCell1d[i][1]](1)
            );
        // se la lunghezza è al di sotto della tolleranza stampiamo un messaggio e segnaliamo check = false
        if (length < 2*(sqrt(epsilon))/(pow(3,1/4))) // dato che l'area del poligono più piccolo è un triangolo equilatero di lato L, se impongo che l'area sia maggiore di epsilon devo controllare che il lato non sia minore di L = 3^(1/4)(epsilon)^(1/2)) (uso la formula inversa)
        {
            cout << "Cella 1D con id: " << i << " ha lunghezza 0" << endl;
            check = false;
        }
    }
    // se è andato tutto a buon fine stampa un messaggio di successo
    if (check)
    {
        cout << "Tutti i segmenti hanno lunghezza maggiore di 0." << endl;
    }
    return check;
}

bool checkMarkers0d(const PolygonalMesh& mesh)
{
    bool check = true;
    // scorre gli elementi della mappa dei marker 0d
    for (const auto& markerPair : mesh.Cell0dMarkers)
    {
        unsigned int marker = markerPair.first; // memorizza il marker
        const list<unsigned int> ids = markerPair.second; // lista degli ID associati al marker

        for (unsigned int id : ids)
        {
            // trova la posizione dell'ID del punto nella mappa dei marker
            auto it = find(mesh.IdCell0d.begin(), mesh.IdCell0d.end(), id);
            if (it!=mesh.IdCell0d.end())
            {
                size_t pos = distance(mesh.IdCell0d.begin(), it);

                // trova le coordinate relative all'id
                const Vector2d coords = mesh.CoordinatesCell0d[pos];
                double x = coords(0);
                double y = coords(1);

                if (marker == 0) // se il marker è 0, i punti non devono essere sul bordo
                {
                    if (x < numeric_limits<double>::epsilon() || abs(x - 1) < numeric_limits<double>::epsilon() ||
                        y < numeric_limits<double>::epsilon() || abs(y - 1) < numeric_limits<double>::epsilon())
                    {
                        cout << "Il marker relativo alla cella 0d " << id << " è 0 ma la cella è al bordo" << endl;
                        check = false;
                    }
                }
                else // se il marker non è 0, i punti devono essere sul bordo
                {
                    if (!(x < numeric_limits<double>::epsilon() || abs(x - 1) < numeric_limits<double>::epsilon() ||
                          y < numeric_limits<double>::epsilon() || abs(y - 1) < numeric_limits<double>::epsilon())) // Se il marker è 0, i punti non devono essere sul bordo
                    {
                        cout << "Il marker relativo alla cella 0d " << id << " non è 0 ma la cella non è al bordo" << endl;
                        check = false;
                    }
                }
            }
        }
    }
    if (check)
    {
        cout << "Tutti i marker 0d sono stati memorizzati correttamente." << endl;
    }
    return check;
}

bool checkMarkers1d(PolygonalMesh& mesh)
{
    bool check = true;
    // scorre gli elementi della mappa dei marker 1d
    for (const auto& markerPair : mesh.Cell1dMarkers)
    {
        unsigned int marker = markerPair.first; // memorizza il marker
        const list<unsigned int> ids = markerPair.second; // lista degli ID associati al marker

        for (unsigned int id : ids)
        {
            // trova la posizione dell'ID del punto nella mappa dei marker
            auto it = find(mesh.IdCell1d.begin(), mesh.IdCell1d.end(), id);
            if (it!=mesh.IdCell1d.end())
            {
                int pos = distance(mesh.IdCell1d.begin(), it);
                unsigned int e1 = mesh.VerticesCell1d[pos](0);
                unsigned int e2 = mesh.VerticesCell1d[pos](1);
                // se una cella 1d con marker != 0 ha un estremo con marker 0, allora il marker è sbagliato
                if (marker != 0)
                {
                    list<unsigned int> mk = mesh.Cell0dMarkers[0];
                    auto it = find(mk.begin(), mk.end(), e1);

                    // se trova l'id di un estremo nella lista degli id 0d con marker 0 ritorna falso
                    if (it != mk.end())
                    {
                        cout << "Il segmento 1d con id " << id << " non è al bordo ma il marker non è 0" << endl;
                        check = false;
                    }
                    it = find(mk.begin(), mk.end(), e2);
                    if (it != mk.end())
                    {
                        cout << "Il segmento 1d con id " << id << " non è al bordo ma il marker non è 0" << endl;
                        check = false;
                    }
                }
                // se una cella 1d con marker 0 entrambi gli estremi con marker non 0, allora il marker è sbagliato
                else
                {
                    for (size_t i=1; i<mesh.Cell0dMarkers.size(); ++i)
                    {
                        const list<unsigned int> mk = mesh.Cell0dMarkers[i];
                        // troviamo le posizioni degli estremi nella mappa dei marker 0d
                        auto it1 = find(mk.begin(), mk.end(), e1);
                        auto it2 = find(mk.begin(), mk.end(), e2);
                        // se gli estremi appartengono alla mappa dei marker diversi da 0 allora sono al bordo
                        if (it1 != mk.end() && it2 != mk.end())
                        {
                            cout << "Il segmento 1d con id " << id << " è al bordo ma il marker è 0" << endl;
                            check = false;
                        }
                    }
                }
            }
        }
    }
    if (check)
    {
        cout << "Tutti i marker 1d sono stati memorizzati correttamente." << endl;
    }
    return check;
}


bool checkareas(PolygonalMesh& mesh)
{
    bool check = true;
    double epsilon = numeric_limits<double>::epsilon();
    double x1;
    double x2;
    double y1;
    double y2;
    for (unsigned int i = 0; i < mesh.NumberCell2d; i++){
        vector<unsigned int> edges = mesh.EdgesCell2d[i]; //prende il vettore di lati numero i
        double area;
        for (unsigned int k = 0; k < edges.size(); k++){
            const unsigned int vert1 = mesh.VerticesCell1d[edges[k]][0];
            const unsigned int vert2 = mesh.VerticesCell1d[edges[k]][1];
            x1 = mesh.CoordinatesCell0d[vert1](0);
            x2 = mesh.CoordinatesCell0d[vert2](0);
            y1 = mesh.CoordinatesCell0d[vert1](1);
            y2 = mesh.CoordinatesCell0d[vert2](1);
            area = area + (x1*y2 - y1*x2); //utilizzo la formula per calcolare l'area dei poligoni vista a lezione di geometria computazionale
        }
        area = 0.5*abs(area);
        if (area < epsilon){ //area del poligono più piccolo (triangolo equilatero)
            cout << "Il poligono ha area nulla" << endl;
            check = false;

        }

    }
    if (check)
    {
        cout << "Tutte le aree sono non nulle." << endl;
    }
    return check;
}
}
