#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct averageData{            //総合推薦度と書籍番号をいれる構造体
  double average;
  int index;
};

void initializeAverageDatas(struct averageData * averageDatas, int M){　　　　
    for(int i=0; i<M; i++){
      averageDatas[i].average = 0.0;
      averageDatas[i].index = i;
    }
}

int cmpfunc(const void *a, const void *b){　　　　　　　　        //総合推薦度を並び替える関数
    if( (((struct averageData*)a)->average < ((struct averageData*)b)->average) ){
      return 1;
    }
    else if( (((struct averageData*)a)->average > ((struct averageData*)b)->average) ){
      return -1;
    } else {
      return 0; 
    }
}

double * returnUserScores(double **evaluations, int N, int M, int targetUser){    //1-1のようなユーザーの類似性スコアを返す関数
  double dist[N];
  double userScores[N];

  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      if(evaluations[targetUser-1][j]<0 || evaluations[i][j]<0) continue;
      dist[i] += (evaluations[targetUser-1][j]-evaluations[i][j]) * (evaluations[targetUser-1][j]-evaluations[i][j]);
    }
    userScores[i] = 1/(sqrt(dist[i])+1);
  }

  return userScores;
}

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

  //calculate averages
  for(int i=0; i<M; i++) {
    if(indexes[i] == 1){
      for(int j=0; j<N; j++) {
	if(j == targetUserNumber-1) continue;
	if(evaluations[j][i] >= 0){
	  sumOfBookScore += evaluations[j][i]*userScores[j];
	  sumOfUserScore += userScores[j];
	}
      }
      sumOfBookScore /= sumOfUserScore;
      averageDatas[i].average = sumOfBookScore;
      sumOfBookScore = 0.0;
      sumOfUserScore = 0;
    }
  }

  //sort average datas
  qsort(averageDatas, M, sizeof(struct averageData), cmpfunc);

  // print datas
  printf("\n");
  for(int i=0; i<printCount; i++){
    printf("%d %lf\n", averageDatas[i].index+1, averageDatas[i].average);
  }

  initializeAverageDatas(averageDatas, M);
}

void recommend(double ** evaluations, int targetUserNumber, int *indexes, int M, int N, struct averageData *averageDatas, double* userScores, int targetBookNum){
  double sumOfBookScore=0;
  double sumOfUserScore=0;

  userScores = returnUserScores(evaluations, N, M, targetUserNumber);

  //identify which books should be rated
  for(int i=0; i<M; i++){
    if(evaluations[targetUserNumber-1][i] < 0){
      indexes[i] = 1;
    } else {
      indexes[i] = 0;
    }
  }



  //calculate averages
  for(int i=0; i<M; i++) {
    if(indexes[i] == 1){
      for(int j=0; j<N; j++) {
	if(j == targetUserNumber-1) continue;
	if(evaluations[j][i] >= 0){
	  sumOfBookScore += evaluations[j][i]*userScores[j];
	  sumOfUserScore += userScores[j];
	}
      }
      sumOfBookScore /= sumOfUserScore;
      averageDatas[i].average = sumOfBookScore;
      sumOfBookScore = 0.0;
      sumOfUserScore = 0;
    }
  }

  //sort average datas
  qsort(averageDatas, M, sizeof(struct averageData), cmpfunc);

  // print datas
  printf("\n");
  printf("%d %lf\n", averageDatas[targetBookNum-1].index+1, averageDatas[targetBookNum- 1].average);

  initializeAverageDatas(averageDatas, M);
}

