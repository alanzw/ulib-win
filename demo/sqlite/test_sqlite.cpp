#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    return 1;
  }

  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  //int nRow, nCol;
  //char **ppReslut;
  //rc = sqlite3_get_table(db, "", &ppReslut, &nRow, &nCol, &zErrMsg);
  //if (rc)
  //{
  //    fprintf(stderr, "Error in getting tables: %s\n", sqlite3_errmsg(db));
  //    sqlite3_free_table(ppReslut);
  //    sqlite3_close(db);
  //    return 2;
  //}
  //else
 // {
  //    printf("%s\n", *ppReslut);
  //}

  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  sqlite3_close(db);
 
  return 0;
}
