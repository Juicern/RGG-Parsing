# RGG-Backend(Undone)
This is the backend code of RGG(Reserved Graph Grammar), written in C++, which mainly includes data structures and parsing algorithms. 

The frontend part will be written after completing this project, with C# and running on Windows platform probably.

## What is RGG？

In reading the visual language literature, or any book on software engineering, one cannot help but notice that a large variety of visual languages exists of which only a few are equipped with a proper formal syntax definition. Given a syntax definition of a visual language one can easily imagine an editor which supports the creation of diagram according to the syntax. 

Context-free graph grammar, as we all know, has productions in which every left-hand side consists of a single non-terminal node. It is straightforward but awkward to define the syntax of a large portion of visual languages and too restrictive. Therefore we allow for *context-sensitive graph grammar* , in which both left- and right-hand side of production are graphs, even though the performance penalty might be serious.

RGG is the most popular context-sensitive graph grammar, which facilitates the complexity of LGG(Layered graph grammar) by adding some mechanisms.

This project is divided into 2 parts:

* Data Structures
* Parsing algorithms

## Data Structures

The main data structure of this project is currently the following:

* Graph
* Node
* Edge(maybe belongs to Node)
* Production

## Parsing Algorithms

The main algorithms of this project are listed below:

* How to find the isomorphic graph of a given graph?
* How to find all redexes of a host graph？
* How to pick the production to replace the redex?(Maybe set an order)
* How to replace the redex?(fix the embedding problem)