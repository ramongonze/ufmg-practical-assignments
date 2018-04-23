# tp1-collaborative-filtering
Programming Assignment 1 of the subject Recommender Systems from UFMG.

This programming assignment has the goal to yield a recommender system for movies using collaborative filtering. For the beginning, either model-based and memory-based approaches will be explored. Concepts like data normalization, similarity computation, neighborhood selection, rating aggregation, and dimensionality reduction will be tested and the best model is going to be selected.

# Finalized (15/04/2018)
The approach item-based was used to predict ratings. To calculate the similarities between two movies, it was used the cossine similarity. To normalize the ratings, the mean-centering, and it was also used the neighborhood selection. And to evaluate the algorithm, it was used the Root Mean Squared Error (RMSE).

The description of all used methods are available in the *Documentation.pdf* file.
