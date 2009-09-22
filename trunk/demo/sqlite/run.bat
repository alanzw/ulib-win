@echo off

rem Create a new database
test_sqlite ex.db "select tbl_name from sqlite_master;"
test_sqlite ex.db "select * from ex_t1;"
test_sqlite ex.db "select * from sqlite_master"
@pause

@echo on

