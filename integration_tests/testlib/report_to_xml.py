

from .testreport import Header, SubTestResult, TestResult, Configuration
from .testreport import TestGroup, IntegrationTestResultsReport


# Transformation to XML.

def _xml_create_testcases(test_results, packagename):
    """Returns a list of ElementTree.Element containing test cases.

    The test cases are constructed from the testreport.TestResult
    and testreport.SubTestResult contained in the arguments given.
    test_results is a list of TestResult, and packagename is the
    indicates the general grouping for the test results,
    such as configuration and testgroup.

    """

    import xml.etree.ElementTree as ET

    et_test_results = []

    for test_result in test_results:

        classname = packagename + "." + test_result.name

        for sub_test_result in test_result.sub_test_results:
            et_testcase = ET.Element(
                "testcase",
                {"name": sub_test_result.name, "classname": classname}
            )

            if sub_test_result.result in ["failure", "unknown"]:
                et_testcase.append(ET.Element(
                    "failure",
                    {
                        "type": sub_test_result.result,
                        "message": sub_test_result.message
                    }
                ))

            et_test_results.append(et_testcase)

        # Append status test that shows the overall status of the test.
        et_final_testcase = ET.Element(
            "testcase",
            {"name": "overall_test_status", "classname": classname}
        )
        end_status_bad = {"failed_compilation", "timeout", "crash", "error"}
        if test_result.end_status in end_status_bad:
            et_final_testcase.append(ET.Element(
                "failure",
                {"type": test_result.end_status}
            ))
        et_test_results.append(et_final_testcase)

    return et_test_results


def _xml_create_testsuites(testgroup):
    """Returns a list of ElementTree.Element containing test suites.

    The test suites are created from the given list of testreport.TestGroup.
    A test suite is created for each configuration in each test group.

    """

    import xml.etree.ElementTree as ET

    et_testsuites = []

    for configuration in testgroup.configurations:
        et_testsuite = ET.Element(
            "testsuite",
            {"name": (testgroup.name + ": " + configuration.api)}
        )

        packagename = testgroup.name + ": " + configuration.api
        et_testcases = _xml_create_testcases(
            configuration.test_results, packagename
        )
        et_testsuite.set("tests", str(len(et_testcases)))
        et_testsuite.extend(et_testcases)

        et_testsuites.append(et_testsuite)

    return et_testsuites


def write_xml_version(report, target_name):
    """Writes a Jenkins/JUnit-compatible XML-version of the given test report.

    The given testreport.IntegrationTestResultsReport is transformed into
    a corresponding XML-version which is compatible with the JUnit test
    output expected from the continuous integration system Jenkins.
    The target_name is the filename for the resulting output XML.

    """

    import os.path
    target_name = os.path.abspath(target_name)

    import xml.etree.ElementTree as ET

    root = ET.Element("testsuites")

    for testgroup in report.testgroups:
        root.extend(_xml_create_testsuites(testgroup))

    try:
        import os.path
        os.makedirs(os.path.dirname(target_name))
    except OSError:
        pass
    ET.ElementTree(root).write(target_name)
