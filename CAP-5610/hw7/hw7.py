import numpy as np
from surprise import Dataset, Reader, evaluate
from surprise import SVD, print_perf, NMF, KNNBasic 

file_path = "restaurant_ratings.txt"
reader = Reader(line_format='user item rating timestamp', sep='\t')
data = Dataset.load_from_file(file_path, reader=reader)

data.split(n_folds=3)

# print("SVD Performance: ")
# algo = SVD()
# perf = evaluate(algo, data, measures=['RMSE', 'MAE'])
# print_perf(perf)

# print("PMF Performance: ")
# algo = SVD(biased=False)
# perf = evaluate(algo, data, measures=['RMSE', 'MAE'])
# print_perf(perf)

# print("NMF Performance: ")
# algo = NMF()
# perf = evaluate(algo, data, measures=['RMSE', 'MAE'])
# print_perf(perf)

# print("User Based Collaborative Filtering MSD: ")
# algo = KNNBasic(sim_options = {
# 	'name':'MSD',
# 	'user_based':True
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)

# print("User Based Collaborative Filtering Cosine: ")
# algo = KNNBasic(sim_options = {
# 	'name':'cosine',
# 	'user_based':True
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)

# print("User Based Collaborative Filtering Pearson: ")
# algo = KNNBasic(sim_options = {
# 	'name':'pearson',
# 	'user_based':True
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)



# print("Item Based Collaborative Filtering MSD: ")
# algo = KNNBasic(sim_options = {
# 	'name':'MSD',
# 	'user_based':False
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)

# print("Item Based Collaborative Filtering Cosine: ")
# algo = KNNBasic(sim_options = {
# 	'name':'cosine',
# 	'user_based':False
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)

# print("Item Based Collaborative Filtering Pearson: ")
# algo = KNNBasic(sim_options = {
# 	'name':'pearson',
# 	'user_based':False
# 	})
# perf = evaluate(algo, data, measures=['RMSE'])
# print_perf(perf)


def mean(l):
	return sum(l)/len(l)


ub = []
ib = []

for i in range(100):
	# print("User Based Collaborative Filtering MSD: ")
	algo = KNNBasic(k=i, sim_options = {
		'name':'MSD',
		'user_based':True
		})
	perf = evaluate(algo, data, measures=['RMSE'])
	# print_perf(perf)
	m = mean(perf['rmse'])
	ub.append(m)

	# print("Item Based Collaborative Filtering MSD: ")
	algo = KNNBasic(k=i, sim_options = {
		'name':'MSD',
		'user_based':False
		})
	perf = evaluate(algo, data, measures=['RMSE'])
	m = mean(perf['rmse'])
	ib.append(m)
	# print_perf(perf)

print("User-Based Mean: ")
for i in ub:
	print(i)

print("Item-Based Mean: ")
for i in ib:
	print(i)

print("Optimum k for User-based filtering: " + str(ub.index(min(ub))+1))
print("Optimum k for Item-based filtering: " + str(ib.index(min(ib))+1))