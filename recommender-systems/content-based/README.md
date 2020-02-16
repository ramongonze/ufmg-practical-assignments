# tp2-content-based
Programming Assignment 2 of the subject Recommender Systems from UFMG. 

This practical work has the goal to yield a movie recommender system using the content-based approach. Details about the practical work's description are available in *PA-Description.pdf*.

## RapidJSON
The RapidJSON is a library to treat JSON files. The dataset of movies features, used in this work, is in JSON format. 

It is available on https://github.com/Tencent/rapidjson

Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.

## Finished (12/05/2018)

Best score reached (RMSE): 1.71746. The content used from the movies were the genres, actors, directors and writers, the country of production and the IMDb Rating. It was used a function similar to cosine to calculate the similarity between two binary vectors. And to calculate the similarity between to plots, it was used Rocchio recommendation (to build the user's vector). 

The description of all used methods are available in the *Documentation.pdf* file.
