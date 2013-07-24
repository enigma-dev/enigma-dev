

import testlib.run_all_platform_tests
import testlib.parse_test_report
import testlib.report_to_html
import testlib.report_to_xml
import sys


if sys.version_info[0] < 3 or sys.version_info[1] < 3:
    raise Exception(
        "Error: Python version at least 3.3. is required, "
        "but encountered version: " + str(sys.version_info)
    )


def run_and_record_tests(grouptest=None):
    """Runs and records tests.

    The test report is outputted to html and Jenkins-JUnit-xml.
    The "grouptest" argument can indicate a specific folder.
    If not given, or None, all tests will be run.
    If given, only the tests in that testgroup will be run.

    """

    # Detect OS.
    # See: http://docs.python.org/3/library/sys.html#sys.platform,
    # as well as the test report format.
    import sys
    import os
    sys_platform_to_platform = {
        "linux": "linux",
        "win32": "windows"
    }
    sys_plat = sys.platform
    if sys_plat not in sys_platform_to_platform:
        raise Exception(
            "Current platform not supported by the integration testing: " +
            sys_plat
        )
    platform = sys_platform_to_platform[sys_plat]

    path_base = os.path.abspath(
        os.path.dirname(os.path.realpath(__file__))
    )

    # Run tests, parse report, export to html and Jenkins-xml.
    if grouptest != None:
        testlib.run_all_platform_tests.run_all_tests(platform, grouptest)
    else:
        testlib.run_all_platform_tests.run_all_tests(platform)
    import glob
    for filename in glob.glob(path_base + "/output/*"):
        try:
            os.remove(filename)
        except OSError:
            pass

    import os.path
    html_output_name = os.path.abspath("output/testreport.html")
    xml_output_name = os.path.abspath("output/testreport_jenkins.xml")

    report = testlib.parse_test_report.read_test_report(path_base + "/tests/inttestreport.txt")
    testlib.report_to_html.write_html_version(report, html_output_name)
    testlib.report_to_xml.write_xml_version(report, xml_output_name)
    

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 1:
        run_and_record_tests()
    elif len(sys.argv) == 2:
        run_and_record_tests(sys.argv[1])
    else:
        raise Exception("Can receive at most 1 argument, but received: " + str(sys.argv))
