export class View {
  static createDiv(className) {
    const div = document.createElement('div');
    div.className = className;
    return div;
  }

  constructor(boardId, hintId) {
    this.board = document.getElementById(boardId);
    this.hint = document.getElementById(hintId);
  }

  init(ctrler) {
    this.board.innerHTML = '';
    this.hint.style.display = '';
    this.hint.className = 'chess chess-black last-move';
    this.hint.innerHTML = '1';

    for (let i = 0; i < 15*15; ++i) {
      const cross = View.createDiv('cross');
      cross.id = `cross-${i}`;
      cross.onclick = ctrler.humanPlay;

      cross.appendChild(View.createDiv('vl'));
      cross.appendChild(View.createDiv('hl'));

      this.board.appendChild(cross);
    }

    [48, 56, 112, 168, 176].forEach(i => {
      document.getElementById(`cross-${i}`)
        .appendChild(View.createDiv('dot'))
    })
  }

  updateBoard(cross, turn) {
    if (turn) {
      document.querySelector('[last-move]').removeAttribute('last-move')
    }
    const chess = View.createDiv(`chess ${turn % 2 ? 'chess-white' : 'chess-black'}`);
    chess.innerHTML = turn + 1;
    chess.setAttribute('last-move', '');
    cross.appendChild(chess);
  }

  updateTurn(turn) {
    this.hint.className = `chess ${turn % 2 ? 'chess-white' : 'chess-black'}`;
    this.hint.innerHTML = turn + 1;
  }

  updateOver(combo) {
    document.querySelector('[last-move]').removeAttribute('last-move')
    combo.forEach(index => {
      document.getElementById(`cross-${index}`)
        .classList.add('combo');
    });
    this.hint.style.display = 'none';
  }
}