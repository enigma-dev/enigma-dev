

from . import run_test


def _remove_ignore_failure(filename):
    """Removes the given file, ignoring any errors."""

    import os
    try:
        os.remove(filename)
    except OSError:
        pass


def run_tests(group_name, path_all_tests, platform, path_engine):
    """Runs the tests in the given testgroup.

    Records the results in group_name + ".inttest".

    group_name is the name/folder name of the testgroup.
    "platform" is the platform testing on.
    "path_all_tests" is the base folder of the test groups.
    path_engine is the path to the engine.

    """

    import os
    import os.path

    path_all_tests = os.path.abspath(path_all_tests)
    path_engine = os.path.abspath(path_engine)

    path_folder = path_all_tests + "/" + group_name
    path_tests_results = path_all_tests + "/" + group_name + ".inttest"

    # Clean up.
    import glob
    for filename in glob.glob(path_folder + "/*.testexe"):
        _remove_ignore_failure(filename)
    for filename in glob.glob(path_folder + "/*.inttest"):
        _remove_ignore_failure(filename)

    with open(path_tests_results, "w") as f_res:
        # Initialize test results.
        f_res.write("-testgroup\n")
        f_res.write("{\n")
        f_res.write("  name=" + group_name + "\n")

        # Read through test configuration file.
        with open(path_folder + "/testing_config") as f_conf:
            for line in f_conf:
                [key, value] = line.split("=", 1)
                if key != platform:
                    continue
                value = value[:len(value) - 1]

                f_res.write("  --configuration\n")
                f_res.write("  {\n")
                f_res.write("    api=" + value + "\n")

                # Run all tests with current configuration.
                for filename in glob.glob(path_folder + "/*.egm"):
                    import os.path
                    test_name = os.path.basename(filename)
                    # Remove the ".egm" part.
                    test_name = test_name[:len(test_name) - len(".egm")]
                    run_test.perform_test(
                        path_engine, path_folder, test_name, value
                    )

                # Collect and write test results.
                for filename in glob.glob(path_folder + "/*.inttest"):
                    with open(filename) as test_file:
                        f_res.write(test_file.read(None))
                        f_res.write("\n")

                f_res.write("  }\n")

                # Clean up.
                for filename in glob.glob(path_folder + "/*.testexe"):
                    _remove_ignore_failure(filename)
                for filename in glob.glob(path_folder + "/*.inttest"):
                    _remove_ignore_failure(filename)

        f_res.write("}\n")
