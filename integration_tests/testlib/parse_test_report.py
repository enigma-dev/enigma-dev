

from .testreport import Header, SubTestResult, TestResult, Configuration
from .testreport import TestGroup, IntegrationTestResultsReport


def _error_exit(string):
    raise Exception(string)


# Parsing.

def _get_next_line(f, end_is_ok=False):
    """Gets the next line in the file input stream.

    Skips newlines. If reading reaches end-of-file, raise an exception,
    unless end_is_ok is true

    """

    a = f.readline()
    while a == "\n":
        a = f.readline()
    if a == "" and not end_is_ok:
        _error_exit("Error: Reached end of file prematurely.")
    # Ignore non-newline starting whitespace.
    a = a.lstrip()
    # Remove trailing newline if any.
    a = a[0:len(a) - 1]
    return a


def _read_in_mapping(string):
    """Reads in a key-value pair separated by "=" and returns it."""

    (a1, sp, a2) = string.partition("=")
    return (a1, a2)


def _read_in_header(f):
    """Reads in the header from the file input stream.

    Returns testreport.Header. The file input stream is left after the header
    ends.

    """

    settings = {}
    # Three settings.
    for iii in range(3):
        (a1, a2) = _read_in_mapping(_get_next_line(f))
        settings[a1] = a2

    return Header(settings)


def _read_in_test(f, test_name_description):
    """Reads in a test result from the file input stream.

    Returns testreport.TestResult. The file input stream is left after the
    test result.

    """

    (test_name, s, description) = test_name_description.partition(":")

    current = _get_next_line(f)
    subtest_results = []
    (subtest, s, content) = current.partition("=")
    while subtest == "subtest":
        [name, result, message] = content.split(":", 2)
        subtest_results.append(SubTestResult(name, result, message))
        current = _get_next_line(f)
        (subtest, s, content) = current.partition("=")

    (a1, s, end_status) = current.partition("=")
    if a1 != "end_status":
        _error_exit("Error: Encountered bad end_status: " + current)

    return TestResult(test_name, description, end_status, subtest_results)


def _read_in_configuration(f):
    """Reads in a configuration from the file input stream.

    Returns testreport.Configuration. The file input stream is expected to
    start just before the opening "{" of the configuration, and ends after
    the closing "}".

    """

    start = _get_next_line(f)
    if start != "{":
        _error_exit("Error: Encountered bad start of configuration: " + start)

    current = _get_next_line(f)
    (a1, api) = _read_in_mapping(current)
    if a1 != "api":
        _error_exit("Error: Missing api setting: " + current)

    current = _get_next_line(f)
    test_results = []
    while current != "}" and current != "":

        (a1, s, test_name_description) = current.partition("=")
        if a1 != "test":
            _error_exit("Error: Encountered bad test start: " + current)

        test_results.append(_read_in_test(f, test_name_description))

        current = _get_next_line(f)

    if current == "":
        _error_exit("Error: Reached end of file prematurely.")

    return Configuration(api, test_results)


def _read_in_testgroup(f):
    """Reads in a testgroup from the file input stream.

    Returns testreport.TestGroup. The file input stream is expected to start
    just before the opening "{" of the testgroup, and ends after the closing
    "}".

    """

    start = _get_next_line(f)
    if start != "{":
        _error_exit("Error: Encountered bad start of test group: " + start)

    current = _get_next_line(f)
    (a1, name) = _read_in_mapping(current)
    if a1 != "name":
        _error_exit("Error: Missing name: " + current)

    current = _get_next_line(f)
    configurations = []
    while current != "}" and current != "":

        if current != "--configuration":
            _error_exit("Error: Expected configuration start, got: " + current)

        configurations.append(_read_in_configuration(f))

        current = _get_next_line(f)

    if current == "":
        _error_exit("Error: Reached end of file prematurely.")

    return TestGroup(name, configurations)


def _read_in_integration_test_results_report(f):
    """Reads in a full report from the file input stream.

    Returns testreport.IntegrationTestResultsReport.

    """

    header = _read_in_header(f)

    current = _get_next_line(f, end_is_ok=True)
    testgroups = []
    while current != "":
        if current != "-testgroup":
            _error_exit("Error: Expected start of test group, got: " + current)
        testgroups.append(_read_in_testgroup(f))
        current = _get_next_line(f, end_is_ok=True)

    return IntegrationTestResultsReport(header, testgroups)


def read_test_report(file_name):
    """Parses a full report from the given file name.

    Returns testreport.IntegrationTestResultsReport.

    """

    with open(file_name, 'r') as f:
        return _read_in_integration_test_results_report(f)
