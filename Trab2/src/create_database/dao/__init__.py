# from .product_dao import ProductDAO
# from .product_details_dao import ProductDetailsDAO
# __all__ = ['ProductDAO', 'ProductDetailsDAO']
# __all__ = ['product_dao', 'product_details_dao']

from .product_dao import ProductDAO, Product
from .product_details_dao import ProductDetailsDAO, ProductDetails


__all__ = [ 'Product', 'ProductDetails', 'ProductDAO', 'ProductDetailsDAO']