void evaluate(double **evaluations, int userNum, int bookNum, double newEvaluation){                    
  if(userNum<=0 || bookNum<=0 || newEvaluation<0 || newEvaluation>5) {
    printf("invalid input on evaluate\n");　　　　　           //制約に適さない場合の出力
    return;
  }
  evaluations[userNum-1][bookNum-1] = newEvaluation;            //入力された新しい評価を代入
}
//add関数追加
void add(double ** evaluations, int M, int N){
  int choice,i,j,user1,user2;
  double evaluations1[N+1][M];
  double evaluations2[N][M+1];
  printf("add User->1 add Book->2 add friendrelations->3");
  scanf("%d",choice);
  if(choice == 1){ N = N + 1;
    evaluations1 = (double **)malloc(N*sizeof(double *));
    for(int i=0; i<N; i++){
      evaluations1[i] = (double *)malloc(M*sizeof(double));
    }　
       for(i=0;i<N-1;i++){
	 for(j=0;j<M;j++){
	   evaluations1[i][j] = evaluations[i][j];
	 }
       }
    for(j=0;j<M;j++)
      {
	evaluations1[N+1][M]= -1;
      }
    evaluations = (double **)malloc(N*sizeof(double *));
    for(int i=0;i<N;i++){
      evaluations[i] = (double *)malloc(M*sizeof(double));
    }
    for(i=0;i<N;i++){
      for(j=0;j<M;j++){
	evaluation[i][j] = evaluations1[i][j];
      }
    }
  }
  else if(choice == 2){M = M+1;
    evaluation2 = (double **)malloc(N*sizeof(double *));
    for(int i=0; i<N; i++){
      evaluations2[i] = (double *)malloc(M*sizeof(double));
    }　
       for(i=0;i<N;i++){
	 for(j=0;j<M-1;j++){
	   evaluations2[i][j] = evaluations[i][j];
	 }
       }
    for(j=0;j<N;j++)
      {
	evaluations2[N][M+1]= -1;
      }
    evaluations = (double **)malloc(N*sizeof(double *));
    for(int i=0;i<N;i++){
      evaluations[i] = (double *)malloc(M*sizeof(double));
    }
    for(i=0;i<N;i++){
      for(j=0;j<M;j++){
	evaluation[i][j] = evaluations2[i][j];
      }
    }
  }
  else if(choice == 3){
    printf("追加する友人関係を入力してください。");
    scanf("%d %d",&user1,&user2);
    adj[user1][user2] = 1;
    adj[user2][user1] = 1;
    
  }
}
int main(){
  int N, M, E;
  double **evaluations;                                         //２次元配列のデータにアクセスするためのポインタ?
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

  scanf("%d %d %d", &N, &M, &E);                           //ユーザー数、書籍数、評価
  indexes = (int *)malloc(M*sizeof(int));                  //書籍の数の配列の確保

  evaluations = (double **)malloc(N*sizeof(double *));    //評価を入れる配列の確保
 for(int i=0; i<N; i++){
   evaluations[i] = (double *)malloc(M*sizeof(double));
 }

  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      evaluations[i][j] = -1;                               //-1で初期化
			       }
  }

  averageDatas = (struct averageData *)malloc(M*sizeof(struct averageData));
  for(int i=0; i<M; i++){
    averageDatas[i].average = 0.0;
    averageDatas[i].index = i;
  }

  for(int i=0; i<E; i++){
    scanf("%d %d %lf", &u, &b, &p);                             //ユーザー、書籍ナンバー、評価の入力
    evaluations[u-1][b-1] = p;
  }

  userScores = (double *)malloc(N*sizeof(double));

  //project1-4 section
  printf("______________\n");

  while(1){                                                              //入力待ち状態になってから各コマンドの入力をし実行を繰り返す
    scanf("%s", command);
    strcpy(targetBook, " ");
    strcpy(targetUser, " ");
    scanf("%[^\n]s", input);                                           //改行以外の文字で構成される文字列を読み込む。\n前まで
    sscanf(input, "%s %s %lf", targetUser, targetBook, &newEvaluation); //読み込んだ文字列を三つの変数に分ける
    targetUserNum = atoi(targetUser);                                    //文字を数字として読み込む
    targetBookNum = atoi(targetBook);
    
	evaluate(evaluations, targetUserNum, targetBookNum, newEvaluation);
  } else if((strcmp(command, "rec")==0) && targetBookNum>0){
      recommend(evaluations, targetUserNum, indexes, M, N, averageDatas, userScores, targetBookNum);
  } else if((strcmp(command, "rec")==0) && targetBookNum<=0) {
    recommendAndPrintList(evaluations, targetUserNum, indexes, M, N, averageDatas, userScores);
  } else if(strcmp(command, "ins")==0){
    add(evaluations,M,N);
    } else {
      printf("invalid command\n");
  }
}

    targetBookNum = -1;
    targetUserNum = -1;
  return 0;

}
	
    
