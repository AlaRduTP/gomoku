const { contextBridge } = require("electron");

contextBridge.exposeInMainWorld('api', {
  isAgent: eval(window.process.argv[window.process.argv.length - 1])
});
