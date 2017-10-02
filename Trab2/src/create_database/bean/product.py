from .product_details import ProductDetails

class Product:
    def __init__(self, id:int, asin:str, details:ProductDetails = None):
        self.id = id
        self.asin = asin
        self.details = details

    def __str__(self):
        return "Id:{self.id}, ASIN:{self.asin}, ".format(self=self) + self.details.__str__()

    __repr__ = __str__
