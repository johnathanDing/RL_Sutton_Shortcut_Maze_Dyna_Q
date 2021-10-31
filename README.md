# Sutton's Shortcut Maze with Dyna-Q and Dyna-Q+


      Before shortcut opens        Dyna-Q after shortcut opens    Dyna-Q+ after shortcut opens

<p float="center">
  <img src="./Examples/No_Shortcut_Recording.gif" width="32%" />
  <img src="./Examples/Dyna-Q_Recording.gif" width="32%" />
  <img src="./Examples/Dyna-Q_Plus_Recording.gif" width="32%" />
</p>

This problem aims to compare and discuss the control methods of Dyna-Q and Dyna-Q+, and their effects on training under various heurisitics and parameter settings.

This solution was written in C++ as main language, on Xcode 13.X platform. Visualization of race track requires installation of the SFML packages.
Unit testings were carried out using XCTest as well. All modules have tests except the visualization module, which cannot be supported by XCTest.

## Problem Statement:

