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
This is the main entry point for Part 2 of this MP. You should only modify code
within this file for Part 2 -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""


import numpy as numpy
import math
from collections import Counter


def naiveBayesMixture(train_set, train_labels, dev_set, bigram_lambda, unigram_smoothing_parameter, bigram_smoothing_parameter, pos_prior):
    """
    train_set - List of list of words corresponding with each movie review
    example: suppose I had two reviews 'like this movie' and 'i fall asleep' in my training set
    Then train_set := [['like','this','movie'], ['i','fall','asleep']]

    train_labels - List of labels corresponding with train_set
    example: Suppose I had two reviews, first one was positive and second one was negative.
    Then train_labels := [1, 0]

    dev_set - List of list of words corresponding with each review that we are testing on
              It follows the same format as train_set

    bigram_lambda - float between 0 and 1

    unigram_smoothing_parameter - Laplace smoothing parameter for unigram model (between 0 and 1)

    bigram_smoothing_parameter - Laplace smoothing parameter for bigram model (between 0 and 1)

    pos_prior - positive prior probability (between 0 and 1)
    """

    uni_pos_counter = Counter()
    uni_neg_counter = Counter()

    for i in range(len(train_set)):
        curr_review = train_set[i]
        for word in curr_review:
            if train_labels[i] == 1:
                uni_pos_counter[word] += 1
            else:
                uni_neg_counter[word] += 1

    uni_total_pos_words = sum(uni_pos_counter.values())
    uni_total_neg_words = sum(uni_neg_counter.values())
    uni_unique_pos_words = len(uni_pos_counter.keys())
    uni_unique_neg_words = len(uni_neg_counter.keys())

    bi_pos_counter = Counter()
    bi_neg_counter = Counter()

    for i in range(len(train_set)):
        curr_review = train_set[i]
        for idx in range(len(curr_review) - 1):
            bigram = curr_review[idx] + curr_review[idx + 1]
            if train_labels[i] == 1:
                bi_pos_counter[bigram] += 1
            else:
                bi_neg_counter[bigram] += 1

    bi_total_pos_words = sum(bi_pos_counter.values())
    bi_total_neg_words = sum(bi_neg_counter.values())
    bi_unique_pos_words = len(bi_pos_counter.keys())
    bi_unique_neg_words = len(bi_neg_counter.keys())

    dev_labels = []

    for i in range(len(dev_set)):
        curr_review = dev_set[i]
        uni_pos_product = 0
        uni_neg_product = 0
        for word in curr_review:
            uni_pos_product += math.log(smoothing(
                uni_pos_counter[word], unigram_smoothing_parameter, uni_total_pos_words, uni_unique_pos_words))
            uni_neg_product += math.log(smoothing(
                uni_neg_counter[word], unigram_smoothing_parameter, uni_total_neg_words, uni_unique_neg_words))

        bi_pos_product = 0
        bi_neg_product = 0
        for idx in range(len(curr_review) - 1):
            bigram = curr_review[idx] + curr_review[idx + 1]
            bi_pos_product += math.log(smoothing(
                bi_pos_counter[bigram], bigram_smoothing_parameter, bi_total_pos_words, bi_unique_pos_words))
            bi_neg_product += math.log(smoothing(
                bi_neg_counter[bigram], bigram_smoothing_parameter, bi_total_neg_words, bi_unique_neg_words))

        p_pos = ((1 - bigram_lambda) * (math.log(pos_prior) + uni_pos_product)
                 ) + (bigram_lambda * (math.log(pos_prior) + bi_pos_product))
        p_neg = ((1 - bigram_lambda) * (math.log(1 - pos_prior) + uni_neg_product)
                 ) + (bigram_lambda * (math.log(1 - pos_prior) + bi_neg_product))

        if p_pos > p_neg:
            dev_labels.append(1)
        else:
            dev_labels.append(0)

    # return predicted labels of development set (make sure it's a list, not a numpy array or similar)
    return dev_labels


def smoothing(count, k, total_words, unique_words):
    return (count + k) / (total_words + k * unique_words)