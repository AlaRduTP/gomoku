const { Agent } = require('../build/Release/addon');

const agents = new Array();
for (let i = 0; i < 2; ++i) {
  agents.push(new Agent());
  agents[i].newGame();
}

onmessage = function(e) {
  postMessage(agents[e.data[0]].play(e.data[1]));
}