// chinh do chinh xac cua double o day la 1e-2
#define eps 1e-2

unordered_map<string, Edge> edges;
unordered_map<string, Junction> junctions;
unordered_map<string, Connection> connections;

void input1(string file_xml)
{
    // load the XML file
    ifstream file(file_xml);

    // variable to parse the XML file
    string line;
    Xml_line xml_line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        xml_line.setLine(line);
        if (xml_line.tag == "/net")
            break;

        if (xml_line.tag == "edge")
        {
            Edge edge;
            for (auto p : xml_line.attbs)
            {
                edge.attbs[p.first] = p.second;
            }
            while (getline(file, line))
            {
                if (line.empty())
                    continue;
                xml_line.setLine(line);
                if (xml_line.tag == "/edge")
                    break;

                Lane l;
                for (auto p : xml_line.attbs)
                {
                    l.attbs[p.first] = p.second;
                }
                edge.lanes.push_back(l);
            }
            edges[edge.attbs["id"]] = edge;
        }
        else if (xml_line.tag == "junction")
        {
            Junction junction;
            for (auto p : xml_line.attbs)
            {
                junction.attbs[p.first] = p.second;
            }
            // while (getline(file, line) && line != "</junction>") {
            //     xml_line.setLine(line);
            //     if (xml_line.tag == "incEdge") {
            //         junction.inc_edges.push_back(xml_line.attbs["id"]);
            //     }
            //     else if (xml_line.tag == "outEdge") {
            //         junction.out_edges.push_back(xml_line.attbs["id"]);
            //     }
            // }
            junctions[junction.attbs["id"]] = junction;
        }
        else if (xml_line.tag == "connection")
        {
            Connection connection;
            for (auto p : xml_line.attbs)
            {
                connection.attbs[p.first] = p.second;
            }
            connections[connection.attbs["from"] + " " + connection.attbs["to"]] = connection;
        }
    }
}

void printEdge(string nameOfEdge)
{
    auto it = edges.find(nameOfEdge);
    if (it == edges.end())
    {
        cout << "Khong tim thay canh co id: " << nameOfEdge;
        return;
    }

    Edge edge = it->second;
    Lane laned;
    int counts = 0;

    for (Lane l : edge.lanes)
    {
        if (l.attbs.find("disallow") != l.attbs.end() && l.attbs["disallow"] == "pedestrian")
        {
            laned = l;
            counts++;
        }
    }

    if (counts == 1)
    {
        cout << nameOfEdge << " " << edge.attbs["from"] << " " << edge.attbs["to"] << " " << laned.attbs["shape"] << endl;
    }
    else if (counts == 0)
    {
        cout << "Khong tim thay disallow trong canh co id: " << nameOfEdge;
    }
    else
    {
        cout << "Co nhieu hon 1 disallow trong canh co id: " << nameOfEdge;
    }
}

void printEdge2Juncs(string edge_id1, string edge_id2)
{
    auto it = connections.find(edge_id1 + " " + edge_id2);
    if (it == connections.end())
    {
        cout << "Khong tim thay connection giua 2 canh co id: " << edge_id1 << " " << edge_id2;
        return;
    }

    Connection connection = it->second;

    string via = connection.attbs["via"];
    int pos = via.find('_', via.find('_') + 1); // tim vi tri cua dau '_' thu 2 trong via
    via = via.substr(0, pos);                   // cat via tu vi tri 0 den truoc dau '_' thu 2

    Edge edge = edges[via];
    // kiem tra xem edge co disallow pedestrian khong
    Lane laned;
    int counts = 0;

    for (Lane l : edge.lanes)
    {
        if (l.attbs.find("disallow") != l.attbs.end() && l.attbs["disallow"] == "pedestrian")
        {
            laned = l;
            counts++;
        }
    }

    if (counts == 0)
    {
        cout << "Khong phai junction crossing" << endl;
        return;
    }

    cout << edge.attbs["id"] << " " << laned.attbs["shape"] << endl;
}

set<string> startEdges()
{
    set<string> res;
    for (auto junc : junctions)
    {
        Junction junction = junc.second;
        if (junction.attbs["type"] == "dead_end")
        {
            for (auto e : edges)
            {
                Edge edge = e.second;
                if (edge.attbs["from"] == junction.attbs["id"])
                {
                    res.insert(edge.attbs["id"]);
                }
            }
        }
    }
    return res;
}

void printStart()
{
    set<string> res = startEdges();
    for (string edge_id : res)
    {
        printEdge(edge_id);
    }
}

set<string> endEdges()
{
    set<string> res;
    for (auto junc : junctions)
    {
        Junction junction = junc.second;
        if (junction.attbs["type"] == "dead_end")
        {
            for (auto e : edges)
            {
                Edge edge = e.second;
                if (edge.attbs["to"] == junction.attbs["id"])
                {
                    res.insert(edge.attbs["id"]);
                }
            }
        }
    }
    return res;
}

void printEnd()
{
    set<string> res = endEdges();
    for (string edge_id : res)
    {
        printEdge(edge_id);
    }
}

