# qmtg (preliminary name)

A fast MTG simulator

This is a C++ implementation of a fast simulation environment for the Magic The Gathering trading card game. The aim of the project is mainly to be able to test decks against each other by pitting AI players against each other. The architecture is also designed so that players will be able to connect through the network to play each other or AI.

A big emphasis is put on testing, quality and speed of execution. To aid with these goals the following frameworks were chosen as aids:
- entityx - Gives a nice abstraction and flexibility when creating components for the game. Also enables for easy implementation of cache friendly code when we want to simulate many games with large amounts of data.
- catch - A very nice testing framework, allows for BDD-style test cases.
- SFML - Simple graphics framework for UI design (not final decision yet).

2017-02-17: The project is in an early exploratory stage and currently the 
