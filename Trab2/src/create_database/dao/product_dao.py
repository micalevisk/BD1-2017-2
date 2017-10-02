from ..bean import Product

class ProductDAO:
    def __init__(self, dbconnection:object):
        self._db = dbconnection
        print( Product(1, 'xx') )
