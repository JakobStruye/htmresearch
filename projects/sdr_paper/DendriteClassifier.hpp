/* ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2014-2015, Numenta, Inc.  Unless you have an agreement
 * with Numenta, Inc., for a separate license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ----------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------
 * This file runs the MNIST dataset using a simple model composed of a
 * set of dendrites. Each dendrite randomly samples pixels from one image.
 * ----------------------------------------------------------------------
 */

#include <assert.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include <nupic/math/Math.hpp>
#include <nupic/math/SparseMatrix.hpp>
#include <nupic/math/SparseMatrix01.hpp>
#include <nupic/types/Types.hpp>
#include <nupic/utils/Random.hpp>

using namespace std;
using namespace nupic;


class DendriteClassifier {
  public:

    DendriteClassifier(int seed=42, int numClasses=10, int inputSize=784);
    virtual ~DendriteClassifier();

    // Go through all training examples for each class. For each class, create
    // a set of dendrites that randomly sample from that class.
    void trainDataset(int nSynapses,
          std::vector< SparseMatrix01<UInt, Int> * > &trainingSet);

    // Classify the dataset using a trained dendrite model and the
    // given threshold, and report accuracy
    void classifyDataset(int threshold,
               std::vector< SparseMatrix01<UInt, Int> * > &dataSet);


    //////////////////////////////////////////////////////
    //
    // Classification scheme 1

    // Go through all training examples of class k. Create a set of dendrites
    // that randomly sample for that class.
    void trainClass(int k, int nSynapses,
           std::vector< SparseMatrix01<UInt, Int> * > &trainingSet);


    int runInferenceOnPattern(int row, int threshold,
               SparseMatrix01<UInt, Int> *dataSet,
               SparseMatrix01<UInt, Int> *dendrites);


    int classifyPattern(int row, int threshold,
               SparseMatrix01<UInt, Int> *dataSet);


    //////////////////////////////////////////////////////
    //
    // Internal variables and methods.
    // Leaving as public for ease of debugging.
    int numClasses_;
    Random rng_;
    std::vector< SparseMatrix01<UInt, Int> * > dendrites_;

    template <typename ChoicesIter>  void sample(SparseMatrix01<UInt, Int> *sm,
          UInt32 row, ChoicesIter choices, UInt32 nChoices);

     // Clear memory
     void deleteDendrites_();


    //////////////////////////////////////////////////////
    //
    // Classification scheme 2
    //
    // A second KNN like classifier using dendrites:
    //
    // Step 1. Choose nPrototypesPerClass_ random training examples from class k
    // with replacement. For each example, create a dendrite that randomly
    // samples from that image.
    //
    // Step 2. Then run each image in training set through the dendrites and
    // record the vector of dendrites that are produced along with their
    // category. The set of dendrites plus the stored patterns comprise the
    // trained model.
    //
    // Step 3. For each test image, run the image through the dendrites, check
    // the set of active dendrites against the stored set by doing a dot
    // product. Choose category corresponding to highest dot product.

    // Go through all training examples for each class. For each class, create
    // a set of dendrites that randomly sample from that class.
    void trainDatasetKNN(int nSynapses, int threshold,
          std::vector< SparseMatrix01<UInt, Int> * > &trainingSet);


    // Classify the dataset using a trained dendrite model and the
    // given threshold, and report accuracy
    void classifyDatasetKNN(int threshold,
               std::vector< SparseMatrix01<UInt, Int> * > &dataSet);



    void createRandomlySampledDendrites(int k, int nSynapses,
           std::vector< SparseMatrix01<UInt, Int> * > &trainingSet);


    void trainKNN(int threshold,
               std::vector< SparseMatrix01<UInt, Int> * > &trainingSet);


    // Given the p'th pattern in the dataSet, run inference using all the
    // dendrites and the given threshold. inferenceNonZeros will contain the
    // concatenated list of the responses from all the dendrites.
    void inferenceForKNN(int p, int threshold,
            SparseMatrix01<UInt, Int> *dataSet,
            vector<UInt> &inferenceNonZeros);


    // For the p'th pattern in the dataset, run the image through the dendrites,
    // check the set of active dendrites against the stored set by doing a dot
    // product. Choose category corresponding to highest dot product.
    int classifyPatternKNN(int p, int threshold,
               SparseMatrix01<UInt, Int> *dataSet);

    // If true, we will use the KNN based training/inference
    bool usingKNN_;

    // The number of dendrite prototypes per class
    int nPrototypesPerClass_;
    SparseMatrix01<UInt, Int> *knn_;
    vector<UInt> knn_categories_;
};
