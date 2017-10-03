def split(text:str, sep=None, maxsplit=-1) -> [str]:
    ''' Divide uma string em substrings que são separadas por 'sep' em uma lista de strings onde nenhum elemento está vazio '''
    return list( filter(None, text.split(sep, maxsplit)) )


# ----------- patterns ----------- #
DISCONTINUED = 'discontinued '
PID       = 'Id:'
ASIN      = 'ASIN:'
TITLE     = 'title:'
GROUP     = 'group:'
SALESRANK = 'salesrank:'
SIMILAR   = 'similar:'
CATEGORIES= 'categories:'
REVIEWS   = 'reviews:'
RTOTAL      = 'total:'
RDOWNLOADED = 'downloaded:'
RAVGRATING  = 'avg rating:'

LEN_RTOTAL      = len(RTOTAL)
LEN_RDOWNLOADED = len(RDOWNLOADED)


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
    if text.startswith(PID):
        return int(text[3:])

def search_asin(text:str) -> str:
    ''' Retorna o 'ASIN' do produto '''
    if text.startswith(ASIN):
        return text[5:].lstrip()

def search_title(text:str) -> str:
    ''' Retorna o 'title' do produto '''
    if text.startswith(TITLE):
        return text[6:].lstrip()

def search_group(text:str) -> str:
    ''' Retorna o 'group' do produto '''
    if text.startswith(GROUP):
        return text[6:].lstrip()

def search_salesrank(text:str) -> int:
    ''' Retorna o 'salesrank' do produto '''
    if text.startswith(SALESRANK):
        return int(text[10:])

def search_similar(text:str) -> object:
    ''' Retorna um dicionário (similar:int, asins:[str]) representando o campo 'similar' do produto  '''
    if text.startswith(SIMILAR):
        content = text[8:].lstrip()
        space_begin = (content.find(' ') or 0) + 1

        qtd_similar = 0
        smilars = ''

        if space_begin > 0:
            qtd_similar = content[: space_begin]
            smilars = content[space_begin:]

        return {
            'similar': int(qtd_similar),
            'asins'  : split_similars(smilars)
        }

def search_categories(text:str) -> int:
    ''' Retorna a quantidade de categorias do produto '''
    if text.startswith(CATEGORIES):
        return int(text[11:])

def search_reviews(text:str) -> object:
    ''' Retorna um dicionário (downloaded:int, total:int, avgrating:float) representando dados sobre os comentários do produto '''
    if text.startswith(REVIEWS):
        reviews = text[8:]

        total_begin = reviews.find(RTOTAL)
        downloaded_begin = reviews.find(RDOWNLOADED, total_begin + LEN_RTOTAL)
        avgrating_begin = reviews.find(RAVGRATING, downloaded_begin + LEN_RDOWNLOADED)

        total = reviews[total_begin+6 : downloaded_begin]
        downloaded = reviews[downloaded_begin+12 : avgrating_begin]
        avgrating = reviews[avgrating_begin+11:]

        return {
            'downloaded': int(downloaded),
            'total'     : int(total),
            'avgrating' : float(avgrating)
        }


def get_category(text:str) -> object:
    ''' Retorna um dicionário (cname:str, cid:int) representando uma categoria do produto '''
    reverse = text[::-1]
    cid = reverse[reverse.find(']')+1 : reverse.find('[')]
    cname = reverse[reverse.find('[')+1:]

    cid = cid[::-1]
    cname = cname[::-1]

    return {
        'cname': cname,
        'cid'  : int( cid )
    }

def get_review(text:str) -> object:
    ''' Retorna um dicionário (date:str, cutomer:str, rating:int, votes:int, helpful:int) representando um comentário de um usuário sobre o produto '''
    cutomer_begin = text.find('cutomer:')
    rating_begin = text.find('rating:')
    votes_begin = text.find('votes:')
    helpful_begin = text.find('helpful:')

    date = text[0:cutomer_begin]
    cutomer = text[cutomer_begin+8 : rating_begin]
    rating = text[rating_begin+7 : votes_begin]
    votes = text[votes_begin+6 : helpful_begin]
    helpful = text[helpful_begin+8:]

    return {
        'date'   : date.strip(),
        'cutomer': cutomer.strip(),
        'rating' : int( rating ),
        'votes'  : int( votes ),
        'helpful': int( helpful )
    }
