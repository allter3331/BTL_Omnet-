/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package generatefattree;

import java.util.List;

/**
 *
 * @author Dell
 */
public abstract class Graph {
    protected int V;
    protected int E;
    protected List<Integer>[] adj;

    public void addEdge(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        E++;
        adj[v].add(w);
        adj[w].add(v);
    }

    public void removeEdge(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        E--;
        adj[v].remove((Object)w);
        adj[w].remove((Object)v);
    }

    private void validateVertex(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V - 1));
    }

    public boolean hasEdge(int u, int v) {
        return adj[u].contains(v);
    }

    public int V() { return V; }

    public List<Integer> adj(int v) { return adj[v]; }

    public int degree(int u) {
        return adj[u].size();
    }

    public abstract List<Integer> hosts();

    public abstract List<Integer> switches();

    public abstract boolean isHostVertex(int v);

    public abstract boolean isSwitchVertex(int v);

    public double pathCableLength(List<Integer> path) {
        return 0;
    }
}
