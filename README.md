# pg_injection

### [Use of the extension](#use-of-the-extension)

In all session where you want to use pg_injection you will have to load the extension using:
```sql
LOAD 'pg_injection.so';
```
Also, You may set error level (default is `WARNING`)
```sql
SET pg_injection.level = ERROR;
```
If you want to generalize the use of the extension modify your postgresql.conf to set
```conf
session_preload_libraries = 'pg_injection'
```
or even
```conf
shared_preload_libraries = 'pg_injection'
```
and add
```conf
pg_injection.level = ERROR
```
at end of the file.
