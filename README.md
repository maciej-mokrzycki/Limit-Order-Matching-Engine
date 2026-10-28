
# Limit Order Matching Engine

An educational, single-threaded C++20 matching engine with an
in-memory limit order book for a single instrument.

## Features

- Buy and sell limit orders.
- Price-time priority: best price first, then FIFO at each price level.
- Partial fills and matching across multiple price levels.
- Trade execution at the resting order's price.
- Integer tick pricing.
- Input validation and duplicate order ID rejection.
- Command-line interface for submitting orders and viewing the book.

## Build and Run

Requires a C++20-compatible compiler and standard library.
No third-party dependencies.

```bash
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic main.cpp -o matching
./matching
```

## Commands

 Command Description

 `BUY id price quantity`  Submit a buy limit order
 `SELL id price quantity`  Submit a sell limit order 
 `BOOK`  Display the order book 
 `QUIT`  Exit the program 

IDs, prices, and quantities must be positive integers.
Order IDs cannot be reused during the same program session.

Prices are expressed in integer ticks. For example, if one tick
represents 0.01 currency units, a price of 10050 represents 100.50.

### Example

```text
SELL 1 100 5
BUY 2 100 3
BOOK
QUIT
```

The engine executes a trade for 3 units at price 100.
The remaining 2 units of sell order 1 stay in the book.

## Implementation

Each side of the book uses a `std::map` of price levels.
Each level contains a `std::deque` of orders in arrival order.

The engine selects the best available price on the opposite side
and matches against the oldest order at that price.
Unfilled quantities remain in the book.


## Planned Development
- Modular C++ library with automated tests.
- Order cancellation and modification, multi-instrument trading, and additional order types.
- Benchmarks to guide data structure and memory optimizations.
- Deterministic market simulation with trading bots.


