# FTL [![Build Status](https://travis-ci.org/Masstronaut/FTL.svg?branch=master)](https://travis-ci.org/Masstronaut/FTL)
Fast Template Library: A library containing implementations of various template classes and utilities with an emphasis on performance.

Currently, FTL offers:
* ftl::vector - a vector implementation supporting all the interfaces of std::vector
* ftl::inline_vector - a vector derivative that injects an inline storage buffer for small element counts
* ftl::unordered_vector - a vector offering O(1) erase operations without any guarantees about element ordering
* ftl::default_allocator - a std::allocator equivalent
* ftl::linear_stack_allocator - an allocator which linearly assigns memory from a chunk of stack memory
