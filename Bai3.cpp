#define pts pair<TENode *, Shape *>

//# b
pii isAvailable(const vector<vector<TENode *>> &graph, Point *origin, double time)
{

    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph.at(i).size(); j++)
        {
            TENode *node = graph.at(i).at(j);
            if (node->origin->equals(origin) && node->time == time)
            {
                return make_pair(i, j);
            }
        }
    }
    
    return make_pair(-1, -1);
}

//# c
bool isAvailable(const vector<vector<TENode *>> &graph, double time, int *index)
{
    for (int i = 0; i < graph.size(); i++)
    {
        for (auto node : graph.at(i))
        {
            if (node->time == time)
            {
                *index = i;
                return true;
            }
        }
    }
    *index = graph.size();
    return false;
}

//# d
pii insert(vector<vector<TENode *>> &graph, TENode *ten)
{
    auto result = isAvailable(graph, ten->origin, ten->time);

    if (result.first != -1)
    {
        return result;
    }

    int index = 0;
    if (isAvailable(graph, ten->time, &index))
    {
        graph.at(index).push_back(ten);
        result = make_pair(index, graph.at(index).size() - 1);
    }
    else
    {
        vector<TENode *> v;
        v.push_back(ten);
        graph.push_back(v);

        // sap xep lai graph theo thoi gian tang dan bang insertion sort
        int i = graph.size() - 1;
        // while (i > 0 && graph.at(i).at(0)->time < graph.at(i - 1).at(0)->time)
        // {
        //     swap(graph.at(i), graph.at(i - 1));
        //     i--;
        // }
        result = make_pair(i, 0);
    }

    return result;
}

//# f
void spread(vector<vector<TENode *>> &graph, int m, int n, double H)
{
    TENode *node = graph.at(m).at(n);
    queue<TENode *> Q;
    Q.push(node);
    while (!Q.empty())
    {
        TENode *temp = Q.front();
        Q.pop();

        for (auto &pair : temp->tgts)
        {
            Shape *shape = pair.second;
            double time = temp->time + shape->getTime();

            if (time < H)
            {
                Point *origin = shape->end;
                TENode *node = pair.first;
                auto pos = isAvailable(graph, origin, time);
                TENode *foundItem = nullptr;

                if (pos.first != -1)
                {
                    foundItem = graph.at(pos.first).at(pos.second);
                }
                else
                {
                    TENode *newNode = new TENode(origin, time, node->tgts);
                    foundItem = newNode;
                    insert(graph, foundItem); // gọi hàm đã làm ở câu (d)
                }

                pair.first = foundItem;

                int index = foundItem->indexInSources(shape);

                if (index != -1)
                {
                    foundItem->srcs.at(index).first = temp;
                }
                else
                {
                    foundItem->srcs.push_back(make_pair(temp, shape));
                }

                Q.push(foundItem);
            }
        }
    }
}

//# g
vector<pii> filter(const vector<vector<TENode *>> &graph)
{
    vector<pii> result;
    for (int u = 0; u < graph.size(); u++)
    {
        for (int v = 0; v < graph.at(u).size(); v++)
        {
            TENode *node = graph.at(u).at(v);
            for (auto pair : node->srcs)
            {
                TENode *src = pair.first;
                Shape *shape = pair.second;
                if (src != nullptr && shape != nullptr)
                {
                    if (node->time < src->time + shape->getTime())
                    {
                        result.push_back(make_pair(u, v));
                        break;
                    }
                }
            }
        }
    }
    return result;
}

//# h
void remove(vector<vector<TENode *>> &graph, const vector<pii> &filter)
{
    for (auto pair : filter)
    {
        int u = pair.first;
        int v = pair.second;
        graph.at(u).at(v) = nullptr;
    }

    for (int i = 0; i < graph.size(); i++)
    {
        vector<TENode *> tmp = vector<TENode *>();
        for (int j = 0; j < graph.at(i).size(); j++)
        {
            if (graph.at(i).at(j) != nullptr)
            {
                tmp.push_back(graph.at(i).at(j));
            }
        }
        graph.at(i) = tmp;
    }

    vector<vector<TENode *> > tmp = vector<vector<TENode *>>();
    for (int i = 0; i < graph.size(); i++)
    {
        if (graph.at(i).size() > 0)
        {
            tmp.push_back(graph.at(i));
        }
    }

    graph = tmp;
}

//# i
vector<pii> getStartedNodes(const vector<vector<TENode *>> &graph)
{
    vector<pii> result;
    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph.at(i).size(); j++)
        {
            TENode *node = graph.at(i).at(j);
            if (node->srcs.size() == 0)
            {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

//# j
bool checkTime(const vector<vector<TENode *>> &graph)
{
    // kiem tra time cua cac TENodes
    for (auto v_node : graph)
    {
        for (auto node : v_node)
        {
            vector<pts> srcs = node->srcs;
            for (auto pair : srcs)
            {
                TENode *src = pair.first;
                Shape *shape = pair.second;
                if (node->time != src->time + shape->getTime())
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void Assert_Bai3()
{
    // kiem tra thoi gian cua cac TENodes
    assert(checkTime(allTENs));
}

void printNumberOfTENodes(const vector<vector<TENode *>> &graph)
{
    int counts = 0;
    for (int i = 0; i < graph.size(); i++)
    {
        counts += graph.at(i).size();
    }
    cout << "So luong TENode: " << counts << endl;
}

int Bai_3() {
    // Bai 2
    input2(file_parts);
    solve2();
    Assert_Bai2();

    //Bai 3
    vector<pii>  initializations = getStartedNodes(allTENs);

    for (auto pair :  initializations) {
        spread(allTENs, pair.first, pair.second, H);
    }

    vector<pii> redundantNode = filter(allTENs);
    remove(allTENs, redundantNode);

    // kiem tra lai
    Assert_Bai2();
    Assert_Bai3();

    // in ra so luong TENodes trong allTENs
    printNumberOfTENodes(allTENs);

    return 0;
}