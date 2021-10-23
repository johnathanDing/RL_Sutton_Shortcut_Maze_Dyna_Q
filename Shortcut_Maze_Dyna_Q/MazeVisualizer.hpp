//
//  MazeVisualizer.hpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/20/21.
//

#ifndef MazeVisualizer_hpp
#define MazeVisualizer_hpp

#include "GridWorld.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

/// Class that visualizes the maze grid world.
class MazeVisualizer
{
private:
    // Constant reference to a GridWorld class instance
    const GridWorld& grid_world;
    // For easy access to starting position
    std::tuple<int, int> start_pos;
    // For easy access to goal position
    std::tuple<int, int> goal_pos;
    // SFML game window
    sf::RenderWindow window;
    // Pixel size for a single grid
    int grid_pixel;
    
public:
    /// Constructor of MazeVisualizer class. Takes an input GridWorld reference and an optional input pixel size
    /// @param input_grid_world A constant reference to input GridWorld class instance
    /// @param input_pixel_size An optional input parameter for grid display pixel size. Default is 200
    MazeVisualizer(const GridWorld& input_grid_world, int input_pixel_size = 200);
    
    /// Draws the maze grid world layout in game window.
    void drawMaze();
    
    /// Draw Maze grid world with given position episode in game window.
    /// @param grid_pos_episode Input grid position episode.
    void drawMazeEpisode(std::vector<std::tuple<int, int>> grid_pos_episode);
    
private:
    /// Starts a SFML game window
    void startWindow();
    
    /// Push maze layout into drawing buffer
    void drawGridWorld();
    
    /// Push the agents position on the maze grid world into drawing buffer
    /// @param grid_pos Current grid position to be drawn
    void drawGridPos(std::tuple<int, int> grid_pos);
};

#endif /* MazeVisualizer_hpp */
