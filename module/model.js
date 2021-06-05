export class Model {
  static index15to17(index) {
    return index + 18 + 2*Math.floor(index / 15);
  }

  static index17to15(index) {
    return 15*(Math.floor(index / 17) - 1) + index%17 - 1;
  }

  constructor() {
    this._board = [];
    this._board.length = 17 * 17;
    this._turn = 0;
    this._over = false;
    this._winner = -1;
  }

  init() {
    this._board.fill(-1);
    this._turn = 0;
    this._over = false;
    this._winner = -1;
  }

  board(index) {
    return this._board[index];
  }

  updateBoard(index, turn) {
    this._board[index] = turn % 2;
  }

  get turn() {
    return this._turn;
  }

  updateTurn() {
    this._turn += 1;
  }

  get over() {
    return this._over
  }

  updateOver() {
    this._over = true;
  }

  get winner() {
    return this._winner;
  }

  updateWinner(winner) {
    this._winner = winner;
  }
}