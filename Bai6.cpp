#include "structures.h"

void replaceStation(vector<vector<TENode*>>& graph, string stations)
{
    for (int i = 0; i < graph.size(); i++)
    {
        auto v = graph.at(i);
        for (int j = 0; j < v.size(); j++)
        {
            auto temp = v.at(j);
            if (temp->endOfLane())
            {
                auto name = temp->isStation(stations);
                if (!name.empty())
                {
                    graph.at(i).at(j) = new Station(temp, name);
                }
            }
        }
    }
}

map<string, vector<ArtificialStation*>> getTimeWindows(string fileName, double H, string* stations) {
    ifstream file(fileName);
    map<string, vector<ArtificialStation*>> result;

    string line;
    while (getline(file, line)) {
        // route_0 E0 E0 J1 J1 3 2 1
        istringstream iss(line);
        string name;
        double bestTime;
        double amplitude;
        double period;
        string tmp;
        iss >> tmp >> tmp >> name >> tmp >> tmp >> period >> bestTime >> amplitude;

        vector<ArtificialStation*> values;
        for (double i = 0.0; i < H; i += period) {
            values.push_back(new ArtificialStation(name, bestTime + i, amplitude));
        }

        if (values.size() > 0) {
            result.insert(make_pair(name, values));
            stations->append("$" + name + "$");
        }
    }

    return result;
}

void insertAS(vector<vector<TENode *>> &graph, TENode *ten)
{
    int index = 0;
    if (isAvailable(graph, ten->time, &index))
    {
        graph.at(index).push_back(ten);
    }
    else
    {
        vector<TENode *> v;
        v.push_back(ten);
        graph.push_back(v);
    }
}

void Bai_6() {
    // Bai 2
    input2(file_parts);
    solve2();
    Assert_Bai2();

    // Bai 3
    vector<pii> initializations = getStartedNodes(allTENs);

    for (auto pair : initializations)
    {
        spread(allTENs, pair.first, pair.second, H);
    }

    vector<pii> redundantNode = filter(allTENs);
    remove(allTENs, redundantNode);

    // kiem tra lai
    Assert_Bai2();
    Assert_Bai3();

    // Bai 5
    connectAllChains(allTENs);

    redundantNode = filter(allTENs);
    remove(allTENs, redundantNode);
    sortGraph(allTENs);
    Assert_Bai3();

    // Bai 6
    string stations = "";
    map<string, vector<ArtificialStation*>> mapArtificialStations = getTimeWindows(file_routes, H, &stations);
    
    replaceStation(allTENs, stations);

    for (int i = 0; i < allTENs.size(); i++) {
        auto v = allTENs.at(i);
        for (int j = 0; j < v.size(); j++) {
            auto temp = v.at(j);
            if (instanceof<Station>(temp)) {
                string name = temp->name;
                auto found_it = mapArtificialStations.find(name);
                if (found_it != mapArtificialStations.end()) {
                    auto allArStations = mapArtificialStations[name];
                    for (auto elem : allArStations) {
                        elem->createConnection(temp);
                    }
                }
            }
        }
    }

    for (const auto& el : mapArtificialStations) {
        auto v = el.second; 
        for (auto elem : v) {
            insertAS(allTENs, elem);
        }
    }
    sortGraph(allTENs);
    

    printNumberOfTENodes(allTENs);
}
