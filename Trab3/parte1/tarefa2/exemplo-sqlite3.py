# USO: python3 exemplo-sqlite3.py

import sqlite3 # https://docs.python.org/3.6/library/sqlite3.html

try:

    con = sqlite3.connect(':memory:') # usar a RAM como para BD
    print('pysqlite version =', sqlite3.version)
    print('SQLite db lib version =', sqlite3.sqlite_version)

    with con:
        cur = con.cursor()
        cur.execute("ATTACH DATABASE 'firstDB.db' AS db") # criar banco 'firstDB.db' e usá-lo com o alias 'db'

        cur.executescript("""
            DROP TABLE IF EXISTS db.alunos;
            CREATE TABLE IF NOT EXISTS db.alunos (matricula INT PRIMARY KEY, nome TEXT NOT NULL);
            INSERT INTO db.alunos VALUES(21554923, 'micael');
            INSERT INTO db.alunos VALUES(21550188, 'moisés');
        """)

        cur.execute("SELECT * FROM db.alunos")
        rows = cur.fetchall()
        for row in rows:
            print(row)

        cur.execute("DETACH DATABASE db")
        con.commit()

except sqlite3.Error as e:
    if con: con.rollback()
    print('[ERROR]', e)
finally:
    if con: con.close()
