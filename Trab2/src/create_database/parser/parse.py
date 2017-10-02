from ..dao import * # contém o bean
from .utils import *
import sys


def normalizeText(text:str) -> str:
    return text.lstrip().rstrip()


def getNormalizedNextLine() -> str:
    line = next(lines_arr)
    return normalizeText(line)


def parse(filepath:str, dbconnection:object):
    try:
        fileDescriptor = open(filepath, 'r', encoding='utf-8', errors='ignore')
    except IOError as ioex:
        sys.exit(ioex)

    global lines_arr
    lines_arr = iter( fileDescriptor.readlines() )

    for line in lines_arr:
        lineNormalized = normalizeText(line)

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
                for i in range(reviewsInfo['downloaded']):
                    currProductDetails.reviews.insert(i, get_review( getNormalizedNextLine() ))

            ## debug only
            try:
                print(currProduct)
            except UnicodeEncodeError as err:
                pass

    fileDescriptor.close()
