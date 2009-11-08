@echo off

test_sqlite ex.db "SELECT date('now');"

test_sqlite ex.db "SELECT date('now','start of month','+1 month','-1 day');"

test_sqlite ex.db "SELECT datetime(1092941466, 'unixepoch');"

rem Create a new database
rem test_sqlite ex.db "CREATE TABLE ex_t1(one varchar(10), two smallint);"
rem test_sqlite ex.db "CREATE TABLE ex_t2(id smallint, one varchar(10));"

rem
test_sqlite ex.db "SELECT tbl_name FROM sqlite_master;"
test_sqlite ex.db "SELECT * FROM ex_t1;"
test_sqlite ex.db "SELECT * FROM ex_t2;"
test_sqlite ex.db "SELECT * FROM sqlite_master"
@pause

test_sqlite ex.db "SELECT * FROM ex_t1 WHERE one='hello';"

test_sqlite ex.db "SELECT * FROM ex_t1 WHERE one IN (SELECT one FROM ex_t2);"

test_sqlite ex.db "SELECT COUNT(*) FROM ex_t1;"

echo --- CROSS JOIN
test_sqlite ex.db "SELECT b.id, a.one, a.two FROM ex_t1 a CROSS JOIN ex_t2 b WHERE b.one=a.one"

echo --- INNER JOIN
test_sqlite ex.db "SELECT ex_t2.id, ex_t1.one, ex_t1.two FROM ex_t1 INNER JOIN ex_t2 ON ex_t2.one=ex_t1.one;"

@echo on

