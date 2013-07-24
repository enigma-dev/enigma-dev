

from . import run_tests_folder


def _remove_ignore_failure(filename):
    """Removes the given file, ignoring any errors."""

    import os
    try:
        os.remove(filename)
    except OSError:
        pass


def run_all_tests(platform, grouptest=None):
    """Runs all the test for the given platform.

    Generates a complete report at integration_tests/tests/inttestreport.txt.
    The report follows the integration test report format.
    The "grouptest" will run only one testsgroup's tests, or all if equal
    to None.

    """

    import os
    import os.path
    import glob

    path_base = os.path.abspath(
        os.path.dirname(os.path.realpath(__file__)) + "/.."
    )

    path_engine = os.path.abspath(path_base + "/..")
    tests_report = "inttestreport.txt"
    path_all_tests = path_base + "/tests"
    path_tests_report = path_all_tests + "/" + tests_report

    # Cleanup.
    for filename in glob.glob(path_all_tests + "/*.inttest"):
        _remove_ignore_failure(filename)

    with open(path_tests_report, "w") as f:
        f.write("type=integration_test\n")
        import datetime
        timenow = datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S")
        f.write("time=" + timenow + "\n")
        f.write("platform=" + platform + "\n")

        # Generate test results for each folder.
        if grouptest == None:
            grouptests_to_look_through = glob.glob(path_all_tests + "/*")
        else:
            grouptests_to_look_through = [path_all_tests + "/" + grouptest]
        for filename in grouptests_to_look_through:
            if not os.path.isdir(filename):
                continue
            group_name = os.path.basename(filename)
            run_tests_folder.run_tests(
                group_name, path_all_tests,
                platform, path_engine
            )

        # Concatenate and create final test results.
        for filename in sorted(glob.glob(path_all_tests + "/*.inttest")):
            with open(filename, "r") as f_dir_part:
                f.write(f_dir_part.read(None))
                f.write("\n")

    # Cleanup.
    for filename in glob.glob(path_all_tests + "/*.inttest"):
        _remove_ignore_failure(filename)


if __name__ == "__main__":
    import os
    import sys
    if len(sys.argv) != 2:
        raise Exception(
            "Wrong number of arguments: Expected one argument, platform, "
            "but got: " + str(sys.argv)
        )

    platform = sys.argv[1]
    run_all_tests(platform)
