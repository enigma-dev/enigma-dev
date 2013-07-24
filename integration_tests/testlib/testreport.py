

class Header:
    """Header describes the first part of the test report, before sections.

    The "settings" attribute is a dictionary from keys to values,
    such as the key-value pair: "platform": "linux".

    """

    def __init__(self, settings):
        # Settings is a map from key to value.
        self.settings = settings


class SubTestResult:
    """Subtest result, as defined in the test format.

    Its attributes are described in the test format.

    """
    def __init__(self, name, result, message):
        self.name = name
        self.result = result
        self.message = message


class TestResult:
    """Test result, as defined in the test format.

    Its attributes are described in the test format,
    except for sub_test_results, which is a list of
    SubTestResult.

    """

    def __init__(self, name, description, end_status, sub_test_results):
        self.name = name
        self.description = description
        self.end_status = end_status
        self.sub_test_results = sub_test_results


class Configuration:
    """Configuration, as described in the test format.

    test_results is a list of TestResult.

    """

    def __init__(self, api, test_results):
        self.api = api
        self.test_results = test_results


class TestGroup:
    """Testgroup, as described in the test format.

    "name" is the name of the testgroup, and "configurations"
    is a list of Configuration.

    """

    def __init__(self, name, configurations):
        self.name = name
        self.configurations = configurations


class IntegrationTestResultsReport:
    """A complete test results report.

    "header" is a Header, and "testgroups" is a list of
    TestGroup

    """

    def __init__(self, header, testgroups):
        self.header = header
        self.testgroups = testgroups
