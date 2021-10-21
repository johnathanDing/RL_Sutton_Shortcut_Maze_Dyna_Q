//
//  MazeVisualizer.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/20/21.
//

#include "MazeVisualizer.hpp"

MazeVisualizer::MazeVisualizer(const GridWorld& input_grid_world, int input_pixel_size) :
grid_world(input_grid_world),
start_pos(input_grid_world.getStartPos()),
goal_pos(input_grid_world.getGoalPos()),
grid_pixel(input_pixel_size)
{
    std::cout << "MazeVisualizer initialized!" << "\n";
};


void MazeVisualizer::drawMaze()
{
    // Open the window if it's not already
    if (window.isOpen() == false) {
        startWindow();
    }
    
    // Grid-drawing loop
    while (window.isOpen()) {
        // Create the event object
        sf::Event event;
        
        // Follow the event commands
        while (window.pollEvent(event)) {
            
            // If close, close
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
                std::cout << "User closed the window!" << "\n";
                window.close();
            }
            
            // Clear the window for the next update
            window.clear(sf::Color::White);
            // Draw the GridWorld only
            drawGridWorld();
            
            // Push the drawings in buffer to display
            window.display();
        }
    }
};


void MazeVisualizer::drawMazeEpisode(std::vector<std::tuple<int, int>> grid_pos_episode)
{
    // Open the window if it's not already
    if (window.isOpen() == false) {
        startWindow();
    }
    
    // Get the length of the episode
    int episode_len (static_cast<int>(grid_pos_episode.size()));
    // Starting index
    int i_pos (0);
    
    // Grid-drawing loop
    while (window.isOpen()) {
        // Create the event object
        sf::Event event;
        
        // Follow the event commands
        while (window.pollEvent(event)) {
            
            // If close, close
            if (event.type == sf::Event::Closed) {
                std::cout << "User closed the window!" << "\n";
                window.close();
            }
            
            // If the space key is pressed, move to the next grid position to draw
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                ++i_pos;
            }
            
            // If full episode is depleted, close the window
            if (i_pos == episode_len) {
                std::cout << "Reached the end of the episode!" << "\n";
                window.close();
            }
            
            // Clear the window for the next update
            window.clear(sf::Color::White);
            // Draw the GridWorld first
            drawGridWorld();
            // Then draw the moving object position
            drawGridPos(grid_pos_episode[i_pos]);
            
            // Push the drawings in buffer to display
            window.display();
        }
    }
};


void MazeVisualizer::startWindow()
{
    // Get the grid size of the GridWorld
    int row_num (static_cast<int>(grid_world.getGrid().size()));
    int col_num (static_cast<int>(grid_world.getGrid()[0].size()));
    // Get the pixel size of the GridWorld
    int window_height (row_num * grid_pixel);
    int window_width (col_num * grid_pixel);
    // Start the game window and set vertical sync
    window.create(sf::VideoMode(window_width, window_height), "Johnathan's Grid Maze");
    window.setVerticalSyncEnabled(true);
};


void MazeVisualizer::drawGridWorld()
{
    // Get the grid size of the GridWorld
    int row_num (static_cast<int>(grid_world.getGrid().size()));
    int col_num (static_cast<int>(grid_world.getGrid()[0].size()));
    
    // Set up a single rectangle with inward edge lines
    sf::RectangleShape single_grid (sf::Vector2f(static_cast<float>(grid_pixel), static_cast<float>(grid_pixel)));
    single_grid.setOutlineColor(sf::Color::Black);
    single_grid.setOutlineThickness(-1.0);
    
    // Draw the entire GridWorld
    for (int i_row (0); i_row < row_num; ++i_row) {
        for (int i_col (0); i_col < col_num; ++i_col) {
            // Draw the first row as bottom. Personal preference
            single_grid.setPosition(i_col*static_cast<float>(grid_pixel), (row_num-1-i_row)*static_cast<float>(grid_pixel));
            
            // Fill the maze with white (empty), black (wall), green (start), and blue (goal) colors
            if (std::make_tuple(i_row, i_col) == start_pos) {
                single_grid.setFillColor(sf::Color::Green);
            }
            else if (std::make_tuple(i_row, i_col) == goal_pos) {
                single_grid.setFillColor(sf::Color::Blue);
            }
            else if (grid_world.getGrid()[i_row][i_col] == 1) {
                single_grid.setFillColor(sf::Color::Black);
            }
            else {
                single_grid.setFillColor(sf::Color::White);
            }
            
            // Draw the rectangle into buffer
            window.draw(single_grid);
        }
    }
};


void MazeVisualizer::drawGridPos(std::tuple<int, int> grid_pos)
{
    // Get the grid size of the GridWorld
    int row_num (static_cast<int>(grid_world.getGrid().size()));
    
    // Set up moving object and fill with blue color
    sf::RectangleShape object_pos (sf::Vector2f(static_cast<float>(grid_pixel), static_cast<float>(grid_pixel)));
    object_pos.setFillColor(sf::Color::Red);
    
    // Set the position of moving object
    float object_x (std::get<1>(grid_pos) * static_cast<float>(grid_pixel));
    float object_y ((row_num-1-std::get<0>(grid_pos)) * static_cast<float>(grid_pixel));
    object_pos.setPosition(object_x, object_y);
    
    // Draw the object into buffer
    window.draw(object_pos);
};
