# (c) https://github.com/reisneto/amazon_productdata_extractor
# prepare data to be used in edmonds-karp algorithm. Data extracted from Amazon product co-purchasing network metadata.

import re
import math
from collections import defaultdict
from datetime import datetime

#-------------- REGEX ---------------
PID = r'Id:\s*(\d+)'
ASIN = r'ASIN:\s*\d+'
CID = r'cutomer:\s*(\w+)'
DISC = r'discontinued product'
CSIZE = r'total:\s*(\d+)'
#-------------- ---------------------
MAX_CLIENTS = 5486
total_capacity = 0
#pid_temp
amazon_data = defaultdict(list) #client has many products



f = open('../amazon-meta_reduzido.txt', 'r')
flog = open('log.txt', 'w')
fgraph = open('graph.in', 'w')


stime = datetime.now()
searchProduct = True
reviews = 0
product_reviews = {}
total_reviews = 0
for line in f:
	#Esta procurando ou por produto ou por cliente
	if searchProduct:
		pid_temp = re.search(PID, line)
		if pid_temp:
			searchProduct = False #Assim que encontra produto, eh hora de encontrar cliente
			index = int(pid_temp.group(1))
	else:
		pid_temp2 = re.search(PID, line)
		if pid_temp2:
			 pid_temp = pid_temp2 #discontinued product case. o produto nao tinha review portando pulou para o proximo produto
			 index = int(pid_temp.group(1))

		review = re.search(CSIZE,line)
		if review:
			reviews = int(review.group(1))
			if reviews > 0:
				product_reviews[pid_temp.group(1)] = reviews
				total_reviews += reviews

		cid = re.search(CID, line)
		if cid:
			#print pid_temp.group(1),cid.group(1)
			if not pid_temp.group(1) in amazon_data[cid.group(1)]: #do not repeat items on list
				amazon_data[cid.group(1)].append(pid_temp.group(1))
			if len(amazon_data) > MAX_CLIENTS:
				index = int(pid_temp.group(1))
				break


#index = 548551 #last product_id is 548551
#1% is about 5486 clients
#index = 200

#source and sink
fgraph.write('%s %d\n' % (0,index+len(amazon_data) +1))

for k,v in amazon_data.iteritems():
	index += 1
	for p in v:
		fgraph.write('%s %s %d\n' % (index,p,1))
	#40% of client reviews
	capacity = math.ceil(0.4 * len(v))
	fgraph.write('%s %s %d\n' % (0,index,capacity))
	total_capacity += capacity


#60% product reviews
for k,v in product_reviews.iteritems():
	fgraph.write('%s %s %d\n' % (k,index+1,v))


flog.write("start time: %s\n" % stime)
flog.write("end time: %s\n" % datetime.now())
flog.write("number of users: %d\n" % index)
flog.write("number of products: %d\n" % len(product_reviews))
flog.write("total of reviews: %d\n" % total_reviews)
flog.write("total capacity: %d\n" % total_capacity)

flog.close()
f.close()
fgraph.close()
