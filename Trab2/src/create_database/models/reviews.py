#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi and Victor Meireles on 2017-10-03
#


class Review:
    """ """
    def __init__(self, asin:str, time:str=None, cutomer:str=None, rating:int=0, votes:int=0, helpful:int=0):
        self.asin = asin
        self.time = time
        self.cutomer = cutomer
        self.rating = rating
        self.votes = votes
        self.helpful = helpful

    def __str__(self):
        return "ASIN:'{asin}', time:'{time}', cutomer:'{cutomer}', rating:{rating}, votes:{votes}, helpful:{helpful}".format(**self.__dict__)

    __repr__ = __str__


class ReviewDAO:
    """ """
    def __init__(self, dbconnection):
        self.dbconnection = dbconnection
