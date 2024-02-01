#define pts pair<TENode *, Shape *>

void input2(string file_name)
{
    vector<TENode *> tempTENs;
    allTENs.push_back(tempTENs);

    ifstream file(file_name);
    string line;
    while (getline(file, line))
    {
        // line = readLine(file);
        // nameOfShape = getName(line);
        // index = getIndex(line);
        // firstPoint = getFirstPoint(line);
        // lastPoint = getLastPoint(line);
        // shape = getShape(firstPoint, lastPoint, line);
        string nameOfShape;
        string index;
        Point *firstPoint = new Point;
        Point *lastPoint = new Point;
        Shape *shape = new Shape;

        istringstream iss(line);
        getline(iss, nameOfShape, ' ');
        getline(iss, index, '_');

        string x = "";
        string y = "";

        getline(iss, x, ',');
        getline(iss, y, '_');
        firstPoint->x = stod(x);
        firstPoint->y = stod(y);

        getline(iss, x, ',');
        getline(iss, y);
        lastPoint->x = stod(x);
        lastPoint->y = stod(y);

        shape->name = nameOfShape;
        shape->start = firstPoint;
        shape->end = lastPoint;
        shape->length = sqrt(pow((lastPoint->x - firstPoint->x), 2) + pow((lastPoint->y - firstPoint->y), 2));

        // if (firstPoint ∉ P)
        if (P.find(firstPoint) == P.end())
        {
            P.insert(firstPoint);
            TENode *n1 = new TENode;
            n1->origin = firstPoint;
            n1->time = 0;
            n1->srcs = vector<pts>();
            n1->tgts = vector<pts>();
            allTENs.at(0).push_back(n1);
        }
        // if (lastPoint ∉ P)
        if (P.find(lastPoint) == P.end())
        {
            P.insert(lastPoint);
            TENode *n2 = new TENode;
            n2->origin = lastPoint;
            n2->time = 0;
            n2->srcs = vector<pts>();
            n2->tgts = vector<pts>();
            allTENs.at(0).push_back(n2);
        }
        // if (shape ∉ S)
        if (S.find(shape) == S.end())
        {
            S.insert(shape);
        }
    }
}

vector<pii> getCoincidences(vector<pts> srcs, vector<pts> tgts)
{
    vector<pii> result;
    for (int i = 0; i < srcs.size(); i++)
    {
        for (int j = 0; j < tgts.size(); j++)
        {
            if (srcs.at(i).second->equals(tgts.at(j).second))
            {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

void solve2()
{
    for (TENode *node : allTENs.at(0))
    {
        for (Shape *shape : S)
        {
            if (shape->start->equals(node->origin))
            {
                node->insertTarget(shape);
            }
            else if (shape->end->equals(node->origin))
            {
                node->insertSource(shape);
            }
        }
    }

    for (TENode *nodeA : allTENs.at(0))
    {
        for (TENode *nodeB : allTENs.at(0))
        {
            vector<pii> nodeA_to_nodeB;
            vector<pii> nodeB_to_nodeA;
            if (!nodeA->equals(nodeB))
            {
                nodeB_to_nodeA = getCoincidences(nodeA->srcs, nodeB->tgts);
                nodeA_to_nodeB = getCoincidences(nodeB->srcs, nodeA->tgts);
                nodeA->insertSourcesAndTargets(nodeB, nodeB_to_nodeA, nodeA_to_nodeB);
            }
        }
    }
}

bool check1()
{
    // kiem tra moi shape chi xuat hien 2 lan
    for (Shape *shape : S)
    {
        int counts = 0;
        for (vector<TENode *> v_node : allTENs)
        {
            for (TENode *node : v_node)
            {
                for (pts src : node->srcs)
                {
                    if (src.second->equals(shape))
                    {
                        counts++;
                    }
                }
                for (pts tgt : node->tgts)
                {
                    if (tgt.second->equals(shape))
                    {
                        counts++;
                    }
                }
            }
        }
        if (counts != 2)
        {
            cout << counts << endl;
            return false;
        }
    }
    return true;
}

bool check2()
{
    // kiem tra moi diem chi xuat hien 1 lan
    for (Point *point : P)
    {
        int counts = 0;
        for (vector<TENode *> v_node : allTENs)
        {
            for (TENode *node : v_node)
            {
                if (node->origin->equals(point))
                {
                    counts++;
                }
            }
        }
        if (counts != 1)
        {
            return false;
        }
    }
    return true;
}

bool check3()
{
    // kiem tra moi node co srcs va tgts
    for (vector<TENode *> v_node : allTENs)
    {
        for (TENode *node : v_node)
        {
            for (pts src : node->srcs)
            {
                if (src.first == nullptr)
                {
                    return false;
                }
                if (src.second == nullptr)
                {
                    return false;
                }
            }
            for (pts tgt : node->tgts)
            {
                if (tgt.first == nullptr)
                {
                    return false;
                }
                if (tgt.second == nullptr)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool check4()
{
    // kiem tra moi node xuat hien trong srcs va tgts cua cac node khac dung so lan
    for (vector<TENode *> v_node1 : allTENs)
    {
        for (TENode *node1 : v_node1)
        {
            int counts = 0;
            int appear = node1->srcs.size() + node1->tgts.size();

            for (vector<TENode *> v_node2 : allTENs)
            {
                for (TENode *node2 : v_node2)
                {
                    if (node1->equals(node2))
                    {
                        continue;
                    }
                    for (pts src : node2->srcs)
                    {
                        if (src.first->equals(node1))
                        {
                            counts++;
                        }
                    }
                    for (pts tgt : node2->tgts)
                    {
                        if (tgt.first->equals(node1))
                        {
                            counts++;
                        }
                    }
                }
            }

            if (counts != appear)
            {
                return false;
            }
        }
    }
    return true;
}

bool check5()
{
    // kiem tra moi node o allTens[0] co time = 0
    for (TENode *node : allTENs.at(0))
    {
        if (node->time != 0)
        {
            return false;
        }
    }
    return true;
}

void Assert_Bai2()
{
    assert(check1()); // kiem tra moi shape chi xuat hien 2 lan
    assert(check2()); // kiem tra moi diem chi xuat hien 1 lan
    assert(check3()); // kiem tra moi node co srcs va tgts
    assert(check4()); // kiem tra moi node xuat hien trong srcs va tgts cua cac node khac dung so lan
    assert(check5()); // kiem tra moi node co time = 0
}

void Bai_2() {
    input2(file_parts);
    solve2();

    //# c
    Assert_Bai2();
    
    cout << "Done!";
}