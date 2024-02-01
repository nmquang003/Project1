#define tiid tuple<int, int, double>
#define pid pair<int, double>

// # a
vector<tiid> getChains(const vector<vector<TENode *>> &graph, Point *origin)
{
    vector<tiid> chains;

    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph.at(i).size(); j++)
        {
            TENode *node = graph.at(i).at(j);
            if (node->origin->equals(origin))
            {
                chains.push_back(make_tuple(i, j, node->time));
                break;
            }
        }
    }

    // xap sep theo thoi gian tang dan
    sort(chains.begin(), chains.end(), [](tiid a, tiid b) {return get<2>(a) < get<2>(b);});

    return chains;
}

// # b
vector<double> createNewChains(vector<tiid> oldChains, double H, double deltaT)
{
    vector<double> newChains;
    oldChains.push_back(make_tuple(-1, -1, H));

    for (int count = 0; count < oldChains.size() - 1; count++)
    {
        auto prev = oldChains.at(count);
        auto next = oldChains.at(count + 1);

        if (get<2>(prev) < get<2>(next) - deltaT)
        {   
            int steps = floor((get<2>(next) - get<2>(prev)) / deltaT);

            if (get<2>(prev) + steps * deltaT == get<2>(next))
            {
                steps--;
            }

            steps = min(steps, (int) floor((H - get<2>(prev)) / deltaT)); // kiem tra xem co vuot qua H hay khong

            for (int i = 1; i <= steps; i++)
            {
                newChains.push_back(get<2>(prev) + deltaT * i);
            }
        }
    }

    return newChains;
}

// # c
vector<pii> insert(vector<vector<TENode *>> &graph, vector<double> newChains, TENode *node)
{
    vector<pii> result;
    for (int i = 0; i < newChains.size(); i++)
    {        
        TENode *newNode = new TENode(node->origin, newChains.at(i), node->tgts);
    
        result.push_back(insert(graph, newNode));
    }

    return result;
}

// # d
bool checkResult(const vector<vector<TENode *>> &graph, vector<double> newChains, vector<pii> newPositions, TENode *p)
{
    if (newChains.size() != newPositions.size())
    {
        return false;
    }
    
    for (int i = 0; i < newChains.size(); i++)
    {
        auto pair = newPositions.at(i);
        auto time = newChains.at(i);

        if (graph.at(pair.first).at(pair.second)->origin != p->origin || 
            graph.at(pair.first).at(pair.second)->time != time)
        {
            return false;
        }
    }
    return true;
}

// # e
vector<pii> merge(vector<tiid> oldChains, vector<double> newChains, vector<pii> newPositions)
{
    vector<pii> result;
    int i = 0, j = 0;

    while (i < oldChains.size() && j < newChains.size())
    {
        if (get<2>(oldChains.at(i)) < newChains.at(j))
        {
            result.push_back(make_pair(get<0>(oldChains.at(i)), get<1>(oldChains.at(i))));
            i++;
        }
        else
        {
            result.push_back(newPositions.at(j));
            j++;
        }
    }

    for (int k = i; k < oldChains.size(); k++)
    {
        result.push_back(make_pair(get<0>(oldChains.at(k)), get<1>(oldChains.at(k))));
    }

    for (int k = j; k < newChains.size(); k++)
    {
        result.push_back(newPositions.at(k));
    }

    return result;
}

bool checkNewOrder(vector<vector<TENode *>> graph, vector<pii> newOrder)
{
    for (int i = 0; i < newOrder.size() - 1; i++)
    {
        auto prevPos = newOrder.at(i);
        auto nextPos = newOrder.at(i + 1);

        auto prev = graph.at(prevPos.first).at(prevPos.second);
        auto next = graph.at(nextPos.first).at(nextPos.second);

        if (prev->time > next->time)
        {
            return false;
        }
    }
    return true;
}

// # f
vector<vector<TENode *>> connectChains(const vector<vector<TENode *>> &graph, vector<pii> newOrder)
{
    for (int i = 0; i < newOrder.size() - 1; i++)
    {
        auto prevPos = newOrder.at(i);
        auto nextPos = newOrder.at(i + 1);

        auto prev = graph.at(prevPos.first).at(prevPos.second);
        auto next = graph.at(nextPos.first).at(nextPos.second);

        PausingShape *pausingShape = new PausingShape(next->time - prev->time);

        pausingShape->start = prev->origin;
        pausingShape->end = next->origin;

        prev->tgts.push_back(make_pair(next, pausingShape));
        next->srcs.push_back(make_pair(prev, pausingShape));
    }

    return graph;
}

void sortGraph(vector<vector<TENode *>> &graph)
{
    sort(graph.begin(), graph.end(), [](vector<TENode *> a, vector<TENode *> b) {return a.at(0)->time < b.at(0)->time;});
}

void Bai_4()
{
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

    // Bai 4
    initializations = getStartedNodes(allTENs);
    for (auto pair : initializations)
    {
        TENode *temp = allTENs.at(pair.first).at(pair.second);
        auto oldChains = getChains(allTENs, temp->origin);
        auto newChains = createNewChains(oldChains, H, deltaT);
        auto newPos = insert(allTENs, newChains, temp);

        assert(checkResult(allTENs, newChains, newPos, temp));

        for (auto pair : newPos)
        {   
            spread(allTENs, pair.first, pair.second, H);
        }

        auto newOrder = merge(oldChains, newChains, newPos);

        assert(checkNewOrder(allTENs, newOrder));

        connectChains(allTENs, newOrder);
    }

    redundantNode = filter(allTENs);
    remove(allTENs, redundantNode);
    sortGraph(allTENs);

    // Assert_Bai2(); // các cạnh có thể xuất hiện nhiều hơn 2 lần rồi nên assert bị sai
    Assert_Bai3();

    // in ra so luong TENodes trong allTENs
    printNumberOfTENodes(allTENs);
}