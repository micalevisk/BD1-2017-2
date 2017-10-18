# TODO: conectar com a RAM e: criar tabela 'products', povoar e listar
# https://sqlite.org/lang_createtable.html
# "CREATE TABLE products (maker text, model number, type varchar(7))"
# "INSERT INTO products VALUES ('A', 10001, 'pc')"
# "INSERT INTO products VALUES ('B', 10321, 'laptop')"

# (c) https://docs.python.org/3/library/sqlite3.html
# A minimal SQLite shell for experiments

import sqlite3

# con = sqlite3.connect(":memory:")
con = sqlite3.connect("testeBD.db")
con.isolation_level = None
cur = con.cursor()

buffer = ""

print("Enter your SQL commands to execute in sqlite3.")
print("Enter a blank line to exit.")

while True:
    line = input()
    if line == "":
        break
    buffer += line
    if sqlite3.complete_statement(buffer):
        try:
            buffer = buffer.strip()
            cur.execute(buffer)

            if buffer.lstrip().upper().startswith("SELECT"):
                print(cur.fetchall())
        except sqlite3.Error as e:
            print("An error occurred:", e.args[0])
        buffer = ""

con.close()
