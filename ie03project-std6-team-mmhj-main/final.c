#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<dirent.h>
#include<sqlite3.h>

#define MAX 100000

//------------以下図書検索に関わる部分---------------------------
struct averageData{
  double average;
  int index;
};

void initializeAverageDatas(struct averageData * averageDatas, int M){
  for(int i=0; i<M; i++){
    averageDatas[i].average = -1;
    averageDatas[i].index = i;
  }
}

int cmpFunc(const void *a, const void *b){
  if( (((struct averageData*)a)->average < ((struct averageData*)b)->average) ){
    return 1;
  } 
  else if( (((struct averageData*)a)->average > ((struct averageData*)b)->average) ){
    return -1;
  } else {
    return 0;
  }
}

double * returnUserScores(double **evaluations, int N, int M, int targetUser){//類似性スコアの計算
  int flug = 0;
  double dist[N];
  double *userScores;
  userScores = (double *)malloc(N*sizeof(double));

  for(int i = 0; i < N; i++)dist[i] = 0;

  for(int i=0; i<N; i++){
    flug = 0;
    for(int j=0; j<M; j++){
      if(evaluations[targetUser-1][j]<0 || evaluations[i][j]<0) continue;
      dist[i] += (evaluations[targetUser-1][j]-evaluations[i][j]) * (evaluations[targetUser-1][j]-evaluations[i][j]);
      if(flug == 0)flug = 1;
    }
    if(flug == 1)userScores[i] = 1/(sqrt(dist[i])+1);
    else userScores[i] = 0;
  }

  return userScores;
}

//--------------------------友達関係に関する部分----------------------
int v[MAX];//訪問フラグ
int adj[MAX][MAX];//隣接行列
int fri[MAX];//友達かどうかの情報を入れる配列

void bfs(int x, int N){//幅優先探索(制限:友達の友達まで)
  
  int i, j;
  int count = 0, num = 0;
  int queue[100]; /* キュー */
  int head = 0; /* 先頭データのインデックス */
  int tail = 0; /* 終端データのインデックス */

  queue[tail++] = x-1;
  v[x-1] = 1;

  while( head != tail){
    if(count == num && count > 0)break;
    if(count > 0)num++;
    i = queue[head++]; /* キューから取り出す */
    for(j = 0; j < N; j++){
      if( adj[i][j] == 1 && v[j] == 0 ){
	//printf("%d->%d ", i+1, j+1);
	queue[tail++] = j; /* キューに入れる */
	v[j] = 1;
	fri[j] = 1;
	if(i == x-1)count++;
      }
    }
  }

  printf("\n");
  
}

void friendSearch(int x, int n){//友達かどうかの判定
  for(int k = 0;k < n;k++){
    v[k] = 0;
    fri[k] = 0;
  }
  bfs(x,n);
}

