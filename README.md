# Flood-It

A C implementation of the Flood-It puzzle game with multiple solving strategies.

## Overview

Flood-It is a color-based puzzle game in which the objective is to make the entire grid a single color by selecting colors that progressively expand the controlled area.

This project was developed as part of a university assignment. It explores recursive algorithms, data structures, and different strategies for solving the game.

## Features

- Grid generation with configurable dimensions
- Configurable number of colors
- Multiple solving modes
- Recursive flood-fill algorithm
- Graphical interface using SDL2
- Execution time and move count measurements

## Technologies

- C
- SDL2
- GCC
- Make

## Requirements

On Ubuntu or Debian, install the required tools and SDL2 development library:

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev
