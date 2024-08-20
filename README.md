# Philosophers Project 🍽️
The Philosophers project at 42 School involves **simulating the classic dining philosophers** problem to explore concepts in concurrency and synchronization. 🧠

The program is designed to demonstrate how philosophers (represented as threads) behave when sharing resources (forks). 
The main goal is to **understand and manage synchronization issues**, such as **avoiding deadlocks** and ensuring that all philosophers get a chance to eat.

This project enhances our understanding of thread management, synchronization techniques, and concurrent programming.

## Features 📋
1. **Thread Management:** Create and manage multiple threads representing philosophers.
2. **Synchronization:** Use mutexes and semaphores to handle resource sharing and avoid deadlocks.
3. **Concurrency:** Implement concurrent behaviors and ensure proper synchronization.

## Objectives 🎯
1. Master thread management and concurrency concepts.
2. Develop robust synchronization techniques.
3. Improve understanding of concurrent programming.

## Usage 📜
You need to provide several arguments when running the program to specify the simulation parameters:
**Number of Philosophers**: Specifies how many philosophers (threads) will be involved in the simulation.

**Time to Die**: The time (in milliseconds) a philosopher can go without eating before dying. This simulates the maximum time a philosopher can spend without acquiring both forks.

**Time to Eat**: The time (in milliseconds) it takes for a philosopher to eat once they have both forks.

**Time to Sleep**: The time (in milliseconds) a philosopher spends sleeping after eating. During this time, the philosopher is not attempting to acquire forks.

**Number of Meals:**(Optional): The number of times each philosopher should eat before the simulation ends. If not specified, philosophers will continue eating and sleeping indefinitely.

The simulation **prints messages** to the console to indicate the actions of the philosophers. Each message is formatted as follows:
[timestamp] Philosopher ID: action

[timestamp]: The elapsed time (in milliseconds) since the simulation started.

Philosopher ID: The unique identifier of the philosopher performing the action.

action: The action being performed by the philosopher, such as: is thinking, is eating, is sleeping, has taken a fork, is dead

## Output 🖼️
Here is a screenshot showing the output of the program with basic parameters:
![Capture d’écran 2024-08-20 202156](https://github.com/user-attachments/assets/bab850b2-b585-4004-ba60-8dc32c8c3d2c)

## Installation 🛠️
To explore the Philosophers project: git clone git@github.com:elodieiam/philosophers.git
