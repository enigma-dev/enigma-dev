

from . import parse_test_report
from . import report_to_xml


if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        raise Exception(
            "Expected 2 arguments, namely test report name and output name."
        )
    report_name = sys.argv[1]
    output_name = sys.argv[2]

    import os.path
    output_name = os.path.abspath(output_name)  

    report = parse_test_report.read_test_report(report_name)
    report_to_xml.write_xml_version(report, output_name)
else:
    raise Exception("Main script, do not use indirectly.")
