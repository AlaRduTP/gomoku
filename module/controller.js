import { Model } from './model.js';
import { View } from './view.js';

export class Controller {
  constructor(boardId, hintId) {
    this.model = new Model();
    this.view = new View(boardId, hintId);
    this.over = false;
  }

  init() {
    this.model.init();
    this.view.init(this);
  }

  checkWinner(crossId) {
    const turn = this.model.board(crossId);
    const move = [1, 16, 17, 18];

    for (let i = 0; i < 4; ++i) {
      const m = move[i];
      const combo = [crossId];

      for (let p = crossId; this.model.board(p - m) == turn; p -= m, combo.push(p));
      for (let p = crossId; this.model.board(p + m) == turn; p += m, combo.push(p));

      if (combo.length >= 5) {
        return combo;
      }
    }
    return [];
  }

  playChess(cross, crossId) {
    this.updateBoard(cross, crossId, this.model.turn);

    const combo = this.checkWinner(crossId);
    if (combo.length) {
      this.updateWinner(combo);
      return;
    }
    
    this.updateTurn();

    if (this.model.turn >= 225) {
      this.updateTie();
      return;
    }

    const next = this.model.turn % 2;
    if (isAgent(next)) {
      agentWorker.postMessage([next, crossId]);
    }
  }

  agentPlay(crossId) {
    const cross = document.getElementById(`cross-${Model.index17to15(crossId)}`);
    this.playChess(cross, crossId);
  }

  get humanPlay() {
    const game = this;

    return function () {
      if (isAgent(game.model.turn % 2)) {
        return;
      }

      const cross = this;
      const crossId = Model.index15to17(cross.id.split('-')[1] - 0);

      if (!game.model.over && game.model.board(crossId) == -1) {
        game.playChess(cross, crossId);
      }
    }
  }

  updateBoard(cross, crossId, turn) {
    this.model.updateBoard(crossId, turn);
    this.view.updateBoard(cross, turn);
  }

  updateTurn() {
    this.model.updateTurn();
    this.view.updateTurn(this.model.turn);
  }

  updateOver(combo) {
    this.model.updateOver();
    this.view.updateOver(combo.map(Model.index17to15));
  }

  updateWinner(combo) {
    this.model.updateWinner(this.model.turn % 2);
    this.updateOver(combo);
  }

  updateTie() {
    this.updateOver([]);
  }
}