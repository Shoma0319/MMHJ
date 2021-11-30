#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>
#define MAX 100000

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

int searchWithWords(){
    char queries[10][100];
    char input[10000];
    double maxWordScore=-1;
    double maxPageScore=-1;
    int query_num=10;

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

    //重複削除
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(i==j) continue;
            if(strcmp(queries[i], queries[j])==0){
                strcpy(queries[j], "");
            }
        }
    }

    //project9
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

int main(){
    searchWithWords();
    return 0;
}
