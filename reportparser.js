function getAttrs(xmlElement,attrparam) {
  let res = {};
  for(let attr_i = 0; attr_i < attrparam.length ; attr_i++) {
    if(xmlElement.hasAttribute(attrparam[attr_i])) { //skip attributes which are not present
      let attr_tmp = xmlElement.getAttribute(attrparam[attr_i]);
      res[attrparam[attr_i]] = isNaN( Number(attr_tmp) ) ? attr_tmp : Number(attr_tmp);
    }
  }
  return res; 
}
function getTrace(failMessage) {
  let message = failMessage;
  let bTrace = message.lastIndexOf(":");
  let configTraceIndex = message.indexOf( "[", bTrace==-1 ? message.length : bTrace);
  if(configTraceIndex != -1) {
    return message.substring( configTraceIndex, message.length);
  }
  else {
    return "";
  }
}
function removeTrace(failMessage) {
  if (!getTrace(failMessage))
    return failMessage;
  return failMessage.substring( 0 , failMessage.indexOf("Google Test trace:")-1);
}
function loader(){
  var reportParser = new DOMParser();
  var reportXML = reportParser.parseFromString( document.getElementById( "testreportXML" ).innerHTML, "text/xml" )
                  .getElementsByTagName("testsuites")[0];
  var testharness = getAttrs(reportXML,["tests","failures","disabled"]);
  
  const testharness_template = `<div id = "testsuites_cont">
  <div id = "result">
  <h1>Test Report</h1>
  <table><tr>
  <th>Tests</th><th>Passed</th><th>Failed</th><th>Disabled</th></tr><tr>
  <td>${testharness.tests}</td>
  <td>${testharness.tests - testharness.failures}</td>  
  <td>${testharness.failures}</td>
  <td>${testharness.disabled}</td></tr></table></div>
  <ul id="testsuites"></ul></div>
  `;
  
  document.getElementsByTagName("body")[0].insertAdjacentHTML("beforeend", testharness_template );
  
  var testsuitesXML = reportXML.getElementsByTagName("testsuite");

  for(let i = 0; i < testsuitesXML.length; i++){
    var testsuite = getAttrs(testsuitesXML[i],["name","tests","failures","disabled","skipped"]);

    const testsuite_template = `<li><div>
    <div><span>${testsuite.failures?"&#10060":"&#9989;"}</span>
    <span><h2>${testsuite.name}</h2></span>
    <span>Tests: ${testsuite.tests}</span>
    <span>Passed: ${testsuite.tests - testsuite.failures}</span>
    <span>Failed: ${testsuite.failures}</span>
    <span>Skipped: ${testsuite.skipped}</span>
    <span>Disabled: ${testsuite.disabled}</span>
    </div><ul id="testcases_${i}"></ul></div></li>`;
    
    document.getElementById("testsuites").insertAdjacentHTML("beforeend",testsuite_template );

    var testcasesXML = testsuitesXML[i].getElementsByTagName("testcase");

    for(let j = 0; j < testcasesXML.length; j++){
      var testcase = getAttrs(testcasesXML[j],["name","value_param"]);
      testcase.configpool = testcasesXML[j].getElementsByTagName("properties")[0].getElementsByTagName("property")[0].getAttribute("value").split(",");
      //set game name seperately for parameterized tests and normal tests
      testcase.game = testcasesXML[j].hasAttribute("value_param")
                      ? testcase.value_param.substring(testcase.value_param.lastIndexOf('/')+1,testcase.value_param.lastIndexOf('\"'))
                      : testcase.name;

      var failureXML = testcasesXML[j].getElementsByTagName("failure");

      testcase_template = `<li><div>
        <div>
          <h3>${testcase.game}</h3>
        </div>
        <ul id="noconfigfails_${i}_${j}"></ul>
        <ul id="configs_${i}_${j}"></ul>
      </div></li>`;

      document.getElementById(`testcases_${i}`).insertAdjacentHTML("beforeend",testcase_template);
    
      for(let ii = 0; ii < testcase.configpool.length; ii++){
        img = testsuite.name == "Regression" ?
              `<img src="enigma_${testcase.game}${testcase.configpool[ii]}.png" alt="Image Not Found" width="200">`
              : ``;
        logs = 
        `<a href="${testcase.game}${testcase.configpool[ii]}/enigma_compile.log" target="_blank" rel="noopener noreferrer" width="200">enigma_compile.log</a>
        <a href="${testcase.game}${testcase.configpool[ii]}/enigma_compiler.log" target="_blank" rel="noopener noreferrer" width="200">enigma_compiler.log</a>
        <a href="${testcase.game}${testcase.configpool[ii]}/enigma_libegm.log" target="_blank" rel="noopener noreferrer" width="200">enigma_libegm.log</a>
        <a href="${testcase.game}${testcase.configpool[ii]}/enigma_game.log" target="_blank" rel="noopener noreferrer" width="200">enigma_game.log</a>`;
                                
        config_template = `<li><div class="${testsuite.name}">
        <div>${testcase.configpool[ii]}</div>
        ${img}
        ${logs}
        </div><ul id="${testcase.game}_${testcase.configpool[ii]}"></ul></li>`;

        document.getElementById(`configs_${i}_${j}`).insertAdjacentHTML("beforeend",config_template);
      }

      for(let k = 0; k < failureXML.length; k++) {
        var failure = getAttrs(failureXML[k],["message"]);
        failure.trace = getTrace(failure.message);
        let fail_template = `<li><div><b>Failure: </b><pre>${removeTrace(failure.message)}<pre></div></li>`;
        if (!failure.trace){
          document.getElementById(`noconfigfails_${i}_${j}`).insertAdjacentHTML("beforeend", fail_template);
        }
        else {
          document.getElementById(`${testcase.game}_${failure.trace}`).insertAdjacentHTML("beforeend",fail_template);
        }
      }
    }
  }
}