//-----------------------------以下クエリに関する部分-----------------------------
/*
int document_index=0;
char word_id[100];
char selected_urlid[100];

struct Document{
    char name[100];
    double pageScore;
    double wordScore;
    double score;
};

struct Document documents[MAX];

int cmpfunc(const void *a, const void *b){
    if( (((struct Document*)a)->score < ((struct Document*)b)->score) ){
        return 1;
    } 
    else if( (((struct Document*)a)->score > ((struct Document*)b)->score) ){
        return -1;
    } else {
        return 0;
    }
}

int callbackUrl(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    int urlid;
    
    urlid = atoi(argv[0]);
    strcpy(documents[urlid-1].name, argv[1]);
    
    document_index++;

    return 0;
}

int callbackWord(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    strcpy(word_id, argv[0]);

    return 0;
}

int callbackQuery(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    int urlid;
    
    for (int i = 0; i < argc; i++) {
        urlid = atoi(argv[i]);
        documents[urlid-1].wordScore += 1;
    }

    return 0;
}

int callbackPageRank(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    int urlid;
    double pageScore;
    char * eptr;
    
    for (int i = 0; i < argc; i++) {
        urlid = atoi(selected_urlid);
        pageScore = strtod(argv[i], &eptr);
        documents[urlid-1].pageScore = pageScore;
    }

    return 0;
}
*/
//----------------------------以下command操作部分-------------------------------------
//1:(rec i)で呼び出される関数
void recommendAndPrintList(double ** evaluations, int targetUserNumber, int *indexes, int M, int N, struct averageData *averageDatas, double* userScores){
  double sumOfBookScore=0;
  double sumOfUserScore=0;
  int printCount=0;
  
  userScores = returnUserScores(evaluations, N, M, targetUserNumber);

  //identify which books should be rated
  for(int i=0; i<M; i++){
    if(evaluations[targetUserNumber-1][i] < 0){
      indexes[i] = 1;
      printCount++;
    } else {
      indexes[i] = 0;
    }
  }

  //friend search
  friendSearch(targetUserNumber, N);

  //calculate averages
  for(int i=0; i<M; i++) {
    if(indexes[i] == 1){
      for(int j=0; j<N; j++) {
	if(j == targetUserNumber-1) continue;
	if(evaluations[j][i] >= 0 && fri[j] == 1){
	  sumOfBookScore += evaluations[j][i] * userScores[j];
	  sumOfUserScore += userScores[j];
	}
      }
      if(sumOfBookScore != 0 && sumOfUserScore != 0)sumOfBookScore /= sumOfUserScore;
      averageDatas[i].average = sumOfBookScore;
      sumOfBookScore = 0.0;
      sumOfUserScore = 0;
    }
  }

  //sort average datas
  qsort(averageDatas, M, sizeof(struct averageData), cmpFunc);

  // print datas
  printf("推薦度の高い順に結果を出力します\n");
  for(int i=0; i<printCount; i++){
    if(averageDatas[i].average > 0)printf("図書番号:%d 推薦度:%.3lf\n", averageDatas[i].index+1, averageDatas[i].average);
  }

  initializeAverageDatas(averageDatas, M);
  printf("\n");
}
//2:(rec i j)で呼び出される関数
void recommend(double ** evaluations, int targetUserNumber, int *indexes, int M, int N, struct averageData *averageDatas, double* userScores, int targetBookNum){
  double sumOfBookScore=0;
  double sumOfUserScore=0;

  userScores = returnUserScores(evaluations, N, M, targetUserNumber);

  //for(int i = 0; i < N; i++)printf("%f\n",userScores[i]);

  //identify which books should be rated
  for(int i=0; i<M; i++){
    if(evaluations[targetUserNumber-1][i] < 0){
      indexes[i] = 1;
    } else {
      indexes[i] = 0;
    }
  }

  //friend search
  friendSearch(targetUserNumber, N);

  //calculate averages
  for(int i=0; i<M; i++) {
    if(indexes[i] == 1){
      for(int j=0; j<N; j++) {
	if(j == targetUserNumber-1) continue;
	if(evaluations[j][i] >= 0 && fri[j] == 1){
	  sumOfBookScore += evaluations[j][i]*userScores[j];
	  sumOfUserScore += userScores[j];
	}
      }
      if(sumOfBookScore != 0 && sumOfUserScore != 0)sumOfBookScore /= sumOfUserScore;
      averageDatas[i].average = sumOfBookScore;
      sumOfBookScore = 0.0;
      sumOfUserScore = 0;
    }
  }

  //sort average datas
  qsort(averageDatas, M, sizeof(struct averageData), cmpFunc);

  // print datas
  printf("\n%d番目に推薦する図書を出力します\n",targetBookNum);
  if(averageDatas[targetBookNum-1].average > 0)printf("図書番号:%d 推薦度:%.3lf\n", averageDatas[targetBookNum-1].index+1, averageDatas[targetBookNum- 1].average);
  else printf("該当の図書はありませんでした\n");

  initializeAverageDatas(averageDatas, M);
  printf("\n");
}

//3:図書の評価の更新(eval i j v)
void evaluate(double **evaluations, int userNum, int bookNum, double newEvaluation){
  if(userNum<=0 || bookNum<=0 || newEvaluation<0 || newEvaluation>5) {
    printf("\ninvalid input on evaluate:無効な入力です\n");
    return;
  }
  evaluations[userNum-1][bookNum-1] = newEvaluation;
  printf("\n該当の図書に関するユーザの評価を更新しました\n\n");
}

//4:ユーザーの削除(del i 1)
void deleteUser(double **evaluations, int userNum, int M){
  int i;
  for(i = 0; i < M; i++)evaluations[userNum-1][i] = -1;
  printf("\n該当のユーザーに関する評価データを削除しました\n\n");
}

