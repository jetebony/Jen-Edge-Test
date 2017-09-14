# integra-utils library

contains:

- jsonreport
- stringutils
- logger
- mossie - Mosquitto interface

# Dependencies

- libjson (libjson-c)
- libmosquittopp

# Building
The Makefile supported both ARM and x64 builds.
To build the library on ARM using the cross compiler:

```
make clean && make arm
```
To build and run the unit tests on ARM:
```
make clean && make armtest
```

To build on the PC with native build tools:
```
make clean && make
```
To build and run the unit tests:
```
make clean && make test
```

The output files will be placed as follows:

Libraries in ../lib
Binaries in ../bin

# Provides

`CJsonReport` - a class for producing JSON formatted reports / responses. It handles the basic header requirements:
- success
- errors
- warnings
Additional data can be added using the `addData` methods. The `toString` method will produce a string in JSON format.

`StringUtils` - a collection of functions for handling `std::string`. Functions include:
- `trim()` to remove leading and trailing white space
- `split()` to split a string into an array of substring seperated by a specified delimiter.

`CLogger` - a singleton class to log data via Mosquitto to the audit database, and debug system.
The `setVerbose` method turns on or off the logging of debug messages.
The `setModule` method sets which module is logging the data, e.g. linkstated
The module name must be set before sending log data, otherwise the Mosquitto 'topic' will be incorrect.

The `writeLogEntry` method performs the logging action by publishing to Mosquitto. There is an optional `description`
argument to provide more information about the event.
There is also an optional 'subtopic' argument for interface specific logging.

`CMossie` - a singleton class to handle communication using the Mosquitto message broker system.


# Sample Code
```
#include <integra-utils/logger.h>

using namespace integra_utils;

int main(int argc, char *argv[])
{
	CLogger::setModule("test_module");

	// Send data top the audit log using topic: audit/test_module
	CLogger::instance().writeLogEntry(CLogger::Level::Information, "Audit log data");

	// Send data top the debug system (debugging must be enabled first)
	// using topic: debug/test_module/eth0
	CLogger::instance().setVerbose(true);
	CLogger::instance().writeLogEntry(CLogger::Level::Debug, "Debug data", "eth0");

	return EXIT_SUCCESS;
}
```

