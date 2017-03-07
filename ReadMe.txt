========================================================================
    STATIC LIBRARY : SystemStore Project Overview
========================================================================

This library is used to save the machine system parameters. Like users, configs, features, calibration results.
The parameters is unique to each machine, so cannot port to another machine. Because each machine is not exactly the same, the calibration results always different.
It is different from the project data. Project data depends on the DUT, so it should can port to another machine.

The library also using BOOST 1.59.0 library. But because it is too big, so i didn't put it into the repository.
To build the library, you need to download the boost 1.50.0 library, and put into .\Extlibs\boost_1_59_0 folder.
