//
//  Shortcut_Maze_Test.m
//  Shortcut_Maze_Test
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#import <XCTest/XCTest.h>
#import "GridWorld.hpp"

@interface GridWorld_Test : XCTestCase {
@private
    GridWorld* test_grid_world;
}

@end

@implementation GridWorld_Test

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    std::vector<std::vector<int>> test_input_grid (6, std::vector<int> (9, 0));
    for (int i(1); i<9; ++i) {
        test_input_grid[2][i] = 1;
    }
    std::tuple<int, int> test_start {0, 3};
    std::tuple<int, int> test_goal {5, 8};
    test_grid_world = new GridWorld(test_input_grid, test_start, test_goal);
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    delete test_grid_world;
    test_grid_world = nullptr;
}

- (void)testGetGrid {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    std::vector<std::vector<int>> got_grid(test_grid_world->getGrid());
    for (int row(0); row<6; ++row) {
        for (int col(0); col<9; ++col) {
            if (row==2 && col>0) {
                XCTAssertEqual(got_grid[row][col], 1);
            } else {
                XCTAssertEqual(got_grid[row][col], 0);
            }
        }
    }
}

- (void)testGetStart {
    std::tuple<int, int> got_start(test_grid_world->getStartPos());
    XCTAssertEqual(std::get<0>(got_start), 0);
    XCTAssertEqual(std::get<1>(got_start), 3);
}

- (void)testGetGoal {
    std::tuple<int, int> got_goal(test_grid_world->getGoalPos());
    XCTAssertEqual(std::get<0>(got_goal), 5);
    XCTAssertEqual(std::get<1>(got_goal), 8);
}

- (void)testChangeGrid {
    XCTAssertEqual(test_grid_world->getGrid()[2][8], 1);
    test_grid_world->changeGrid(2, 8);
    XCTAssertEqual(test_grid_world->getGrid()[2][8], 0);
}

@end
