Overview
========

This project contains a simple example of a read-only implicit weighted graph built using the [Boost Graph Library](http://www.boost.org/doc/libs/1_43_0/libs/graph/doc/table_of_contents.html "Boost Graph Library"). It can be used as a starting point for developers creating their own implicit graphs.

The example consists of a single program called `implicit-graph` which creates a simple graph, prints information about it, then uses it in a Dijkstra search.


Installation
============

You must have the Boost Graph Library installed.  Set `BOOST_PATH` in the Makefile to point to the root of your Boost include tree.  The `implicit-graph` program builds in the distribution directory and does not install anywhere else on the system.


History
=======

* **Version 1.0.0**: Initial release


Copyright
=========

Copyright [W.P. McNeill](mailto:billmcn@gmail.com) 2010.

Distributed under the Boost Software License, Version 1.0.

See accompanying file LICENSE\_1\_0.txt or copy at [http://www.boost.org/LICENSE\_1\_0.txt](http://www.boost.org/LICENSE_1_0.txt "Boost License 1.0").
