AI Demonstration - by Bradley Booth

AIE 13/8/17, Duncan Henderson and Jeff Cotter

Important Notes:

- this project contains a game that has been split into 4 main parts (see README entities)
  trading, pathfinding, steering and planning.

- each part can be enabled by pressing 1, 2, 3 or 4

- each part demonstrates a different (sometimes multiple) decision making techniques

- code is fully documented with function declarations and general explanations

- a large amount of the systems in this project use the Graph structure, which stores
  data in a series of nodes without any solid rules on how these nodes are connected.
  each system must manage the graph itself

- all decision making techniques are used on at least 2 entities



Decision Making Techniques:

below are detailed descriptions of what each decision making techique does

-------------------------------------------------------------------------------------------------

- Finite State Machine:

  - uses a graph structure of States and Transitions to determine the
    active state

  - states themselves automatically have update functions that get called

  - transitions automatically cycle the state if their condition evaluates to true


- Blackboard:

  - a base type for an item on the blackboard hides templated items,
    this allows for blackboards with multiple types

  - items have a response system, allowing for experts to respond to items
    on the blackboard and arbeiters to examine these responses

  - items also have automatic expiry timers built into them, allowing for
    static or dynamic messages that remove themselves after a certain amount of time

  - items that could expire can also be locked, meaning they are static until
    they are unlocked by an agent


- Behaviour Tree:

  - several behaviours are connected together in a tree like structure
  
  - includes Action and Condition leaf nodes
  
  - includes Composite nodes such as the Selector and the Sequence

  - includes an additional Decorator node which implements custom behaviour

  - all nodes in the tree use function pointers to gain their functionality

  - the behaviour tree supports PENDING responses and handles it accordingly


- Planner:

  - has a PlannerState which can be modified with a PlannerAction

  - both can be inherited, allowing for the user to create custom planner routines

  - uses the A* search algorithm to determine the smallest set of actions
    to turn the starting state into the goal state

  - dynamically creates new states when they are needed rather than 
    creating the entire domain before searching

-------------------------------------------------------------------------------------------------



Pathfinding Techniques:

-------------------------------------------------------------------------------------------------

- Navigational Mesh:

  - uses mesh like data from a .txt file to load in the mesh into a Graph structure

  - optimises the mesh data for pathfinding and path-smoothing

  - automatically determines the correct triangles on the mesh to use for pathfinding
    given two points independent from the mesh


- A* Pathfinding:

  - multiple entities using the same graph

  - uses A* to determine the shortest path between two triangles

  - custom heuristics, 3 are included in Heuristics.cpp/.h (Manhattan, Diagonal and Euclidean)

  - bounded relaxation, a value which changes the way A* works.

    a value of 1 allows A* to behave like normal, always finding the optimal path

    a value of 0 makes A* behave like Dikjstra's Algorithm, ignoring 'H' scores altogether

    values higher than 1 allow paths to be found faster than regular A*, but sacrifices finding
    optimal paths

    eg. a value of 5 means the search could be 5 times faster, but the path will be terrible.
    this does not mean that values above 1 cannot find the optimal path


- Path Smoothing

  - portals (lines that must be crossed along the path) are used to determine which 
    nodes in the path can be skipped, and then cut off


- Shoulder Width

  - the Navigational Mesh determines the maximum shoulder width of an
    agent that can be inside this triangle

  - A* only searches through triangles which are wide enough to have the agent
    that called the search fit inside it

  - smoothed paths cut accross the corners less to account
    for the shoulder width increase

    this is achieved by shrinking the portal edges towards the centre by the shoulder width

-------------------------------------------------------------------------------------------------



Steering Behaviours:

-------------------------------------------------------------------------------------------------

- Multiple Steering Behaviours:

  - a single steering behaviour is implemented as an inherited child

  - a SteeringBehaviourManager can store multiple behaviours in a list
  
  - each returns an acceleration vector to apply to the overall system

  - steering behaviour managers clamp velocity and acceleration, allowing
    for control over how fast the boid moves and turns


- Automatic Weighted Combination:

  - steering behaviours are combined by multiplying their acceleration vectors
    by a weight, and then adding this to a sum.

  - if the acceleration is to large, it is scaled down to the maximum. this means 
    that the weights become ratios of the maximum acceleration rather than multipliers

- Collision Avoidance:

  - the obstacle avoidance behaviour steers the boid away from obstacles using two feelers
    which dynamically adjust in length based on velocity

  - the feelers detect collisions between 3 different shapes; Circles, Boxes and Polygons


- List of Steering Behaviours implemented (9):

  Seek
  Flee

  Pursue
  Evade

  Cohesion
  Seperation
  Allignment

  ObstacleAvoidance

  Queue (makes boids line up, this becomes obvious when multiple travel towards the same target for a long time)






