# RGG Parsing
This is the backend code of RGG Parsing(Reserved Graph Grammar), written in C++, which mainly includes data structures, parsing algorithms and test. 

The frontend part will be written after completing this project, with C# and running on Windows platform probably.

## What is RGG？

In reading the visual language literature, or any book on software engineering, one cannot help but notice that a large variety of visual languages exists of which only a few are equipped with a proper formal syntax definition. Given a syntax definition of a visual language one can easily imagine an editor which supports the creation of diagram according to the syntax. 

Context-free graph grammar, as we all know, has productions in which every left-hand side consists of a single non-terminal node. It is straightforward but awkward to define the syntax of a large portion of visual languages and too restrictive. Therefore we allow for *context-sensitive graph grammar* , in which both left- and right-hand side of production are graphs, even though the performance penalty might be serious.

RGG is the most popular context-sensitive graph grammar, which facilitates the complexity of LGG(Layered graph grammar) by adding some mechanisms.

This project is divided into 3 parts:

* Data Structures
* Parsing algorithms
* Test

## Data Structures

The main data structure of this project is currently the following:

* Vertex
* Node
* Edge
* Graph
* Production

## Parsing Algorithms

The main algorithms of this project are listed below:

* How to find the isomorphic graph of a given graph? (I will refer to my partner about this section)
* How to find all redexes of a host graph？(same as above)
* How to pick the production to replace the redex? (Maybe set an order)
* How to replace the redex? (Solved)

## 27 April 2021 Update

The process is in the below:

1. Complete the data structures
2. Complete the graph-replacement algorithm
3. Write test for the above and fix all the bugs

## 6 May 2021 Update

Finish the function reading host graph and productions from the local file. And pass the test.
