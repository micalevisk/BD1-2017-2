> from https://www.postgresql.org/docs/9.1/static/ecpg-cpp.html
> doc libpq https://www.postgresql.org/docs/8.3/static/libpq.html

# Para compilar
```bash
ecpg -o test_mod.c test_mod.pgc
cc -c test_mod.c -o test_mod.o #  -I "/cygdrive/c/Program Files/PostgreSQL/9.6/include"

c++ -c test_cpp.cpp -o test_cpp.o
c++ test_cpp.o test_mod.o -lecpg -o test_cpp
```
