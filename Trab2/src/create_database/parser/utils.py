from re import search


def split(text:str, sep=None, maxsplit=-1) -> [str]:
    ''' Divide uma string em substrings que são separadas por 'sep' em uma lista de strings onde nenhum elemento está vazio '''
    return list( filter(None, text.split(sep, maxsplit)) )


DISCONTINUED = 'discontinued '

# --------------- regexs --------------- #
PID       = r'^Id:\s*(\d+)$'        ## [1]:valor
ASIN      = r'^ASIN:\s*(\w+)$'      ## [1]:valor
TITLE     = r'^title:\s*(.+)$'      ## [1]:valor
GROUP     = r'^group:\s*(\w+)$'     ## [1]:valor
SALESRANK = r'^salesrank:\s*(\d+)$' ## [1]:valor
SIMILAR   = r'^similar:\s*(?P<count>\d+)(?:\s+(?P<asins>.+))?' ## [count]:quantidade de items em [asins]:ASINs de outros produtos, separados por espaço (opcional)
CATEGORIES= r'^categories:\s*(\d+)$' ## [1]:quantidade de categorias
CATEGORY  = r'(?P<cname>.+)?\[\s*(?P<cid>\d+)\s*\]' ## [cname]:nome da categoria (opcional) [cid]:id da categoria
REVIEWS   = r'^reviews:\s*total:\s*(?P<total>\d+)\s+downloaded:\s*(?P<count>\d+)\s*avg\s+rating:\s*(?P<avgrating>\d+)$' ## [total]:quantidade de comentários [count]:quantidade de comentários listados no dump [avgrating]:média dos votos
REVIEW    = r'^(?P<date>.+)\s+cutomer:\s*(?P<uid>\w+)\s+rating:\s*(?P<rating>\d+)\s+votes:\s*(?P<votes>\d+)\s+helpful:\s*(?P<helpful>\d+)$' ## [date]:dia da publicação [uid]:user id [rating]:avaliação do usuário [votes]:número de votos do usuário [helpful]:número de votos de utilidade


def split_similars(text:str) -> [str]:
    ''' Retorna uma lista de substrings ou None '''
    if text:
        return split(text, ' ')

def split_categories(text:str) -> [str]:
    ''' Retorna uma lista de substrings ou None '''
    if text:
        return split(text, '|')

def discontinuedProduct(text:str) -> bool:
    return text.startswith(DISCONTINUED)

def search_id(text:str) -> int:
    ''' Retorna o 'Id' do produto '''
    match = search(PID, text)
    if match:
        return int( match.group(1) )
    # return int( match.group(1) ) if match else None

def search_asin(text:str) -> str:
    ''' Retorna o 'ASIN' do produto '''
    match = search(ASIN, text)
    if match:
        return match.group(1)

def search_title(text:str) -> str:
    ''' Retorna o 'title' do produto '''
    match = search(TITLE, text)
    if match:
        return match.group(1)
    # return match.group(1) if match else None

def search_group(text:str) -> str:
    ''' Retorna o 'group' do produto '''
    match = search(GROUP, text)
    if match:
        return match.group(1)
    # return match.group(1) if match else None

def search_salesrank(text:str) -> int:
    ''' Retorna o 'salesrank' do produto '''
    match = search(SALESRANK, text)
    if match:
        return int( match.group(1) )
    # return int( match.group(1) ) if match else None

def search_similar(text:str) -> object:
    ''' Retorna um dicionário (count:int, asins:[str]) representando o campo 'similar' do produto  '''
    match = search(SIMILAR, text)
    if match:
        return {
            'count': int( match.group('count') ),
            'asins': split_similars( match.group('asins') )
        }

def search_categories(text:str) -> int:
    ''' Retorna a quantidade de categorias do produto '''
    match = search(CATEGORIES, text)
    if match:
        return int( match.group(1) )
    # return int( match.group(1) ) if match else None

def search_reviews(text:str) -> object:
    ''' Retorna um dicionário (count:int, total:int, avgrating:float) representando dados sobre os comentários do produto '''
    match = search(REVIEWS, text)
    if match:
        return {
            'count'    : int( match.group('count') ),
            'total'    : int( match.group('total') ),
            'avgrating': float( match.group('avgrating') )
        }


def get_category(text:str) -> object:
    ''' Retorna um dicionário (cname:str, cid:int) representando uma categoria do produto '''
    match = search(CATEGORY, text)
    if match:
        return {
            'cname': match.group('cname') or '',
            'cid'  : int( match.group('cid') )
        }

def get_review(text:str) -> object:
    ''' Retorna um dicionário (date:str, uid:str, rating:int, votes:int, helpful:int) representando um comentário de um usuário sobre o produto '''
    match = search(REVIEW, text)
    if match:
        return {
            'date': match.group('date'),
            'uid' : match.group('uid'),
            'rating': int( match.group('rating') ),
            'votes': int( match.group('votes')),
            'helpful': int( match.group('helpful'))
        }
