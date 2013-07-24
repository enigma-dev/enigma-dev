Integration testing - Usage and development guide
========================================================

Running tests and viewing results
--------------------------------------------------------

In order to run all tests, start a terminal, navigate to the folder "integration\_tests", and execute the Python script:

    run_and_record_tests.py

A full text report can be found in "integration_tests/tests/inttestreport.txt". Other output versions (including a human-readable html-version and Jenkins-JUnit-readable XML-version) can be found in the folder "output". If you only want to run the tests in a specific folder, simply pass the name of the folder as an argument to the script.

Examples of running the script from the command-line (starting from the folder enigma-dev):

Ubuntu 13.04:

    cd integration_tests
    python3 run_and_record_tests.py // This runs all tests.
    python3 run_and_record_tests.py extensions // This runs the tests in the folder "tests/extensions".

Adding a test
--------------------------------------------------------

Integration tests are implemented through the use of regular games. This means that they can be created and modified like other games. Tests must be saved in the EGM file format. The name of the test must be a valid C++ identifier, and it must be saved like this: "name-of-test).egm".

The easiest way to add a new test is to copy an old test, change its name, open it in an editor, study how it works, and modify it.

Every test file must be put into a test folder, or testgroup. Testgroups each have a configuration file which indiates which combinations of API-selections it will be executed for. The testing library will execute each test in the testgroup once for each appropriate configuration. The configuration file for each folder is called "testing\_config". Like tests, the name of a testgroup must be a valid C++ identifier.

When writing the test itself, you must enable an extension called "Integration testing". When starting the test, the function `integration_testing_begin` must be called, and when ending it, `integration_testing_end` (the last function will also stop execution). For recording sub-test results, there are several functions available, such as `integration_testing_record_conditional`. Read more about them in the documentation for the "Integration testing" extension.

Note that there is a timeout on tests; tests should be very quick to execute, in order to avoid meeting the timeout. Longer timeouts may be supported in the future.

Adding a testgroup
--------------------------------------------------------

A testgroup can be added simply by creating a new folder in "integration\_tests/tests" and adding a configuration file to it named "testing\_config". The format of the configuration file is simple. An example:

    linux=graphics:OpenGL1
    linux=graphics:OpenGL3
    windows=graphics:OpenGL1
    windows=graphics:OpenGL3
    windows=graphics:DirectX

If you want to add more than one API-selection to a given configuration, simply add ";" and the API-selection to the end of the configuration.

Developing
--------------------------------------------------------

The core part of the integration testing system is the integration testing report. Its format is described in "integration\_tests/format/test\_file\_format". It is designed to be easy to read, to output to, and to parse.

There are two main code parts of the integration testing system. The first is the "Integration testing" extension, which provides functionality for writing integration tests in a test file. It is found under "ENIGMAsystem/SHELL/Universal\_System/Extensions/IntegrationTesting". The second part handles the execution and recording of tests, as well as parsing test reports and outputting them different formats. This part is written in Python, and can be found under "integration\_testing/testlib/" and the "run\_and\_record\_tests.py" script. Note that a few parts rely on Python 3.3 (such as the timeout mechanism added in 3.3 to subprocess.call).

Overview of integration testing structure
--------------------------------------------------------

| Folders and files              | Info                                                        |
| ------------------------------ | ----------------------------------------------------------- |
| integration\_tests/            |                                                             |
| ....format/                    | Information about the format                                |
| ....output/                    | Test reports in different formats, temporary files only     |
| ....testlib/                   | Python scripts for dealing with tests and test results      |
| ....tests/                     | Testgroups                                                  |
| ........testgroup\_1/          | Contains tests                                              |
| ........testgroup\_2/          | Contains tests                                              |
| ........testgroup\_3/          | Contains tests                                              |
| ........inttestreport.txt      | Test report, main format                                    |
| ....README.md                  | This README                                                 |
| ....run\_and\_record\_tests.py | Main script to run all or some tests                        |

