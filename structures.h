#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <bits/stdc++.h>
using namespace std;
#define pii pair<int, int>

template<typename Base, typename T>
inline bool instanceof(const T* ptr)
{
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

struct Lane
{
    unordered_map<string, string> attbs;
};

struct Edge
{
    unordered_map<string, string> attbs;
    vector<Lane> lanes;
};

struct Junction
{
    unordered_map<string, string> attbs;
    // vector<string> inc_edges;
    // vector<string> out_edges;
};

struct Connection
{
    unordered_map<string, string> attbs;
};

struct Constant
{
public:
    static double v;
};

// Define and initialize static member
double Constant::v = 1;

class Point
{
public:
    double x, y;
    bool equals(Point* p)
    {
        return (this->x == p->x && this->y == p->y);
    }
};

struct ComparePoint
{
    bool operator()(const Point* a, const Point* b) const
    {
        if (a == nullptr || b == nullptr)
        {
            throw std::invalid_argument("Null pointer passed to operator<");
        }
        // sort by x, y
        return a->x == b->x ? a->y < b->y : a->x < b->x;
    }
};

class Shape
{
public:
    Point* start;
    Point* end;
    double length;
    string name;
    bool equals(const Shape* s) const
    {
        return (this->start->equals(s->start) && this->end->equals(s->end));
    }

    double time = 0;
    double getTime()
    {
        if (time == 0)
        {
            time = this->length / Constant::v;
        }
        return time;
    }
};

struct CompareShape
{
    bool operator()(const Shape* a, const Shape* b) const
    {
        if (a == nullptr || b == nullptr)
        {
            throw std::invalid_argument("Null pointer passed to operator<");
        }
        // sort by start, end
        return
            // a->name == b->name ?
            a->start->x == b->start->x ? a->start->y == b->start->y ? a->end->x == b->end->x ? a->end->y < b->end->y : a->end->x < b->end->x : a->start->y < b->start->y : a->start->x < b->start->x;
        // a->name < b->name;
    }
};

class PausingShape : public Shape
{
public:
    PausingShape()
    {
        this->time = 0;
        this->length = 0;
        this->name = "artificial";
    }
    PausingShape(double time)
    {
        this->time = time;
        this->length = 0;
        this->name = "artificial";
    }
    PausingShape(Shape* shape, string name)
    {
        this->start = shape->start;
        this->end = shape->end;
        this->length = shape->length;
        this->time = shape->getTime();
        this->name = name;
    }
    // ghi de ham getTime
    double getTime()
    {
        return this->time;
    }
};

class ArtificialShape : public PausingShape {
public:
    ArtificialShape(double time) : PausingShape(time) {}
    //Phương thức getTime được ghi đè như sau
    double getTime() {
        return this->time;
    }
};

class TENode
{
public:
    vector<pair<TENode*, Shape*>> srcs;
    Point* origin;
    double time;
    int key = -1;
    string name;
    vector<pair<TENode*, Shape*>> tgts;

    TENode() {}
    TENode(Point* origin, double time, vector<pair<TENode*, Shape*>> tgts)
    {
        this->origin = origin;
        this->time = time;
        for (auto it : tgts)
        {
            this->tgts.push_back(make_pair(it.first, it.second));
        }
        this->name = "";
    }

    void insertSource(Shape* shape)
    {
        // Nếu srcs có chứa một pair mà phần tử đầu của nó giống hệt s->start
        // thì không thêm cặp null và s vào
        // Ngược lại thi thêm cặp null và s vào
        bool flag = false;
        for (int i = 0; i < this->srcs.size(); i++)
        {
            if (this->srcs.at(i).second->equals(shape))
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            pair<TENode*, Shape*> pair = make_pair(nullptr, shape);
            this->srcs.push_back(pair);
        }
    }

    void insertTarget(Shape* s)
    {
        // Nếu tgts có chứa một pair mà phần tử đầu của nó giống hệt s->end
        // thì không thêm cặp null và s vào
        // Ngược lại thi thêm cặp null và s vào
        bool flag = false;
        for (int i = 0; i < this->tgts.size(); i++)
        {
            if (this->tgts.at(i).second->equals(s))
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            pair<TENode*, Shape*> pair =
                make_pair(nullptr, s);
            this->tgts.push_back(pair);
        }
    }

    bool equals(TENode* te_node)
    {
        return this->origin->equals(te_node->origin);
    }

    void insertSourcesAndTargets(TENode* node, vector<pii> fromNode, vector<pii> toNode)
    {
        for (int i = 0; i < fromNode.size(); i++)
        {
            int atNode = fromNode.at(i).first;
            int toThis = fromNode.at(i).second;

            if (this->srcs.at(atNode).first == nullptr)
            {
                this->srcs.at(atNode).first = node;
            }
            if (node->tgts.at(toThis).first == nullptr)
            {
                node->tgts.at(toThis).first = this;
            }
        }

        for (int i = 0; i < toNode.size(); i++)
        {
            int atNode = toNode.at(i).first;
            int fromThis = toNode.at(i).second;

            if (this->tgts.at(fromThis).first == nullptr)
            {
                this->tgts.at(fromThis).first = node;
            }
            if (node->srcs.at(atNode).first == nullptr)
            {
                node->srcs.at(atNode).first = this;
            }
        }
    }

    int indexInSources(Shape* shape)
    {
        int index = 0;

        for (auto it : srcs)
        {
            Shape* s = it.second;
            if (shape->equals(s))
            {
                return index;
            }
            index++;
        }

        return -1;
    }

    bool endOfLane()
    {
        if (this->tgts.size() <= 1)
            return true;

        for (int i = 0; i < this->srcs.size(); i++)
        {
            auto shape = this->srcs.at(i).second;
            auto name = shape->name;

            if (name.compare("artificial") == 0) continue;
            
            for (int j = 0; j < this->tgts.size(); j++)
            {
                auto nextShape = this->tgts.at(j).second;
                auto nextName = nextShape->name;

                if (nextName.compare("artificial") == 0 || nextName.compare(name) == 0) continue;
                
                return true;
            }
            
        }

        return false;
    }

    string isStation(string stations) {
        for (int i = 0; i < this->srcs.size(); i++)
        {
            auto shape = this->srcs.at(i).second;
            auto name = shape->name;

            if (name.compare("artificial") == 0 || stations.find("$" + name + "$") == string::npos) continue;
            return name;
        }

        return "";
    }

    virtual TENode* transform(TENode* node) {
        return this;
    }

    virtual void connect2Artificial(TENode* node) {

    }

    virtual void createConnection(TENode* node) {

    }
};

class Station : public TENode
{
public:
    Station() : TENode() {}
    Station(TENode* temp, string name) : TENode()
    {
        this->srcs = temp->srcs;
        this->name = name;
        this->tgts = temp->tgts;
        this->origin = temp->origin;
        this->time = temp->time;

        for (auto& pr : this->srcs)
        {
            for (auto& pt : pr.first->tgts)
            {
                if (pt.first->equals(temp))
                {
                    pt.first = this;
                }
            }
        }

        for (auto& pr : this->tgts)
        {
            for (auto& pt : pr.first->srcs)
            {
                if (pt.first->equals(temp))
                {
                    pt.first = this;
                }
            }
        }
    }
};

class ArtificialStation : public Station {
public:
    double bestTime;
    double amplitude;
    double earliness;
    double tardiness;

    ArtificialStation(string name, double bestTime, double amplitude) : Station() {
        this->name = name;
        this->time = bestTime;
        this->bestTime = bestTime;
        this->amplitude = amplitude;
        this->earliness = bestTime - amplitude;
        this->tardiness = bestTime + amplitude;
    }

    void createConnection(TENode* node) {
        if (instanceof<Station>(node) && !instanceof<ArtificialStation>(node)) {
            if (node->name.compare(this->name) == 0) {
                auto penaltyT = max(this->earliness - node->time, 0.0);
                penaltyT = max(penaltyT, node->time - this->tardiness);
                auto aShape = new ArtificialShape(penaltyT);
                node->tgts.push_back(make_pair(this, aShape));
                this->srcs.push_back(make_pair(node, aShape));
            }
        }
    }

};

#endif // !STRUCTURES_H 