#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

import getopt, sys
from create_database import *


## ----------------------------------------------- ##
databaseConfigsDict = {
    'name' :None,
    'host' :"localhost",
    'user' :None,
    'pwd'  :None,
    'dfile':None
}
## ----------------------------------------------- ##


USAGE_TEXT="""\
Irá criar um Banco de Dados (local) relacional com o nome passado.
usage: createDatabase.py [-h] -i <path/to/input.txt> -d <DATABASE_NAME> -u <USERNAME> -p <USER_PASSWORD> [-r]

optional arguments:
-h, --help      Show this help message and exit
-d, --database  Define database's name to connect
-r, --reset     Reset all, ie., drop (delete) schema "public" and create it (exclusive option)

required arguments:
-i, --input     Define input file
-u, --user      Username
-p, --pwd       User password\
"""


def showHelpAndExit():
    sys.exit(USAGE_TEXT)

def setConfigsFromArgs():
    global dropAndCreateSchema
    dropAndCreateSchema = False

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hi:u:p:d:r", ["database=", "input=", "help", "user=", "pwd="])
    except getopt.GetoptError as err:
        sys.exit(err)

    for o, a in opts:
        if o in ("-h", "--help"): showHelpAndExit()
        elif o in ("-d", "--database"): databaseConfigsDict["name"] = a
        elif o in ("-i", "--input"): databaseConfigsDict["dfile"] = a
        elif o in ("-u", "--user"): databaseConfigsDict["user"] = a
        elif o in ("-p", "--pwd"): databaseConfigsDict["pwd"] = a
        elif o in ("-r", "--reset"): dropAndCreateSchema = True
        else:
            assert False, "unhandled option"

    if None in databaseConfigsDict.values(): showHelpAndExit()


def main():
    ## iniciar conexão
    conn = DBConnection(databaseConfigsDict["host"],
                        databaseConfigsDict["name"],
                        databaseConfigsDict["user"],
                        databaseConfigsDict["pwd"])

    if dropAndCreateSchema:
        conn.manipular('DROP SCHEMA public CASCADE; CREATE SCHEMA public', None, True, True)
    else:
        ## realizar parser do arquivo e popular o banco
        parse(databaseConfigsDict["dfile"], conn)

    ## encerrar conexão com o banco
    conn.fechar()


if __name__ == "__main__":
    setConfigsFromArgs()
    main()