//5:図書の削除(del j 2)
void deleteBook(double **evaluations, int bookNum, int N){
  int i;
  //for(i = 0; i < N; i++)printf("%f\n",evaluations[i][bookNum-1]);
  for(i = 0; i < N; i++)evaluations[i][bookNum-1] = -1;
  printf("\n該当の図書に関する評価データを削除しました\n\n");
}

//6:友人関係の削除(del i j 3)
void deleteFriend(int s, int t){
  if(adj[s-1][t-1] == 1){
  adj[s-1][t-1] = 0;
  adj[t-1][s-1] = 0;
  printf("\n%dと%dは友達ではなくなりました\n\n",s,t);
  }
  else printf("\n%dと%dは元々友達ではありません\n\n",s,t);
}

//9:友人関係の追加(ins i j 3)
void insertFriend(int s, int t){
  if(adj[s-1][t-1] == 0){
  adj[s-1][t-1] = 1;
  adj[t-1][s-1] = 1;
  printf("\n%dと%dは友達になりました\n\n",s,t);
  }
  else printf("\n%dと%dは既に友達です\n\n",s,t);
}

//7:クエリの文章検索(que 1)
/*
int searchWithText(){
    char queries[10][100];
    char input[10000];
    double maxWordScore=-1;
    double maxPageScore=-1;
    int query_num=0;

    //open database
    sqlite3 *db;
    char *err_msg = 0;
    char *sql;
    int rs = 0;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    //open database end

    //project8
    for(int i=0; i<10; i++){
        strcpy(queries[i], "");
    }

    scanf("%s", queries[0]);
    scanf("%[^\n]s", input);
    sscanf(input, "%s %s %s %s %s %s %s %s %s", queries[1], queries[2], queries[3], queries[4], queries[5], queries[6], queries[7], queries[8], queries[9]);

    for(int i=0; i<10; i++){
        if(strcmp(queries[i], "")!=0) query_num++;
    }

    //project10
    //get document's url id and name
    sql = sqlite3_mprintf("select rowid, * from Urls;");
    sqlite3_exec(db, sql, callbackUrl, 0, &err_msg);

    //initialize documents word score
    for(int i=0; i<document_index; i++) documents[i].wordScore = 0;
  
    //calculate document's word score
    //get queries[0]'s wordid
    if(strcmp(queries[0], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[0]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[1]'s wordid
    if(strcmp(queries[1], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[1]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[2]'s wordid
    if(strcmp(queries[2], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[2]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[3]'s wordid
    if(strcmp(queries[3], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[3]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[4]'s wordid
    if(strcmp(queries[4], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[4]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }
    //get queries[5]'s wordid
    if(strcmp(queries[5], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[5]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[6]'s wordid
    if(strcmp(queries[6], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[6]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }
    //get queries[7]'s wordid
    if(strcmp(queries[7], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[7]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[8]'s wordid
    if(strcmp(queries[8], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[8]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[9]'s wordid
    if(strcmp(queries[9], "")!=0){
        sql = sqlite3_mprintf("select rowid from Words where word='%q';", queries[9]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get page rank score
    for(int i=0; i<document_index; i++){
        sprintf(selected_urlid, "%d", i+1);
        sql = sqlite3_mprintf("select pagerank from PageRanks where urlid='%q';", selected_urlid);
        sqlite3_exec(db, sql, callbackPageRank, 0, &err_msg);
    }

    for(int i=0; i<document_index; i++){
        if(documents[i].wordScore==query_num) documents[i].wordScore = 1;
        else documents[i].wordScore = 0;
    }

    printf("%d\n", query_num);

    //find max value of pageScore and wordScore
    for(int i=0; i<document_index; i++){
        if(documents[i].pageScore > maxPageScore) maxPageScore = documents[i].pageScore;
        if(documents[i].wordScore > maxWordScore) maxWordScore = documents[i].wordScore;
    }

    for(int i=0; i<document_index; i++){
        documents[i].pageScore /= maxPageScore;
        documents[i].wordScore /= maxWordScore;
        documents[i].score = documents[i].pageScore + documents[i].wordScore;
    }  

    qsort(documents, document_index, sizeof(struct Document), cmpfunc);

    printf("word  page   score\n");
    for(int i=0; i<document_index; i++){
        printf("%.2lf : %.2lf : %.2lf : %s\n", documents[i].wordScore, documents[i].pageScore, documents[i].score, documents[i].name);
    }

    sqlite3_close(db);

    return 0;
}
*/
//8:クエリの単語検索(que 2)
/*
int searchWithWildCard(){
    char queries[10][100];
    char input[10000];
    double maxWordScore=-1;
    double maxPageScore=-1;
    int query_num=0;

    //open database
    sqlite3 *db;
    char *err_msg = 0;
    char *sql;
    int rs = 0;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    //open database end

    //project8
    for(int i=0; i<10; i++){
        strcpy(queries[i], "");
    }

    scanf("%s", queries[0]);
    scanf("%[^\n]s", input);
    sscanf(input, "%s %s %s %s %s %s %s %s %s", queries[1], queries[2], queries[3], queries[4], queries[5], queries[6], queries[7], queries[8], queries[9]);

    for(int i=0; i<10; i++){
        if(strcmp(queries[i], "")!=0) query_num++;
    }

    //project10
    //get document's url id and name
    sql = sqlite3_mprintf("select rowid, * from Urls;");
    sqlite3_exec(db, sql, callbackUrl, 0, &err_msg);

    //initialize documents word score
    for(int i=0; i<document_index; i++) documents[i].wordScore = 0;
  
    //calculate document's word score
    //get queries[0]'s wordid
    if(strcmp(queries[0], "")!=0){
        for(int i=0; i<strlen(queries[0]); i++) {
            if (queries[0][i]=='*') queries[0][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[0]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[1]'s wordid
    if(strcmp(queries[1], "")!=0){
        for(int i=0; i<strlen(queries[1]); i++) {
            if (queries[1][i]=='*') queries[1][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[1]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[2]'s wordid
    if(strcmp(queries[2], "")!=0){
        for(int i=0; i<strlen(queries[2]); i++) {
            if (queries[2][i]=='*') queries[2][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[2]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[3]'s wordid
    if(strcmp(queries[3], "")!=0){
        for(int i=0; i<strlen(queries[3]); i++) {
            if (queries[3][i]=='*') queries[3][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[3]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[4]'s wordid
    if(strcmp(queries[4], "")!=0){
        for(int i=0; i<strlen(queries[4]); i++) {
            if (queries[4][i]=='*') queries[4][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[4]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }
    //get queries[5]'s wordid
    if(strcmp(queries[5], "")!=0){
        for(int i=0; i<strlen(queries[5]); i++) {
            if (queries[5][i]=='*') queries[5][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[5]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[6]'s wordid
    if(strcmp(queries[6], "")!=0){
        for(int i=0; i<strlen(queries[6]); i++) {
            if (queries[6][i]=='*') queries[6][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[6]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }
    //get queries[7]'s wordid
    if(strcmp(queries[7], "")!=0){
        for(int i=0; i<strlen(queries[7]); i++) {
            if (queries[7][i]=='*') queries[7][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[7]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[8]'s wordid
    if(strcmp(queries[8], "")!=0){
        for(int i=0; i<strlen(queries[8]); i++) {
            if (queries[8][i]=='*') queries[8][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[8]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get queries[9]'s wordid
    if(strcmp(queries[9], "")!=0){
        for(int i=0; i<strlen(queries[9]); i++) {
            if (queries[9][i]=='*') queries[9][i]='%';
        }
        sql = sqlite3_mprintf("select rowid from Words where word like '%q';", queries[9]);
        sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
        //calculate word score
        sql = sqlite3_mprintf("select urlid from Locations where wordid='%q';", word_id);
        sqlite3_exec(db, sql, callbackQuery, 0, &err_msg);
    }

    //get page rank score
    for(int i=0; i<document_index; i++){
        sprintf(selected_urlid, "%d", i+1);
        sql = sqlite3_mprintf("select pagerank from PageRanks where urlid='%q';", selected_urlid);
        sqlite3_exec(db, sql, callbackPageRank, 0, &err_msg);
    }

    printf("%d\n", query_num);

    //find max value of pageScore and wordScore
    for(int i=0; i<document_index; i++){
        if(documents[i].pageScore > maxPageScore) maxPageScore = documents[i].pageScore;
        if(documents[i].wordScore > maxWordScore) maxWordScore = documents[i].wordScore;
    }

    for(int i=0; i<document_index; i++){
        documents[i].pageScore /= maxPageScore;
        documents[i].wordScore /= maxWordScore;
        documents[i].score = documents[i].pageScore + documents[i].wordScore;
    }  

    qsort(documents, document_index, sizeof(struct Document), cmpfunc);

    printf("word  page   score\n");
    for(int i=0; i<document_index; i++){
        printf("%.2lf : %.2lf : %.2lf : %s\n", documents[i].wordScore, documents[i].pageScore, documents[i].score, documents[i].name);
    }

    sqlite3_close(db);

    return 0;
}
*/
//------------------------以下main文--------------------------------
int main(){
  
  int N, M, E;
  double **evaluations;
  int u, b;
  double p;
  double sumOfBookScore = 0.0;
  double sumOfUserScore = 0.0;
  double *userScores;
  int *indexes;
  struct averageData *averageDatas; 
  int printCount = 0;
  //project1-4
  char command[4];
  char targetUser[10];
  int targetUserNum = -1;
  char targetBook[10];
  int targetBookNum = -1;
  double newEvaluation = -1;
  char input[50];
  //project1-5,6
  int R, s, t;


  scanf("%d %d %d", &N, &M, &E);

  indexes = (int *)malloc(M*sizeof(int));

  evaluations = (double **)malloc(N*sizeof(double *));
  for(int i=0; i<N; i++)evaluations[i] = (double *)malloc(M*sizeof(double));

  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      evaluations[i][j] = -1;
    }
  }

  averageDatas = (struct averageData *)malloc(M*sizeof(struct averageData));
  for(int i=0; i<M; i++){
    averageDatas[i].average = -1;
    averageDatas[i].index = i;
  }

  for(int i=0; i<E; i++){
    scanf("%d %d %lf", &u, &b, &p);
    evaluations[u-1][b-1] = p;
  }

  userScores = (double *)malloc(N*sizeof(double));

  scanf("%d",&R);
  for(int i = 0; i < N; i++)adj[i][i] = 0;//配列の初期化
  for(int i = 0; i < R; i++){//隣接行列に格納
    scanf("%d%d",&s,&t);
    adj[s-1][t-1] = 1;
    adj[t-1][s-1] = 1;
  }

  //--------------------------------以下ループ-------------------------------

  while(1){

    //----------------------------コマンド一覧の表示-----------------------
    printf("\n--------------------コマンドを入力してください-------------------\n");
    printf("--------------------コマンド一覧｜説明---------------------------\n");
    printf("info 1     | N、M、Eを表示します\n");
    printf("info 2     | 隣接行列(友達関係)を表示します＼\n");
    printf("-----------------------------------------------------------------\n");
    printf("ins 1      | 新たにユーザーを追加します\n");
    printf("ins 2      | 新たに図書を追加します\n");
    printf("ins i j 3  | ユーザーiとユーザーjの友達関係を追加します\n");
    printf("-----------------------------------------------------------------\n");
    printf("del i 1    | i番目のユーザーを削除します\n");
    printf("del j 2    | j番目の図書を削除します\n");
    printf("del i j 3  | ユーザーiとユーザーjの友達関係を削除します\n");
    printf("-----------------------------------------------------------------\n");
    printf("rec i      | ユーザiに対して、推薦する図書と推薦度を降順に出力\n");
    printf("rec i j    | ユーザiに対して、j番目に推薦する図書と推薦度を出力\n");
    printf("eval i j v | ユーザiが図書jに新たな評価vを与える\n");
    printf("-----------------------------------------------------------------\n");
    printf("que 1      | クエリの単語検索に移行します\n");
    printf("que 2      | クエリの文章検索に移行します\n"); 
    printf("-----------------------------------------------------------------\n\n");

    //標準入力部分
    scanf("%s", command);
    strcpy(targetUser, " ");
    strcpy(targetBook, " ");
    
    scanf("%[^\n]s", input);
    sscanf(input, "%s %s %lf", targetUser, targetBook, &newEvaluation);
    targetUserNum = atoi(targetUser);
    targetBookNum = atoi(targetBook);

    if(targetUserNum<=0){
      printf("error\n targetUser num is not correct:値が正しくありません\n");
      targetUserNum = -1;
      targetBookNum = -1;
      newEvaluation = -1;
      continue;
    }

    if(strcmp(command, "ins")==0 && targetUserNum == 1){//追加コマンド：ユーザー
      N += 1;     
      double **values = (double **)realloc(evaluations, N * sizeof(double *));
      if(values == NULL){
        free( evaluations );
        exit( EXIT_FAILURE );
      }
      evaluations = values;
      values = NULL;
      evaluations[N] = (double *)malloc(M*sizeof(double));
      for(int i=0; i<N; i++){
	double *values2 = (double *)realloc(evaluations[i], M*sizeof(double));
        if(values2 == NULL){
          free( evaluations );
          exit( EXIT_FAILURE );
        }
        evaluations[i] = values2;
        values2 = NULL;
      }
      for(int j = 0; j < M; j++)evaluations[N-1][j] = -1;
      double *values3 = (double *)realloc(userScores, N*sizeof(double));
      if(values3 == NULL){
        free( userScores );
        exit( EXIT_FAILURE );
      }
      userScores = values3;
      values3 = NULL;
      for(int i = 0; i < N; i++) adj[N-1][i] = 0;
      for(int i = 0; i < N; i++) adj[i][N-1] = 0;
      
    } else if(strcmp(command, "ins")==0 && targetUserNum == 2){//追加コマンド：図書
      M += 1;
      int *values4 = (int *)realloc(indexes, M*sizeof(int));
      if(values4 == NULL){
	free( indexes );
	exit( EXIT_FAILURE );
      }
      indexes = values4;
      values4 = NULL;     
      for(int i=0; i<N; i++){
	double *values5 = (double *)realloc(evaluations[i], M*sizeof(double));
        if(values5 == NULL){
          free( evaluations );
          exit( EXIT_FAILURE );
        }
        evaluations[i] = values5;
        values5 = NULL;
      }
      for(int i=0; i<N; i++)evaluations[i][M-1] = -1;
      struct averageData *values6 = (struct averageData *)realloc(averageDatas, M*sizeof(struct averageData));
      if(values6 == NULL){
	free( averageDatas );
	exit( EXIT_FAILURE );
      }
      averageDatas = values6;
      values6 = NULL;
      averageDatas[M-1].average = -1;
      averageDatas[M-1].index = M-1;
      
    } else if(strcmp(command, "ins")==0 && newEvaluation>0){//追加コマンド：友達関係
      insertFriend(targetUserNum, targetBookNum);
    } else if(strcmp(command, "eval")==0){
      evaluate(evaluations, targetUserNum, targetBookNum, newEvaluation);
    } else if((strcmp(command, "rec")==0) && targetBookNum>0){
      recommend(evaluations, targetUserNum, indexes, M, N, averageDatas, userScores, targetBookNum);
    } else if((strcmp(command, "rec")==0) && targetBookNum<=0) {
      recommendAndPrintList(evaluations, targetUserNum, indexes, M, N, averageDatas, userScores);
    } else if(strcmp(command, "del")==0 && targetBookNum == 1){//削除コマンド：ユーザー
      if(targetUserNum <= N)deleteUser(evaluations, targetUserNum, M);
      else printf("この番号のユーザーは存在しません");	
    } else if(strcmp(command, "del")==0 && targetBookNum == 2){//削除コマンド：図書
      if(targetUserNum <= M)deleteBook(evaluations, targetUserNum, N);
      else printf("この番号の図書は存在しません");
    } else if(strcmp(command, "del")==0 && newEvaluation>0){//削除コマンド：友達関係
      deleteFriend(targetUserNum, targetBookNum);
      //    } else if(strcmp(command, "que")==0 && targetUserNum == 1){//クエリコマンド:単語(ワイルドカード)
      //      searchWithWildCard();
      //    } else if(strcmp(command, "que")==0 && targetUserNum == 2){//クエリコマンド:文章
      //      searchWithText();
    } else if(strcmp(command, "info")==0 && targetUserNum == 1){
      printf("%d %d\n",N ,M);
      for(int i=0; i<N; i++){
	for(int j=0; j<M; j++){
	  printf("%d %d %.1f\n",i+1, j+1, evaluations[i][j]);
	}
      }
    } else if(strcmp(command, "info")==0 && targetUserNum == 2){
      for(int i = 0; i < N; i++){
	for(int j = 0; j < N; j++)printf("%d ",adj[i][j]);
	printf("\n");
      }
    } else printf("invalid command:そのようなコマンドはありません\n");
    targetUserNum = -1;
    targetBookNum = -1;
    newEvaluation = -1;
  }

  return 0;

}
