void connectAllChains(vector<vector<TENode *>> &graph) {
    for (auto origin : P) {
        auto oldChains = getChains(graph, origin);
        auto node = graph.at(get<0>(oldChains.at(0))).at(get<1>(oldChains.at(0)));
        auto newChains = createNewChains(oldChains, H, deltaT);
        auto newPos = insert(graph, newChains, node);
        assert(checkResult(graph, newChains, newPos, node));
        
        for (auto pair : newPos) {
            spread(graph, pair.first, pair.second, H);
        }

        auto newOrder = merge(oldChains, newChains, newPos);

        assert(checkNewOrder(allTENs, newOrder));

        connectChains(allTENs, newOrder);
    }
}

void Bai_5() {
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

    printNumberOfTENodes(allTENs);
}