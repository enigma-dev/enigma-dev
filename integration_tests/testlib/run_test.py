

def _compile_test(path_source, targets, path_executable):
    """Compiles the given test egm-file.

    Output from stdout/stderr from compilation is redirected to /dev/null.
    "targets" is api targets as a string of the form:

    API_1:SELECTION_1[;API_2:SELECTION_2]

    path_executable is the desired output for the compiled executable.

    """

    import subprocess
    arguments = [
        "java", "-Djna.nosys=true", "-jar", "plugins/enigma.jar",
        path_source, "-output=" + path_executable
    ]
    if targets != "":
        arguments.append(targets)
    return subprocess.call(
        arguments,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )


def _execute_test(path_executable):
    """Executes the given test executable.

    Output from stdout/stderr from compilation is redirected to /dev/null.
    Times out after 3 seconds.

    """

    import subprocess
    return subprocess.call(
        path_executable, timeout=3,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )


def perform_test(path_engine, path_test_folder, test_name, targets):
    """Compiles and executes the test with the given options.

    Records the result in test_name + ".inttest".

    path_engine is the path to the engine, containing folders like
    CompilerSource and ENIGMAsystem.
    path_test_folder is the path to the folder where the test resides.
    test_name is the name of the test, without extensions.
    targets is api selection argument.

    """

    import os

    path_engine = os.path.abspath(path_engine)
    path_test_folder = os.path.abspath(path_test_folder)

    path_source = path_test_folder + "/" + test_name + ".egm"
    path_executable = path_test_folder + "/" + test_name + ".testexe"
    path_test_results = path_test_folder + "/" + test_name + ".inttest"

    if targets == "":
        targets_cleaned = ""
    else:
        targets_cleaned = "-targets=" + targets

    current_directory = os.getcwd()

    # Compile the test and run the test.
    # Whether the test compiles, or the test times out, etc.,
    # the end_status of the test should be recorded in
    # the test results file.

    # Compile the test.
    os.chdir(path_engine)
    return_value = _compile_test(path_source, targets_cleaned, path_executable)
    os.chdir(current_directory)

    with open(path_test_results, "w") as file_test_results:
        file_test_results.write("    test=" + test_name + ":")

        if return_value != 0:
            # Compilation failed.
            file_test_results.write("\n    end_status=failed_compilation\n")
            return

    # Compilation succeeded.
    import subprocess
    try:
        # Run test.
        os.chdir(path_test_folder)
        return_value = _execute_test(path_executable)
        os.chdir(current_directory)
    except subprocess.TimeoutExpired:
        os.chdir(current_directory)
        # Test timed out.
        with open(path_test_results, "a") as file_test_results:
            file_test_results.write("\n    end_status=timeout\n")
        return
    else:
        os.chdir(current_directory)
        with open(path_test_results, "a") as file_test_results:
            if return_value != 0:
                # Test crashed.
                file_test_results.writelines("\n    end_status=crash")
                return

            # The test ran successfully, so it should have written its
            # status correctly by this point.
            # Nothing more needs to be done.
