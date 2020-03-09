# naive_bayes.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Justin Lizama (jlizama2@illinois.edu) on 09/28/2018
# Modified by Jaewook Yeom 02/02/2020

"""
This is the main entry point for Part 1 of MP3. You should only modify code
within this file for Part 1 -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""

import numpy as numpy
import math
from collections import Counter


def naiveBayes(train_set, train_labels, dev_set, smoothing_parameter, pos_prior):
    """
    train_set - List of list of words corresponding with each movie review
    example: suppose I had two reviews 'like this movie' and 'i fall asleep' in my training set
    Then train_set := [['like','this','movie'], ['i','fall','asleep']]

    train_labels - List of labels corresponding with train_set
    example: Suppose I had two reviews, first one was positive and second one was negative.
    Then train_labels := [1, 0]

    dev_set - List of list of words corresponding with each review that we are testing on
              It follows the same format as train_set

    smoothing_parameter - The smoothing parameter you provided with --laplace (1.0 by default)

    pos_prior - positive prior probability (between 0 and 1)
    """

    pos_counter = Counter()
    neg_counter = Counter()

    for i in range(len(train_set)):
        curr_review = train_set[i]
        for word in curr_review:
            if train_labels[i] == 1:
                pos_counter[word] += 1
            else:
                neg_counter[word] += 1

    total_pos_words = sum(pos_counter.values())
    total_neg_words = sum(neg_counter.values())
    unique_pos_words = len(pos_counter.keys())
    unique_neg_words = len(neg_counter.keys())

    dev_labels = []

    for i in range(len(dev_set)):
        curr_review = dev_set[i]
        pos_product = 0
        neg_product = 0
        for word in curr_review:
            pos_product += math.log(smoothing(
                pos_counter[word], smoothing_parameter, total_pos_words, unique_pos_words))
            neg_product += math.log(smoothing(
                neg_counter[word], smoothing_parameter, total_neg_words, unique_neg_words))

        p_pos = math.log(pos_prior) + pos_product
        p_neg = math.log(1 - pos_prior) + neg_product

        if p_pos > p_neg:
            dev_labels.append(1)
        else:
            dev_labels.append(0)

    # return predicted labels of development set (make sure it's a list, not a numpy array or similar)
    return dev_labels


def smoothing(count, k, total_words, unique_words):
    return (count + k) / (total_words + k * unique_words)
