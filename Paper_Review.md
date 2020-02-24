# Visualizing and Understanding Convolutional Network

## Introduction
### 1. Briefly explain the topic of the text
   - The paper is mainly related to show the visualization opertation of convolution Network and show the details of how CNN extract detailed feature from each layer by visualization method. Besides, it perform an ablation study to discover the performance contribution from each layer of the network.
### 2. Present the aim of the text and summarize the main finding or key argument.

- I. Introduce a visualization technique the reveals the input stimuli that excite feature maps at each layer of the network model
- II. Observe the development and evolution of features during training to diagnose the potential problems in model
- III.Propose use a muti-layered Deconvolution Nework to project the feature activation back to the input pixel space
- IV. Perform a sensitivity analysis of classifier, review which part of sense are important for classification

### 3. Conclude the introduction with a brief statement of your evaluation of the text.-
- Visualization with Deconvnet use top-down projections that reveal structures within each patch that stimulate a particular feature map demonstrate a clear understanding of how the network cature feature details, and the readers of the paper will have an incisive conception of the framework of CNN from a scientific standpoint.


## Summmary
### 1. Present a summary of the key points along with a limited number of examples.
- I. Presented a novel way to visualize the activity within the model, reveals the features
- II. Showed how these visualization can be used to debug problems with the model to obtain result, for example, improve Krizhevsky impressive ImageNet 2012 result.
- III. Demonstrate through a series of occlusion experiments that the model, while trained for classification, is highly sensitive to local structure in the image.
- IV. An ablation study on the model revealed that having a minimum depth to the network.
- V. Showed how the imageNet trained model can generalize well to other datasets.

## Critique
### 1. A balanced discussion and evaluation of the strengths, weakness and notable features of the text. 
- The strengths of the paper is that comparing with other visulization methods proposed by other authors, the main advantages of the method in this paper is for the higher layer, the invariances are extremely comlexed and would be poorly captured by a simple quadratic approximation, but the author provides a non-paramatric view of invariance, showing which parterns from the training set activate the feature map. Besides, proposed a top-down projections reveal the structure.
- The weakness of the paper does not provide any pseudocodes to demonstrate how some of the de-convnet algorithm .
- The notable features of the text is a new method called top to down projection to show visualization.
## Conclusion
### 2. Restate your overall opinion of the text
- The paper present a novel way to map these activities back tothe input pixel space, showing what input pattern originally caused a given activation in the feature maps.
### 3. Briefly present recommendations
- To demonstrate some pseudocode of key algorithm of the method the paper uses.
### 1. If necessary, some further qualification or explanation of your judgment can be included.
- The paper is excellent but lack of source code to read.
### 5. Include a list of references at the end.
 - CONF Zeiler, Matthew D. Fergus, Rob Fleet, David Pajdla, Tomas Schiele, Bernt Tuytelaars, Tinne 2014 Visualizing and Understanding Convolutional Networks Computer Vision – ECCV 2014 818 833 Springer International Publishing Cham  978-3-319-10590-110.1007/978-3-319-10590-1_53
 
