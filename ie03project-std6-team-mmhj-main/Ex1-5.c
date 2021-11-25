#include <stdio.h>
#define MAX 10
int arr[MAX][MAX],d[MAX],f[MAX],t=1;
int path[N];
bool visited[N + 1];
void dfs(int);

int main(){
  int N,R,Q;
  scanf("%d %d",&N,&R);
 for(i=0;i<R;i++){
   for(j=0;j<R;j++){
     arr[i][j] = 0;
   }
 }     
  for(i=0;i<R;i++){
    scanf("%d %d",&s,&t);
    arr[s][t] = 1;
    arr[t][s] = 1;
  }
  scanf(%d,&Q);
  for(i = 0;i < Q; i++){
    for(j = 1;j <= N;j++){
      path[j] = j+1;
      visit[1] = true;
      dfs(1,j);
   }
    
 }
  return 0;
}


void dfs(int n, int goal){
  int x = path[n - 1];
  if (x == goal) {
    print_path(n);
  } else {
    for (int i = 0; i < M; i++) {
      int y = adjacent[x][i];
      if (y == S) break;
      if (!visited[y]) {
        path[n] = y;
        visited[y] = true;
        dfs(n + 1, goal);
        visited[y] = false;
      }
    }
  }
}#include <stdio.h>
#Define MAX 10
int arr[MAX][MAX],d[MAX],f[MAX],t=1;
int path[N];
bool visited[N + 1];
void dfs(int);

int main(){
  int N,R,Q;
  scanf("%d %d",&N,&R);
 for(i=0;i<R;i++){
   for(j=0;j<R;j++){
     arr[i][j] = 0;
   }
 }     
  for(i=0;i<R;i++){
    scanf("%d %d",&s,&t);
    arr[s][t] = 1;
    arr[t][s] = 1;
  }
  scanf(%d,&Q);
  for(i = 0;i < Q; i++){
    for(j = 1;j <= N;j++){
      path[j] = j+1;
      visit[1] = true;
      dfs(1,j);
   }
    
 }
  return 0;
}


void dfs(int n, int goal){
  int x = path[n - 1];
  if (x == goal) {
    print_path(n);
  } else {
    for (int i = 0; i < M; i++) {
      int y = adjacent[x][i];
      if (y == S) break;
      if (!visited[y]) {
        path[n] = y;
        visited[y] = true;
        dfs(n + 1, goal);
        visited[y] = false;
      }
    }
  }
}

