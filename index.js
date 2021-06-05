import { Controller } from './module/controller.js';

window.isAgent = index => api.isAgent[index];

document.addEventListener('DOMContentLoaded', _ => {
  window.game = new Controller('game-board', 'turn-hint');
  game.init();

  window.agentWorker = new Worker('./module/agent.js', { type: 'module' });
  agentWorker.onmessage = function(e) {
    game.agentPlay(e.data);
  }

  if (isAgent(0)) {
    agentWorker.postMessage([0, 0]);
  }
});
