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
