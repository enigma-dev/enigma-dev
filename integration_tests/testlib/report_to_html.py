

from .testreport import Header, SubTestResult, TestResult, Configuration
from .testreport import TestGroup, IntegrationTestResultsReport


def _html_create_header(header):
    """Converts a testreport.Header to html.

    Returns an ElementTree.Element.

    """

    import xml.etree.ElementTree as ET

    et_header = ET.Element("div")

    et_headline = ET.Element("h1")
    et_header.append(et_headline)
    et_headline.text = "Integration testing report"

    for key in sorted(header.settings):
        et_element = ET.Element("p")
        et_header.append(et_element)
        et_element.text = key.title() + ": " + header.settings[key]

    return et_header


def _html_get_simple_element(e_type, text, attribute_dict={}):
    """Returns a simple html node as ElementTree.Element.

    e_type is the node type, "text" is the content, and attribute_dict is
    a dictionary with attributes.

    """

    import xml.etree.ElementTree as ET
    et = ET.Element(e_type, attribute_dict)
    et.text = text
    return et


def _html_create_sub_test_results(sub_test_results):
    """Converts a list of testreport.SubTestResult to html.

    Returns an ElementTree.Element.

    """

    import xml.etree.ElementTree as ET

    et_table = ET.Element("table")
    for sub_test_result in sub_test_results:
        et_row = ET.Element("tr")
        et_table.append(et_row)

        if sub_test_result.result == "success":
            test_result_class = "test_result_good"
        else:
            test_result_class = "test_result_bad"
        et_row.append(_html_get_simple_element("td", sub_test_result.name))
        et_row.append(_html_get_simple_element(
            "td",
            sub_test_result.result,
            {"class": test_result_class}
        ))
        et_row.append(_html_get_simple_element("td", sub_test_result.message))

    return et_table


def _html_create_test_result(test_result):
    """Converts a testreport.TestResult to html.

    Returns an ElementTree.Element.

    """

    import xml.etree.ElementTree as ET

    et_test_result = ET.Element(
        "div",
        {"title": "Test result", "class": "test_result"}
    )

    et_test_result.append(_html_get_simple_element("h5", test_result.name))

    et_test_result.append(_html_get_simple_element(
        "p",
        "Description: " + test_result.description
    ))

    if test_result.end_status == "compiled_and_ran":
        test_result_class = "test_result_good"
    else:
        test_result_class = "test_result_bad"
    et_test_end_status = _html_get_simple_element("p", "Test end status: ")
    et_test_result.append(et_test_end_status)
    et_test_end_status.append(_html_get_simple_element(
        "mark",
        test_result.end_status,
        {"class": test_result_class}
    ))

    et_test_result.append(_html_create_sub_test_results(
        test_result.sub_test_results
    ))

    return et_test_result


def _html_create_configuration(configuration):
    """Converts a testreport.Configuration to html.

    Returns an ElementTree.Element.

    """

    import xml.etree.ElementTree as ET

    et_configuration = ET.Element(
        "div",
        {"title": "Configuration", "class": "configuration"}
    )

    et_configuration.append(_html_get_simple_element(
        "h4",
        "Configuration: " + configuration.api
    ))

    for test_result in configuration.test_results:
        et_configuration.append(_html_create_test_result(test_result))

    return et_configuration


def _html_create_testgroups(testgroups):
    """Converts a list of testreport.TestGroup to html.

    Returns an ElementTree.Element.

    """

    import xml.etree.ElementTree as ET

    et_article = ET.Element("div", {"title": "Test groups"})

    et_article.append(_html_get_simple_element("h2", "Test groups"))

    for testgroup in testgroups:
        et_testgroup = ET.Element(
            "div",
            {"title": "Test group", "class": "testgroup"}
        )
        et_article.append(et_testgroup)

        et_testgroup.append(_html_get_simple_element("h3", testgroup.name))

        for configuration in testgroup.configurations:
            et_testgroup.append(_html_create_configuration(configuration))

    return et_article


def write_html_version(report, target_name):
    """Writes a stand-alone html-version of the given test report.

    The given testreport.IntegrationTestResultsReport is transformed into
    a corresponding html-version which can be viewed in a browser.
    The target_name is the filename for the resulting output html.
    
    """

    import os.path
    target_name = os.path.abspath(target_name)

    import xml.etree.ElementTree as ET

    root = ET.Element("html")

    et_head = ET.Element("head")
    root.append(et_head)

    et_head.append(ET.Element("meta", {"charset": "utf-8"}))

    et_title = ET.Element("title")
    et_head.append(et_title)
    et_title.text = "Integration testing report"

    et_head.append(_html_get_simple_element("style", """\
.test_result {
  background-color:#F0F0F0;
  margin-left:40px;
}
        """))
    et_head.append(_html_get_simple_element("style", """\
.test_result_good {
  background-color:#A0FFA0;
}
        """))
    et_head.append(_html_get_simple_element("style", """\
.test_result_bad {
  background-color:#FFA0A0;
}
        """))
    et_head.append(_html_get_simple_element("style", """\
.configuration {
  margin-left:40px;
  margin-bottom:10px;
}
        """))
    et_head.append(_html_get_simple_element("style", """\
.testgroup {
  margin-bottom:50px;
}
        """))

    et_body = ET.Element("body")
    root.append(et_body)
    et_body.append(_html_create_header(report.header))
    et_body.append(_html_create_testgroups(report.testgroups))

    import os
    try:
        os.remove(target_name)
    except OSError:
        pass
    try:
        import os.path
        os.makedirs(os.path.dirname(target_name))
    except OSError:
        pass

    with open(target_name, "wb") as f:
        f.write("<!DOCTYPE html>".encode())
        ET.ElementTree(root).write(
            f,
            encoding="UTF-8",
            xml_declaration=True,
            method="html"
        )
