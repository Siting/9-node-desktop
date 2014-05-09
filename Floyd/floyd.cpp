#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define NOPATH -1
int starting_column = 3;

void print(int i, int j, int **mid) {
    if (mid[i][j] == -1) 
        cout << i+1 << " ";
    else {
        if (mid[i][j] != j) print(i,mid[i][j],mid);
        if (mid[i][j] != i) print(mid[i][j],j,mid);
    }
}

void savePathInfo(int i, int j, int **paths, int **mid, int paths_row) {
    if (mid[i][j] == -1) {
        paths[paths_row][starting_column++] = i+1;

    }
    else {
        if (mid[i][j] != j) savePathInfo(i,mid[i][j],paths, mid, paths_row);
        if (mid[i][j] != i) savePathInfo(mid[i][j],j,paths, mid, paths_row);
    }
    
}
        

int main(int argc, char **argv) {
    int n;
    int numFlows;
    
    FILE * (file);

    cin >> n;
    cin >> numFlows;

    int i,j,k;
    int adj[n][n];
    int *mid[n];
    int paths_row, paths_column;
    int *paths[numFlows];


    for (i=0; i<n; i++) {
        mid[i] = (int *)malloc(sizeof(int)*n);
        for (j=0; j<n; j++) {
            adj[i][j] = NOPATH;
            mid[i][j] = -1;
        }
    }
    
    for (paths_row=0; paths_row<numFlows; paths_row++) {
        paths[paths_row] = (int *)malloc(sizeof(int)*(n+3)); // 3: O, D, Cost
        for (paths_column=0; paths_column<(n+3); paths_column++) {
            paths[paths_row][paths_column] = -1;
        }
    }

    while (cin >> i >> j >> k) {
        adj[i][j] = k;
        adj[j][i] = k;
    }

    for (i=0; i<n; i++)
        for (j=0; j<n; j++) {
            if (i==j) continue;
            for (k=0; k<n; k++) {
                if (i == k || j == k 
                    || adj[i][k] == NOPATH 
                    || adj[k][j] == NOPATH) 
                    continue;
                if (adj[i][j] == NOPATH 
                    || adj[i][k] + adj[k][j] < adj[i][j]) {
                    adj[i][j] = adj[i][k] + adj[k][j];
                    mid[i][j] = k;
                }
            }
        }
    
    paths_row = 0;

    for (i=0; i<j; i++)
        for (j=i+1; j<n; j++) {
            
            cout << i+1 << "->" << j+1 << " cost: " << adj[i][j]
                 << " path: ";
            print(i,j,mid); 
            cout << j+1 << endl;
            
            paths[paths_row][0] = i+1;
            paths[paths_row][1] = j+1;
            paths[paths_row][2] = adj[i][j];
            starting_column = 3;
            savePathInfo(i,j,paths, mid, paths_row);
            paths[paths_row][starting_column] = j+1;
            
            paths_row++;
        }
    
    file=fopen("shortest_paths.txt","w");
    
    for (paths_row=0; paths_row<numFlows; paths_row++){
        for (paths_column=0; paths_column<n+3; paths_column++) {
            fprintf (file, "%d ",paths[paths_row][paths_column]);
            //cout << paths[paths_row][paths_column] << " ";
        }
        fprintf (file,"\n");
        //cout << endl;
    }
    
    fclose(file);
    
    return 0;
}
