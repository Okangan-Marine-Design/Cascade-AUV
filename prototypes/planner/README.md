# State Machines: An Overview

## What is a State Machine?

A state machine is a conceptual model that describes a system in terms of its states, transitions, and events. In simpler terms, it's a way to model the behavior of systems that can be in one of several states at any given time, and how they switch between these states.

## Why Use State Machines?

- **Simplicity**: 
  - State machines allow for breaking down complex behaviors into manageable states, simplifying design, understanding, and debugging.
- **Predictability**: 
  - They provide a clear representation of possible states and transitions, ensuring a system behaves predictably.
- **Robustness**: 
  - They can manage unexpected conditions or errors by defining fallback states or behaviors.
- **Flexibility**: 
  - State machines can be easily extended or modified by adding or changing states or transitions.

## Application in AUV (Autonomous Underwater Vehicle) Mission Planning

State machines can be vital for AUV mission planning:

- **Task Sequencing**:
  - AUV tasks might include navigating, avoiding obstacles, or taking samples. Each task can be a state, and transitions can be based on task completion or specific conditions.
- **Error Handling**:
  - In case of a sensor failure or unexpected obstacle, the AUV can transition to a "safe" state, like surfacing or returning to a known position.
- **Energy Management**:
  - States can represent different energy modes, transitioning based on battery levels or energy-saving needs.

## Code Breakdown

### 1. The `FSM` Class

This class provides the basic structure for a Finite State Machine:

- `self.states`: Stores the various states and linked functions.
- `self.current_state`: Represents the current state.
- `self.start_time`: Tracks time since the current state started.

**Methods**:
- `add_state`: Add a new state and linked function.
- `set_start`: Define the starting state.
- `time_elapsed`: Get the elapsed time since the current state began.
- `run`: Executes the current state's function and updates to the next state based on the returned value.

### 2. `TIMEOUTS` Dictionary

This dictionary specifies the maximum time (in seconds) allowed for each state.

### 3. `success_condition` Function

A function to wait for user input:

- Uses `inputimeout` for user input within a specified time.
- On "1" input, it returns success. Otherwise, or on timeout, it's a failure.

### 4. State Functions

Functions like `gates_state`, `bouy_state`, and `surface_state` represent AUV tasks:

- Each function prints the task's progress, checks for its success, and returns the next state based on the outcome.

### 5. Running the State Machine

The end of the code initializes the state machine, adds the states, sets the start state, and then runs the FSM.