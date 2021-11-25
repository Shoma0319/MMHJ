#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>

char word_id[100];
char url_id[100];

struct Document{
    int numberOfURL;
    char name[100000];
    char link[10][100000];
    double pageScore;
    double wordScore;
    double score;
    char words[10][100000];
    int numberOfWords;
};

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
    
    for (int i = 0; i < argc; i++) {
        printf("url id = %s\n", argv[i]);
        strcpy(url_id, argv[i]);
    }

    return 1;
}

int callbackWord(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {
        printf("word id = %s\n", argv[i]);
        strcpy(word_id, argv[i]);
    }

    return 1;
}

int main(){
    DIR *folder;
    struct dirent *de;
    FILE *fp;
    char *base = "./source/";
    //char *base = "./test/";
    char fullFileName[100];
    int fileNamelength;
    char content[100];
    int fileCount=0;
    struct Document *documents;
    int i=0; //page number
    int j=0; //link number
    int k=0; //word number
    char queries[10][100];
    char input[10000];
    double maxWordScore=-1;
    double maxPageScore=-1;
    char pageScore_string[100];

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

    //calculate the number of files in the source directory
    folder = opendir("./source");
    //folder = opendir("./test");
    if(folder == NULL) {
        printf("Unable to read directory\n");
        return 1;
    } else {
        while((de = readdir(folder)) != NULL){
            if(de->d_type != 8) continue; //if de->d_type==8, it is file.
            fileCount++;
        }
    }
    closedir(folder);

    documents = (struct Document *)malloc(fileCount*sizeof(struct Document));

    // //read the file contents
    folder = opendir("./source");
    //folder = opendir("./test");

    if(folder == NULL) {
        printf("Unable to read directory\n");
        return 1;
    }
    else {
        while ((de = readdir(folder)) != NULL) {
            if(de->d_type != 8) continue;

            //initialize documents array
            strcpy(documents[i].name, de->d_name);
            documents[i].numberOfURL = 0;
            documents[i].numberOfWords = 0;
            documents[i].pageScore = 1;
            documents[i].wordScore = 0;

            //insert document name into Urls
            sql = sqlite3_mprintf("insert or ignore into Urls values('%q');", documents[i].name);
            sqlite3_exec(db, sql, 0, 0, &err_msg);
            //get document name's rowid
            sql = sqlite3_mprintf("select rowid from Urls where url='%q';", documents[i].name);
            sqlite3_exec(db, sql, callbackUrl, 0, &err_msg);

            //create full file name(./source/___)
            fileNamelength = strlen(de->d_name)+1;
            strncpy(fullFileName, base, fileNamelength);
            fullFileName[fileNamelength] = '\0';
            strcat(fullFileName, de->d_name);

            fp = fopen(fullFileName, "r");
            while(1) {
                fscanf(fp, "%s", content);
                if(strstr(content, ".html") != NULL) { 
                    strcpy(documents[i].link[j], content);
                    documents[i].numberOfURL += 1;
                    j++;
                } else {
                    strcpy(documents[i].words[k], content);
                    documents[i].numberOfWords += 1;
                    k++;
                    //insert word into Words
                    sql = sqlite3_mprintf("insert or ignore into Words values('%q');", content);
                    sqlite3_exec(db, sql, 0, 0, &err_msg);
                    //get word's rowid
                    sql = sqlite3_mprintf("select rowid from Words where word='%q';", content);
                    sqlite3_exec(db, sql, callbackWord, 0, &err_msg);
                    printf("wordid = %s\n", word_id);
                    printf("urlid = %s\n", url_id);
                    //insert data into Locations
                    sql = sqlite3_mprintf("insert into Locations values('%q', '%q');", word_id, url_id);
                    sqlite3_exec(db, sql, 0, 0, &err_msg);
                }
                if( feof(fp) ) break;
            }
            fclose(fp);
            i++;
            j = 0;
            k=0;
        }
    }
    closedir(folder);

    //calculate the scores of each file
    for(int i=0; i<20; i++){ //20 times
        for(int j=0; j<fileCount; j++){ //page t
            documents[j].pageScore = 0.15;
            for(int k=0; k<fileCount; k++){ //page s
                for(int l=0; l<documents[k].numberOfURL; l++){  //page s's link
                    if(strcmp(documents[k].link[l], documents[j].name)==0){
                        documents[j].pageScore += 0.85 * (documents[k].pageScore / documents[k].numberOfURL);
                    }
                }
            }
        }
    }

    for(int i=0; i<fileCount; i++){
        //get document name's id
        sql = sqlite3_mprintf("select rowid from Urls where url='%q';", documents[i].name);
        sqlite3_exec(db, sql, callbackUrl, 0, &err_msg);
        //conver page score to string
        sprintf(pageScore_string, "%lf", documents[i].pageScore);
        //insert data into PageRanks
        sql = sqlite3_mprintf("insert into PageRanks values('%q', '%q');", url_id, pageScore_string);
        sqlite3_exec(db, sql, 0, 0, &err_msg);
    }
    // close database
    sqlite3_close(db);
}