#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi and Victor Meireles on 2017-10-03
#


class Product:
    """ Representar um registro da relação 'products' """
    def __init__(self, id:int, asin:str, ## required
                title:str=None, name_group:str=None, salesrank:int=None, count_similars:int=0, count_categories:int=0, count_total_reviews:int=0):
        self.id = id
        self.asin = asin
        self.title = title
        self.name_group = name_group
        self.salesrank = salesrank
        self.count_similars = count_similars
        self.count_categories = count_categories
        self.count_total_reviews = count_total_reviews

        # self._similars = [] # lista de Similar
        # self._categories = [] # lista de Category

    '''
    @property
    def similars(self):
        return self._similars

    @similars.setter
    def similars(self, similars_list):
        if similars_list:
            self.count_similars = len(similars_list)
            self._similars = similars_list

    @property
    def categories(self):
        return self._categories

    @categories.setter
    def categories(self, categories_list):
        if categories_list:
            self.count_categories = len(categories_list)
            self._categories = categories_list
    '''

    def __str__(self):
        return "Id:{id}, ASIN:'{asin}', title:'{title}', group:'{name_group}', salesrank:{salesrank}, similar:{count_similars}, categories:{count_categories}, totalreviews:{count_total_reviews}".format(**self.__dict__)

    __repr__ = __str__


class ProductDAO:
    """ Manipular um registro na relação 'products' """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable_products(self):
        return ''
