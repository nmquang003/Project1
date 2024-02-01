#include "structures.h"

void assign_key(TENode* node, int& auto_increment) {
    if (node->key != -1) return;

    node->key = auto_increment++;
    for (auto& pr : node->tgts) {
        assign_key(pr.first, auto_increment);
    }
}

bool check_bai7(int auto_increment) {
    // check auto_increment is equals number of nodes in graph
    int counts = 0;
    for (int i = 0; i < allTENs.size(); i++)
    {
        counts += allTENs.at(i).size();
    }

    if (counts != auto_increment) return false;

    // check key of each node is unique
    vector<int> keys(auto_increment, 0);

    for (int i = 0; i < allTENs.size(); i++)
    {
        for (int j = 0; j < allTENs.at(i).size(); j++)
        {
            int key = allTENs.at(i).at(j)->key;
            if (keys.at(key) == 1) return false;
            keys.at(key) = 1;
        }
    }

    return true;
}

void modify(vector<vector<TENode*>> &graph, string shapeName, int number, vector<double> minMiddleMax) {
    double expected;
    if (minMiddleMax.size() == 2) {
        expected = (minMiddleMax.at(0) + minMiddleMax.at(1))/(2*number);
    }
    else {
        expected = (minMiddleMax.at(0) + 2*minMiddleMax.at(1) + minMiddleMax.at(2))/(4*number);
    }

    for (auto& v : graph) {
        for (auto& node : v) {
            for (auto& pair : node->srcs) {
                if (pair.second->name == shapeName) {
                    pair.first->time = expected;
                }
            }

            for (auto& pair : node->tgts) {
                if (pair.second->name == shapeName) {
                    pair.first->time = expected;
                }
            }
        }
    }
}

void writeFile(vector<vector<TENode*>> &graph, string fileName) {
    ofstream file(fileName);

    for (auto& v : graph) {
        for (auto& node : v) {
            file << node->name << " " << node->time << endl;
        }
    }

    file.close();
}

void Bai_7() {
    Bai_6( );

    // bai 7
    vector<pii> initializations = getStartedNodes(allTENs);

    int auto_increment = 0;
    for (auto pair : initializations)
    {
        assign_key(allTENs.at(pair.first).at(pair.second), auto_increment);
    }

    assert(check_bai7(auto_increment));
}