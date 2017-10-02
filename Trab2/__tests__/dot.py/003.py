##:description: tentativa de parser simples
##:run: python3 -B 003.py '../amazon-meta_reduzido.txt'

## Para apagar o arquivo criado (caso a flag 'B' não seja utilizada)
## find '__pycache__' -exec rm -rf {} \;

import sys

sys.path.insert(0, '../../src/model/parser')
from parser_utils import *



#===================
class ProductDetails:
    def __init__(self, title:str, group:str = None, salesrank:int = None, similiar:[str] = None, categories:[object] = None, reviewsinfo:object = None, reviews:[object] = None):
        self.title = title
        self.group = group
        self.salesrank = salesrank
        self.similiar = similiar
        self.categories = categories
        self.reviewsinfo = reviewsinfo
        self.reviews = reviews

    def __str__(self):
        return "title:\"{self.title}\"".format(self=self)
        # return "group:{self.group}, salesrank:{self.salesrank}, reviewsinfo:{self.reviewsinfo}, reviews:{self.reviews}".format(self=self)

    __repr__ = __str__

#============
class Product:
    def __init__(self, id:int, asin:str, details:ProductDetails = None):
        self.id = id
        self.asin = asin
        self.details = details

    def __del__(self):
        class_name = self.__class__.__name__
        # print("instance of", class_name, "has been destroyed")

    def __str__(self):
        return "Id:{self.id}, ASIN:{self.asin}, ".format(self=self) + self.details.__str__()

    __repr__ = __str__




def normalizeText(text:str) -> str:
    return text.lstrip().rstrip()


def getNormalizedNextLine() -> str:
    line = next(lines_arr)
    return normalizeText(line)

#__________
def main():

    try:
        fileDescriptor = open(FILENAME_PATH, 'r', encoding='utf-8', errors='ignore')
    except IOError as ioex:
        sys.exit(ioex)

    global lines_arr
    lines_arr = iter( fileDescriptor.readlines() )

    # for lineIndex in range( len(lines_arr) ):
    # for line in lines_arr:
    for line in lines_arr:
        lineNormalized = normalizeText(line)
        # encoded = lineNormalized.encode("utf-8")

        if len(lineNormalized):
            currProduct = Product(None, None)
            currProductDetails = ProductDetails(None)

            ## definir Id
            pid = search_id(lineNormalized)
            if pid == None: continue
            currProduct.id = pid

            ## definir ASIN
            currProduct.asin = search_asin( getNormalizedNextLine() )

            nextLine = getNormalizedNextLine()
            if discontinuedProduct(nextLine): print(currProduct); continue
            currProduct.details = currProductDetails

            ## definir title
            currProductDetails.title = search_title(nextLine)

            ## definir group
            currProductDetails.group = search_group( getNormalizedNextLine() )

            ## definir salesrank
            currProductDetails.salesrank = search_salesrank( getNormalizedNextLine() )

            ## definir similar
            currProductDetails.similar = search_similar( getNormalizedNextLine() )

            ## definir categories
            qtdCategories = search_categories( getNormalizedNextLine() )
            if qtdCategories > 0:
                currProductDetails.categories = []

                for i in range(qtdCategories):
                    currCategories_list = []
                    currProductDetails.categories.insert(i, currCategories_list)

                    currCategories_strlist = split_categories( getNormalizedNextLine() )
                    for j, categorie_str in enumerate(currCategories_strlist): ## separar as subcategorias
                        currCategories_list.insert(j, get_category(categorie_str))

            ## definir reviews
            reviewsInfo = search_reviews( getNormalizedNextLine() )
            if reviewsInfo:
                currProductDetails.reviews = []
                currProductDetails.reviewsinfo = {}

                currProductDetails.reviewsinfo.update(reviewsInfo)
                for i in range(reviewsInfo['count']):
                    currProductDetails.reviews.insert(i, get_review( getNormalizedNextLine() ))


            ## debug only
            try:
                print(currProduct)
            except UnicodeEncodeError as err:
                pass

    fileDescriptor.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("[ERROR] precisa passar apenas o caminho para o arquivo.")

    [ FILENAME_PATH ] = sys.argv[1:]
    print("------- início -------")
    main()
    print("------- fim -------")



"""
refs:

https://docs.python.org/3/library/typing.html
https://www.python-course.eu/python3_properties.php

https://www.python-course.eu/python3_functions.php
https://docs.python.org/3/howto/regex.html
https://docs.python.org/3/library/functions.html

"""
