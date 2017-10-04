from ..models import *
from .utils import *
import sys


def normalizeText(text:str) -> str:
    """ Para padronizar um texto para o parse """
    return text.strip()


def getNormalizedNextLine() -> str:
    """ """
    line = next(lines_arr)
    return normalizeText(line)


def parse(filepath:str, dbconnection:object):
    """ """
    try:
        fileDescriptor = open(filepath, 'r', encoding='utf-8', errors='ignore')
    except IOError as ioex:
        sys.exit(ioex)

    global lines_arr
    lines_arr = iter( fileDescriptor.readlines() )
    fileDescriptor.close()

    productDAO = ProductDAO(dbconnection)
    similarDAO = SimilarDAO(dbconnection)
    categoryDAO = CategoryDAO(dbconnection)
    productCategoryDAO = ProductCategoryDAO(dbconnection)
    reviewDAO = ReviewDAO(dbconnection)

    ## criar todas as tabelas
    productDAO.createTable()
    similarDAO.createTable()
    categoryDAO.createTable()
    productCategoryDAO.createTable()
    reviewDAO.createTable(True) # efetivar alterações fisicamente

    productCounter = 0

    for line in lines_arr:
        lineNormalized = normalizeText(line)
        if not len(lineNormalized): continue

        productCounter += 1
        currProduct = Product(None, None);

        ## definir Id
        pid = search_id(lineNormalized)
        if pid == None: continue
        currProduct.id = pid

        ## definir ASIN
        currProduct.asin = search_asin( getNormalizedNextLine() )

        nextLine = getNormalizedNextLine()
        if discontinuedProduct(nextLine):
            productDAO.insert(currProduct)
            continue

        ## definir title
        currProduct.title = search_title(nextLine)

        ## definir group
        currProduct.name_group = search_group( getNormalizedNextLine() )

        ## definir salesrank
        currProduct.salesrank = search_salesrank( getNormalizedNextLine() )

        ## definir similar
        currSimilar_dict = search_similar( getNormalizedNextLine() )
        currProduct.count_similars = currSimilar_dict["similar"]
        currSimilars_list = []; # lista de Similar
        for i, similar_str in enumerate(currSimilar_dict["asins"]):
            currSimilars_list.insert( i, Similar(currProduct.asin, similar_str) )

        ## definir categories
        qtdCategories = search_categories( getNormalizedNextLine() )
        currProduct.count_categories = qtdCategories
        currCategories_list = []; # todas as linhas de categorias (elementos Category)
        currLeafsCategories_list = []; # todas as últimas qtdCategories de cada linha (elementos ProductCategory)
        if qtdCategories > 0:
            for i in range(qtdCategories):
                last_cid = None
                currCategories_strlist = split_categories( getNormalizedNextLine() )
                for j, category_str in enumerate(currCategories_strlist): ## separar as subcategorias
                    currCategory = get_category(category_str)
                    currCategories_list.insert( j+i, Category(currCategory["cid"], currCategory["cname"], last_cid) )
                    last_cid = currCategory["cid"]
                currLeafsCategories_list.insert(i, ProductCategory(currProduct.asin, last_cid))

        ## definir reviews
        reviewsInfo = search_reviews( getNormalizedNextLine() )
        currReviews_list = []; # todas as linhas de reviews (elementos Review)
        if reviewsInfo:
            currProduct.count_total_reviews = reviewsInfo["downloaded"]

            for i in range(currProduct.count_total_reviews):
                currReview = get_review( getNormalizedNextLine() )
                currReviews_list.insert(i, Review(currProduct.asin, currReview["time"], currReview["cutomer"], currReview["rating"], currReview["votes"], currReview["helpful"]) )


        ## inserir na tabela 'products'
        productDAO.insert(currProduct)

        ## inserir na tabela 'similars'
        for similar in currSimilars_list:
            similarDAO.insert(similar)

        ## inserir na tabela 'categories'
        for category in currCategories_list:
            categoryDAO.insert(category)

        ## inserir na tabela 'product_categories'
        for pcategory in currLeafsCategories_list:
            productCategoryDAO.insert(pcategory)

        ## inserir na tabela 'reviews'
        for review in currReviews_list:
            reviewDAO.insert(review)

        ## realizar commit a cada 500 produtos (válidos) inseridos
        if not productCounter % 500: dbconnection.doCommit()

    ## adicionando as restrições
    productDAO.addConstraints()
    similarDAO.addConstraints()
    categoryDAO.addConstraints()
    productCategoryDAO.addConstraints()
    reviewDAO.addConstraints(True) # realizar commit