void append(string &s, double x, double y)
{
    string s1 = to_string(x);
    string s2 = to_string(y);
    s1 = s1.substr(0, s1.find('.') + 3); // lay 2 chu so thap phan
    s2 = s2.substr(0, s2.find('.') + 3); // lay 2 chu so thap phan
    s += s1 + "," + s2 + "_";
}

vector<string> decompose(double len, string path)
{
    vector<string> res;
    vector<pair<double, double>> points;
    istringstream iss(path);
    string token1, token2;

    while (getline(iss, token1, ',') && getline(iss, token2, ' '))
    {
        points.push_back(make_pair(stod(token1), stod(token2)));
    }

    // khoi tao chuoi dau tien
    int id = 0;
    string s = to_string(id++) + "_";
    append(s, points[0].first, points[0].second);
    double preDis = 0;

    for (int i = 0; i < points.size() - 1; i++)
    {
        // lay toa do 2 diem lien tiep
        double x1 = points[i].first;
        double y1 = points[i].second;
        double x2 = points[i + 1].first;
        double y2 = points[i + 1].second;

        if (preDis < -eps || preDis > eps)
            append(s, x1, y1); // them diem dau tien vao chuoi, tranh duplicate

        double dis = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); // tinh khoang cach giua 2 diem
        double x = (x2 - x1) * len / dis;                                 // tinh x cua vector chi phuong
        double y = (y2 - y1) * len / dis;                                 // tinh y cua vector chi phuong

        // dis + preDis < len thi tiep tuc
        if (preDis + dis < len)
        {
            preDis += dis;
            continue;
        }
        // dis + preDis >= len thi tinh toa do diem tiep theo
        x1 = x1 + x * (len - preDis) / len;
        y1 = y1 + y * (len - preDis) / len;

        // them diem tiep theo vao chuoi
        append(s, x1, y1);
        s.pop_back();

        res.push_back(s); // them chuoi vao vector

        // tao chuoi moi
        s = to_string(id++) + "_";
        append(s, x1, y1);

        dis -= len - preDis; // tinh khoang cach con lai

        // tinh toa do cac diem tiep theo
        while (dis > len)
        {
            x1 = x1 + x;
            y1 = y1 + y;

            // them diem tiep theo vao chuoi
            append(s, x1, y1);
            s.pop_back();

            res.push_back(s); // them chuoi vao vector

            // tao chuoi moi
            s = to_string(id++) + "_";
            append(s, x1, y1);

            dis -= len; // tinh khoang cach con lai
        }

        preDis = dis;
    }

    // them diem cuoi cung vao chuoi
    if (preDis < -eps || preDis > eps)
    {
        append(s, points[points.size() - 1].first, points[points.size() - 1].second);
        s.pop_back();
        res.push_back(s);
    }

    return res;
}

string splits(double len, string id)
{
    string res;

    auto it1 = edges.find(id);
    if (it1 != edges.end())
    {
        Edge edge = it1->second;
        Lane laned;
        int counts = 0;
        for (Lane l : edge.lanes)
        {
            if (l.attbs.find("disallow") != l.attbs.end() && l.attbs["disallow"] == "pedestrian")
            {
                laned = l;
                counts++;
                break;
            }
        }
        if (counts == 0)
        {
            cout << "Khong tim thay lane co disallow = pedestrian" << endl;
            return "";
        }
        string path = laned.attbs["shape"];
        vector<string> dpath = decompose(len, path);
        for (string s : dpath)
        {
            res += edge.attbs["id"] + ' ' + s + '\n';
        }
        return res;
    }

    // auto it2 = junctions.find(id);
    // if (it2 != junctions.end()) {
    //     Junction junc = it2->second;

    //     string path = junc.attbs["shape"];
    //     vector<string> dpath = decompose(len, path);
    //     for (string s : dpath) {
    //         res += junc.attbs["id"] + ' ' + s + '\n';
    //     }
    //     return res;
    // }

    return "Khong tim thay Edge co id: " + id;
}

void allParts(double len, string file_name)
{
    set<string> t = startEdges();
    set<string> e = endEdges();
    set<string> s;

    ofstream file(file_name, ios::out);

    while (!t.empty())
    {
        string id = *t.begin();
        t.erase(t.begin());

        if (e.find(id) == e.end())
        {
            string u = splits(len, id);
            // ghi ra file AllParts.txt
            file << u;
            s.insert(id);

            // tim nhung canh noi voi canh vua duyet
            set<string> n;
            for (auto e : edges)
            {
                Edge edge = e.second;
                if (edge.attbs["from"] == id)
                {
                    n.insert(edge.attbs["id"]);
                }
            }

            // loai bo nhung canh da duyet
            for (string id : s)
            {
                n.erase(id);
            }

            // them nhung canh con lai vao tap t
            for (string id : n)
            {
                t.insert(id);
            }
        }
    }

    file.close();
}

void Bai_1() {
    input1(file_xml);

    //# a
    // printEdge("E1");

    //# b
    // printEdge2Juncs("E1", "E2");

    //# c
    // printStart();

    //# e
    // cout << splits(20, "MyEdge_for_testing");

    //# f
    // printEnd();

    //# g
    allParts(1, file_parts);
}
