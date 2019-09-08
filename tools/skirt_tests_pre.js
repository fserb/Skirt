const isNode = typeof process !== 'undefined' && process.versions != null &&
    process.versions.node != null;

var Module = {
  'print': function(text) {
    if (!isNode && window['testLog'])
      window.testLog(text);
    else
      console.log(text);
  },
  'printErr': function(text) {
    if (!isNode && window['testError'])
      window.testError(text);
    else
      console.error(text);
  }
};
