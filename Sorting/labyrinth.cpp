// Author: Theo Snoey
// This labyrinth file works with a labyrinth of pointers / linked lists to ListNodes. the function is pathToFreedom
// is defined below.

#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "Theo Snoey";

/* Change this constant to the path out of your labyrinth. */
const string kPathForYourName = "ESWSNENNEESWSES";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needed` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needed) {
    MazeCell *current = start;

    if (needed.contains(current->contents)){
        needed.remove(current->contents);
        //current->contents = "";
    }

    if (needed.isEmpty()){
        return true;
    }

    for (char direction: moves){
        // check valid move
        if (!stringContains("NSEW", direction)){
            error("Path contains an invalid character!");
        }
        // based on direction, check for nullptr, else update current pointer to new location
        if (direction == 'N'){
            if (current->north == nullptr){
                return false;
            }
            current = current->north;
        }
        if (direction == 'S'){
            if (current->south == nullptr){
                return false;
            }
            current = current->south;
        }
        if (direction == 'E'){
            if (current->east == nullptr){
                return false;
            }
            current = current->east;
        }
        if (direction == 'W'){
            if (current->west == nullptr){
                return false;
            }
            current = current->west;
        }

        // check for the data contained in the current listNode

        if (needed.contains(current->contents)){
            needed.remove(current->contents);
            //current->contents = "";
        }

        // if we find all the needed objects, we have escaped the maze
        if (needed.isEmpty()){
            return true;
        }
    }

    return false;
}


/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("isPathToFreedom: paths on example labyrinth from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});
    MazeCell *smileyStart = maze[2][2];

    /* These paths are in the writeup. Both succeed. */
    EXPECT(isPathToFreedom(smileyStart, "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(smileyStart, "SWWNSEENWNNEWSSEES", allThree));

    /* This path successfully collects just spellbook */
    EXPECT(isPathToFreedom(smileyStart, "SWWN", {"Spellbook"}));

    /* This path fails, doesn't collect all three items */
    EXPECT(!isPathToFreedom(smileyStart, "WNNE", allThree));

    /* This path fails, cannot walk through nullptr link */
    EXPECT(!isPathToFreedom(smileyStart, "WNWS", allThree));
}

PROVIDED_TEST("isPathToFreedom: collects needed item from start") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("isPathToFreedom: errors if attempt step invalid direction char") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "EWQ", {"Wand"})); // Q is invalid step
    EXPECT(!isPathToFreedom(maze[0][0], "EWWZ", {"Wand"})); // fails at nullptr link
    EXPECT_NO_ERROR(isPathToFreedom(maze[0][0], "EEG", {"Wand"})); // success, rest of path ignored
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathForYourName != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathForYourName, allThree));
}

