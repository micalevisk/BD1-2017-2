from ..bean import ProductDetails

class ProductDetailsDAO:
    def __init__(self, dbconnection:object):
        self._db = dbconnection
