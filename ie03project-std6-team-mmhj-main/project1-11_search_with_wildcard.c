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
