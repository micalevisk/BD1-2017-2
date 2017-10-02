# python3 -B createDatabase.py -i ../__tests__/amazon-meta_little.txt

import getopt, sys

from create_database import *
from create_database.parser import *

## ----------------------------------------------- ##
databaseConfigsDict = {
    'root' :"postgres",
    'name' :"db_micaellevi_victorroque",
    'host' :"localhost",
    'port' :"5432",
    'user' :"bd1",
    'pwd'  :"bd1",
    'dfile':None
}
## ----------------------------------------------- ##


USAGE_TEXT="""\
usage: createDatabase.py [-h] -i <path/to/input.txt> -u <USERNAME> -p <USER_PASSWORD>

optional arguments:
-h, --help      Show this help message and exit

required arguments:
-i, --input     Define input file
-u, --user      Username
-p, --pwd       User password\
"""


def showUsageAndExit():
    sys.exit(USAGE_TEXT)

def setConfigsFromArgs():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hi:u:p:d:", ["input=", "help", "user=", "pwd="])
    except getopt.GetoptError as err:
        sys.exit(err)

    for o, a in opts:
        if o in ("-h", "--help"): showUsageAndExit()
        elif o in ("-i", "--input"): databaseConfigsDict["dfile"] = a
        elif o in ("-u", "--user"): databaseConfigsDict["user"] = a
        elif o in ("-p", "--pwd"): databaseConfigsDict["pwd"] = a
        else:
            assert False, "unhandled option"

    if None in databaseConfigsDict.values(): showUsageAndExit()
    # print( "file={dfile}\nuser={user}\npwd={pwd}".format(**databaseConfigsDict) )


def main(dbexists=False):
    if not dbexists:
        ## iniciar conexão temporária para criar um novo database
        con = DBConnection(databaseConfigsDict["host"], databaseConfigsDict["root"], databaseConfigsDict["user"], databaseConfigsDict["pwd"], True)
        con.fechar() if con.manipular("CREATE DATABASE %s" % databaseConfigsDict["name"]) else sys.exit()

    ## iniciar conexão real utilizando o database criado (acima)
    con = DBConnection(databaseConfigsDict["host"], databaseConfigsDict["name"], databaseConfigsDict["user"], databaseConfigsDict["pwd"], False)

    parse(databaseConfigsDict["dfile"], con)

    con.fechar()


if __name__ == "__main__":
    setConfigsFromArgs()
    main(True)
